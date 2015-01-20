#include "BallImage.h"
#include <math.h>
#include <bitset>

namespace tool{
namespace ballimage{

BallImage::BallImage(QWidget *parent) :
    QWidget(parent),
    yHeight(0),
    yWidth(0),
    orangeThresh(200),
    black(new Color(0, 0, 0)),
    imageTabs(new QTabWidget(this)),
    topDisplay(this),
    bottomDisplay(this),
    topImage(base()),
    bottomImage(base()),
    currentCamera(Camera::TOP),
    uVector(-0.374194),
    vVector(0.92735),
    useSigmoid(true),
    paintBlobs(false),
    dotSigMin(4),
    dotSigMax(8)
{
    imagePainters.addModule(topDisplay);
    imagePainters.addModule(bottomDisplay);
    topDisplay.imageIn.wireTo(&topImage, true);
    bottomDisplay.imageIn.wireTo(&bottomImage, true);
    ballImage = NULL;

    mainLayout = new QHBoxLayout;

    imageTabs->addTab(&topDisplay, "TopImage");
    imageTabs->addTab(&bottomDisplay, "BottomImage");
    mainLayout->addWidget(imageTabs);

    QVBoxLayout* aboveBall = new QVBoxLayout;
    QCheckBox* paintBlobs = new QCheckBox(tr("Paint the blobs!"));

    aboveBall->addWidget(paintBlobs);
    QHBoxLayout* aboveBallDot = new QHBoxLayout;
    QCheckBox* sigmoid = new QCheckBox(tr("Use sigmoid filter"));
    dotSigmoidMin = new QLineEdit(tr("4"));
    dotSigmoidMax = new QLineEdit(tr("25"));
    aboveBallDot->addWidget(sigmoid);
    aboveBallDot->addWidget(dotSigmoidMin);
    aboveBallDot->addWidget(dotSigmoidMax);

    QHBoxLayout* stats = new QHBoxLayout;
    logPath = new QLineEdit;
    QPushButton* clearStats = new QPushButton(tr("clear stats"));
    QPushButton* writeStats = new QPushButton(tr("write stats"));
    stats->addWidget(logPath);
    stats->addWidget(clearStats);
    stats->addWidget(writeStats);

    aboveBall->addLayout(aboveBallDot);
    aboveBall->addLayout(stats);

    connect(paintBlobs, SIGNAL(toggled(bool)),
            this, SLOT(togglePaintBlobs(bool)));
    connect(sigmoid, SIGNAL(toggled(bool)),
            this, SLOT(toggleSigmoid(bool)));
    connect(dotSigmoidMin, SIGNAL(editingFinished()),
            this, SLOT(dotSigmoidMinChanged()));
    connect(dotSigmoidMax, SIGNAL(editingFinished()),
            this, SLOT(dotSigmoidMaxChanged()));

    connect(clearStats, SIGNAL(clicked()),
            this, SLOT(resetStats()));
    connect(writeStats, SIGNAL(clicked()),
            this, SLOT(writeStatistics()));

    imagePlaceholder.setAlignment(Qt::AlignTop);

    aboveBall->addWidget(&imagePlaceholder);
    mainLayout->addLayout(aboveBall);

    this->setLayout(mainLayout);

    connect(&topDisplay, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(imageClicked(int, int, int, bool)));
    connect(&bottomDisplay, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(imageClicked(int, int, int, bool)));

    connect(imageTabs, SIGNAL(currentChanged(int)),
            this, SLOT(imageTabChanged(int)));
}

BallImage::~BallImage()
{
    for(int i=0; i<yHeight; i++){
        delete[] ballImage[i];
        delete[] blobImage[i];
    }
    delete[] ballImage;
    delete[] blobImage;

}

void BallImage::allocateBallImage()
{
    yHeight = topImageIn.message().yImage().height();
    yWidth = topImageIn.message().yImage().width();

    ballImage = new int*[yHeight];
    blobImage = new int*[yHeight];

    for(int i=0; i<yHeight; i++){
        ballImage[i] = new int[yWidth];
        blobImage[i] = new int[yWidth];
    }

}

void BallImage::run_()
{
    topImageIn.latch();
    bottomImageIn.latch();
    if(ballImage == NULL) allocateBallImage();

    topImage.setMessage(portals::Message<messages::YUVImage>(
                            &topImageIn.message()));
    bottomImage.setMessage(portals::Message<messages::YUVImage>(
                            &bottomImageIn.message()));
    imagePainters.run();

    resetStats();
    updateBallImages();
}

double BallImage::rateBlob(Blob* b)
{
    double part = b->getAspectRatio() * b->getDensity();

    //double area = M_PI * pow(b->getSecondLength()/2, 2);

    return part;// * area / b->getSum();
}

void BallImage::writeStatistics()
{
    std::cout << "Balls in log ' " << logPath->text().toStdString() <<"'" << std::endl;
    for(int j=0; j<blobs.size(); j++){
        Blob* b = blobs.at(j);
        bool containsBall = false;
        for(int i=0; i<balls.size(); i++){
            point_ ball = balls.at(i);
            if(b->contains(ball)){
                std::cout << "Ball " << i << " contained in blob with confidence: "
                          << b->getAspect() * b->getDensity() * 255 << std::endl;
                containsBall = true;
                balls.erase(balls.begin() + i);
                break;
            }
        }
        if(!containsBall && b->getAspect() * b->getDensity() > 0.8){
            std::cout << "Blob with confidence " <<
                b->getAspect() * b->getDensity() * 255 << " at " <<
                b->getMeanX() << " " << b->getMeanY() <<
                " contained no ball!" << std::endl;
        }
    }

}

void BallImage::updateBallImages()
{
    messages::MemoryImage8 uImage, vImage, yImage;
    if(currentCamera == Camera::TOP)
    {
        uImage = topImageIn.message().uImage();
        vImage = topImageIn.message().vImage();
        yImage = topImageIn.message().yImage();
    }
    else
    {
        std::cout << currentCamera << std::endl;
        uImage = bottomImageIn.message().uImage();
        vImage = bottomImageIn.message().vImage();
        yImage = bottomImageIn.message().yImage();
    }

    QImage image(yImage.width(),
                 uImage.height(),
                 QImage::Format_RGB32);

    for( int i=0; i<yImage.height(); i++){
        for( int j=0; j<yImage.width(); j++){
            // Center about origin
            double ypix = yImage.getPixel(j, i) - 255/2;
            double upix = uImage.getPixel(j/2, i) - 255/2;
            double vpix = vImage.getPixel(j/2, i) - 255/2;

            if(useSigmoid) applySigmoid(&ypix, &upix, &vpix);

            double product = 0;
            if(upix != 0 || vpix != 0)
            {
                // Normalize, but only if we're not going to be diving by 0!
                double yComp = ypix/sqrt(upix*upix + vpix*vpix);
                double uComp = upix/sqrt(upix*upix + vpix*vpix);
                double vComp = vpix/sqrt(upix*upix + vpix*vpix);

                // Perform dot product then scale from 0-1
                product = (uComp*uVector + vComp*vVector + 1) / 2;
            }
            //product = product*product;
            int pixScale = 255*product;
            // if(pixScale > 260){
            //     std::cout << "BIG! " << pixScale << std::endl;
            // }

            ballImage[i][j] = pixScale;
            blobImage[i][j] = 0;

        }
    }
    findBlobs();
    paintBallImages();
}

void BallImage::paintBallImages()
{
    messages::MemoryImage8 uImage, vImage, yImage;
    if(currentCamera == Camera::TOP)
    {
        uImage = topImageIn.message().uImage();
        vImage = topImageIn.message().vImage();
        yImage = topImageIn.message().yImage();
    }
    else
    {
        std::cout << currentCamera << std::endl;
        uImage = bottomImageIn.message().uImage();
        vImage = bottomImageIn.message().vImage();
        yImage = bottomImageIn.message().yImage();
    }

    QImage image(yImage.width(),
                 uImage.height(),
                 QImage::Format_RGB32);

    for( int i=0; i<yImage.height(); i++){
        for( int j=0; j<yImage.width(); j++){
            int pixScale = ballImage[i][j];
            if(!paintBlobs){
                image.setPixel(j, i, qRgb(pixScale, pixScale, pixScale));
            }
            else{
                Color* c = blobColor(blobImage[i][j]);
                image.setPixel(j, i, qRgb(c->getRed(), c->getGreen(), c->getBlue()));
            }
        }
    }

    imagePlaceholder.setPixmap(QPixmap::fromImage(image));

}

void BallImage::findBlobs()
{
    int blobNumber = 0;

    blobs.clear();

    for(int i=0; i<yHeight; i++){
        for(int j=0; j<yWidth; j++){
            if(ballImage[i][j] < orangeThresh) continue;
            if(blobImage[i][j] != 0){
                continue; // already blobbed
            }

            blobNumber++;
            Blob* b = new Blob(blobNumber);
            blobs.push_back(b);
            blobFrom(j, i, b);

            if(b->getSum() < 30){
            //     std::cout << "Rejected blob because size = " << b->getSum()
            //               << std::endl;
                blobs.pop_back();
            //     std::cout << "Vector size: " << blobs.size() << std::endl;
            }
            else{
                //std::cout << "Here??? (" << blobs.size() << ") ";
                //b->setColor(colors.at(blobs.size() + 1));
                //std::cout << "nope!\n";
            }
        }
    }

    // std::cout << "Found " << blobs.size() << " blobs " << blobNumber<<"\n";
    // for(int i=0; i<blobs.size(); i++){
    //     std::cout << "blob: " << i << " had " << blobs.at(i)->getSum() <<
    //         " pixels with density: " << blobs.at(i)->getDensity() << std::endl;
    //     std::cout << "    Length1: " << blobs.at(i)->getFirstLength() <<
    //         "  Length2: " << blobs.at(i)->getSecondLength() << std::endl;
    // }
}

void BallImage::blobFrom(int x, int y, Blob* blob)
{
    int blobID = blob->getID();
    int pixelValue = ballImage[y][x];
    int blobValue = blobImage[y][x];
    if(pixelValue < orangeThresh) return;
    if(blobImage[y][x] != 0) return; // Already part of a blob

    blobImage[y][x] = blobID;

    blob->addPixel(x, y, ((double)ballImage[y][x])/255);

    if(inBounds(x-1, y-1)) blobFrom(x-1, y-1, blob);
    if(inBounds(x, y-1)) blobFrom(x, y-1, blob);
    if(inBounds(x+1, y-1)) blobFrom(x+1, y-1, blob);
    if(inBounds(x-1, y)) blobFrom(x-1, y, blob);
    if(inBounds(x+1, y)) blobFrom(x+1, y, blob);
    if(inBounds(x-1, y+1)) blobFrom(x-1, y+1, blob);
    if(inBounds(x, y+1)) blobFrom(x, y+1, blob);
    if(inBounds(x+1, y+1)) blobFrom(x+1, y+1, blob);
}

Color* BallImage::blobColor(int blobID){
    for(int i = 0; i < blobs.size(); i++){
        if(blobID == blobs.at(i)->getID())
        {
            Blob* b = blobs.at(i);
            double rating = rateBlob(b);//b->getAspectRatio() * b->getDensity();
            int green = rating * 255;
            int red = (1 - rating) * 255;
            return new Color(red, green, 0.0);
        }
    }
    return black;
}

inline bool BallImage::inBounds(int x, int y)
{
    if (x < 0 || x >= yWidth) return false;
    if (y < 0 || y >= yHeight) return false;
    return true;
}

void BallImage::applySigmoid(double* ypix, double* upix, double* vpix)
{
    // upix and vpix will range from 0-255
    // Scaling *small* magnitude vectors to 0
    int magnitude = sqrt((*upix) * (*upix) + (*vpix) * (*vpix));
    double scale = 0;
    if(dotSigMin < magnitude && dotSigMax > magnitude)
    {
        scale = (magnitude - dotSigMin)/(dotSigMax-dotSigMin);
    }
    else if(dotSigMax < magnitude)
    {
        scale = 1;
    }

    // apply scale to the pixels
    *ypix = (*ypix)*scale;
    *upix = (*upix)*scale;
    *vpix = (*vpix)*scale;
}

void BallImage::imageClicked(int x, int y, int brushSize, bool leftClick)
{

    messages::MemoryImage8 uImage, vImage, yImage;
    if(currentCamera == Camera::TOP)
    {
        uImage = topImageIn.message().uImage();
        vImage = topImageIn.message().vImage();
        yImage = topImageIn.message().yImage();
    }
    else
    {
        std::cout << currentCamera << std::endl;
        uImage = bottomImageIn.message().uImage();
        vImage = bottomImageIn.message().vImage();
        yImage = bottomImageIn.message().yImage();
    }

    int ypix = yImage.getPixel(x,y);
    // Center about the orgin
    int upix = uImage.getPixel(x/2, y) - (255/2);
    int vpix = vImage.getPixel(x/2, y) - (255/2);

    if(!leftClick){
        xpos = x;
        ypos = y;
        ballClicked(x, y);
        return;
    }
    // Normalize
    yVector = ypix / sqrt(ypix*ypix+upix*upix+vpix*vpix);
    uVector = upix / sqrt(upix*upix+vpix*vpix);
    vVector = vpix / sqrt(upix*upix+vpix*vpix);

    std::cout << "At click:" << std::endl;
    std::cout << "yPix: " << ypix << " upix: " << upix << " vpix: " << vpix << std::endl;
    std::cout << "vector is: <" << yVector << ", " << uVector << ", " << vVector << ">"<< std::endl;

    updateBallImages();
}

void BallImage::ballClicked(int x, int y){
    point_ location = {(double)x, (double)y};
    balls.push_back(location);
}

void BallImage::resetStats()
{
    balls.clear();
}

void BallImage::imageTabChanged(int i)
{
    // Check if there is a valid widget yet or not.
    // We get this signal when we close the tool.
    if (i == -1)
        return;

    std::cout << "Image log tab switched";
    if (imageTabs->currentWidget() == &topDisplay) {
        currentCamera = Camera::TOP;
        std::cout << " to TOP\n";
    }
    if (imageTabs->currentWidget() == &bottomDisplay) {
        currentCamera = Camera::BOTTOM;
        std::cout << " to BOTTOM\n";
    }
    updateBallImages();
}

void BallImage::toggleSigmoid(bool toggled)
{
    useSigmoid = toggled;
    updateBallImages();
}

void BallImage::togglePaintBlobs(bool toggled)
{
    paintBlobs = toggled;
    updateBallImages();
}

void BallImage::dotSigmoidMinChanged()
{
    bool good;
    int newMin = dotSigmoidMin->text().toInt(&good);

    if(good)
    {
        if (newMin<dotSigMax)
        {
            dotSigMin = newMin;
            std::cout<<"min changed to: "<<newMin << std::endl;
        }
        else
        {
            dotSigmoidMin->setText(tr("Min must be < Max"));
        }
    }
    else
    {
        dotSigmoidMin->setText(tr("Must be integer value"));
    }
    updateBallImages();
}

void BallImage::dotSigmoidMaxChanged()
{
    bool good;
    int newMax = dotSigmoidMax->text().toInt(&good);

    if(good)
    {
        if (newMax>dotSigMin)
        {
            dotSigMax = newMax;
            std::cout<<"max changed to: "<<newMax << std::endl;
        }
        else
        {
            dotSigmoidMax->setText(tr("Min must be < Max"));
        }
    }
    else
    {
        dotSigmoidMax->setText(tr("Must be integer value"));
    }
    updateBallImages();
}

// expects that both ints are in the range[0, 8*sizeof(int) )
// also expects that topSetBit >= bottomSetBit
int BallImage::buildMask(int topSetBit, int bottomSetBit)
{
    int mask = -1;

    mask = mask << (topSetBit - bottomSetBit + 1);
    mask = ~mask;
    mask = mask << bottomSetBit;

    return mask;
}


//---------------------------------BLOB--------------------------------//
Blob::Blob(int bID) :
    blobID(bID),
    count(0),
    sumWeights(0),
    sumX(0),
    sumY(0),
    sumX2(0),
    sumY2(0),
    sumXY(0)
{ }

void Blob::addPixel(int x, int y, double rating)
{
    count++;
    sumWeights += rating;
    sumX       += x*rating;
    sumY       += y*rating;
    sumX2      += x*x*rating;
    sumY2      += y*y*rating;
    sumXY      += x*y*rating;
}

void Blob::compute()
{
    mx = sumWeights * sumX2 - sumX * sumX;
    my = sumWeights * sumY2 - sumY * sumY;
    mxy = 2* (sumWeights * sumXY - sumX * sumY);
    length = sqrt(mxy * mxy + (mx - my) * (mx - my));
    firstLength = sqrt((mx + my + length) / 2) / sumWeights;
    secondLength = sqrt((mx + my - length) / 2) / sumWeights;
    aspectRatio = secondLength / firstLength;
}

bool Blob::contains(point_ location)
{
    compute();
    point_ center = {getMeanX(), getMeanY()};
    double distance = hypot(location.x - center.x, location.y - center.y);
    return (distance < secondLength);
}

}//ballview
}//tool
