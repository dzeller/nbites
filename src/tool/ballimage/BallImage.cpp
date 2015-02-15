#include "BallImage.h"
#include <math.h>
#include <bitset>

#include <stdlib.h>
#include <time.h>

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
    paintEdges(false),
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
    QCheckBox* edges = new QCheckBox(tr("Paint edges"));
    dotSigmoidMin = new QLineEdit(tr("4"));
    dotSigmoidMax = new QLineEdit(tr("25"));
    aboveBallDot->addWidget(edges);
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
    connect(edges, SIGNAL(toggled(bool)),
            this, SLOT(toggleEdges(bool)));
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

    std::cout << "__________________________________________________________" << std::endl;

    resetStats();
    updateBallImages();
}

double BallImage::rateBlob(Blob* b)
{
    double part = b->getAspectRatio() * b->getDensity();

    double area = M_PI * pow(b->getFirstLength(), 2);

    //TODO: It shouldn't be 2.5 below!
    double circumtoArea = 2 * 2.5 * (double)b->getCount() / (b->getWidth() * b->getCircum());
    if(part*circumtoArea > 0.5){
        //std::cout << "A to C was: " << circumtoArea << std::endl;
        std::cout << "Rating was: " << part << " C2A is: " << circumtoArea << " Area: " << b->getCount() << " Rating is: " << part*circumtoArea << std::endl;
        //std::cout << " firstLength: " << b->getFirstLength() << " secondLength: " << b->getSecondLength() << std::endl;
        Circle c = fitCircle(b);
        std::cout << "Circle was at ("<< c.center.x << ", " << c.center.y << ") radius: " << c.radius << std::endl;
        circles.push_back(c);
    }

    return part *  circumtoArea;// * area / b->getSum();
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

            applySigmoid(&ypix, &upix, &vpix);

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
    for(int i=0; i<blobs.size(); i++){
        Blob* b = blobs.at(i);
        int circum = walkBlobEdge(b);
        b->setCircum(circum);
        b->setRating(rateBlob(b));
    }
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
                if(pixScale < 1000){
                    image.setPixel(j, i, qRgb(pixScale, pixScale, pixScale));
                }
                else{
                    image.setPixel(j, i, qRgb(250, 0, 0));
                }
            }
            else{
                Color* c = blobColor(blobImage[i][j]);
                image.setPixel(j, i, qRgb(c->getRed(), c->getGreen(), c->getBlue()));
            }
        }
    }

    QPainter painter(&image);
    painter.setPen(Qt::blue);

    for(int i=0; i<circles.size(); i++){
        Circle c = circles.at(i);
        QPointF center(c.center.x, c.center.y);
        painter.drawEllipse(center, (qreal)c.radius, (qreal)c.radius);
        std::cout << "drew ellipse...\n";
    }

    circles.clear();

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
}

Circle BallImage::fitCircle(Blob* b){
    Point centerMass = {(double)b->getMeanX(), (double)b->getMeanY()};
    std::cout << b->getMeanX() << " " << b->getMeanY() << " " << b->getWidth() << std::endl;
    Circle best = {centerMass, b->getWidth()/2.0};
    int bestRating;
    int delta = 3.0;

    std::vector<Point> edge = b->getEdge();
    int numEdge = edge.size();

    bestRating = rateCircle(best, edge, delta);

    srand(time(NULL));

    // for(int i=0; i<numEdge; i++){
    //     Point p = edge.at(i);
    //     std::cout << "Point p"<<i<< "={" << p.x << "," <<p.y <<"};"<<std::endl;
    //     std::cout << "edges.push_back(p" << i <<");"<<std::endl;
    // }

    for(int i=0; i< numEdge / 4; i++){
        Point one = edge.at(rand()%numEdge);
        Point two = edge.at(rand()%numEdge);
        Point three = edge.at(rand()%numEdge);

        Circle generated = circleFromPoints(one, two, three);
        int rating = rateCircle(generated, edge, delta);

        if(rating > bestRating){
            best = generated;
            bestRating = rating;
            std::cout << "Best rating was " << bestRating << " from ("<<best.center.x <<", " << best.center.y << ") rad: " << best.radius <<std::endl;
        }
    }
    return best;
}

int BallImage::rateCircle(Circle c, std::vector<Point> edge, int errorThresh){
    int count = 0;
    Point center = c.center;

    for(int i=0; i<edge.size(); i++){
        if(hypot(center.x - edge.at(i).x, center.y - edge.at(i).y) - c.radius < errorThresh){
            count++;
        }
    }

    return count;
}

// Calculates the circle which intersects the three given points
// Done by applying Cramers rule to linear system of eqtn
Circle BallImage::circleFromPoints(Point a, Point b, Point c)
{
    // Based on Cramer's rule
    double detBase = (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y);
    double constA = ((b.x*b.x - a.x*a.x) + (b.y*b.y - a.y*a.y)) / 2.0;
    double constB = ((c.x*c.x - a.x*a.x) + (c.y*c.y - a.y*a.y)) / 2.0;

    double xCenter = ((c.y-a.y)*constA - (b.y-a.y)*constB) / detBase;
    double yCenter = ((b.x-a.x)*constB - (c.x-a.x)*constA) / detBase;
    double radius = hypot(a.x - xCenter, a.y - yCenter);

    Point center = {xCenter, yCenter};
    Circle circle = {center, radius};

    return circle;
}

void BallImage::blobFrom(int x, int y, Blob* blob)
{
    int blobID = blob->getID();
    int pixelValue = ballImage[y][x];
    int blobValue = blobImage[y][x];
    if(pixelValue < orangeThresh) return;
    if(blobImage[y][x] != 0) return; //Already part of a blob

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

// Walks the edge of the blob and returns the circumference of the blob
int BallImage::walkBlobEdge(Blob* b)
{
    Point start = b->getTopLeft();
    Point current = b->getTopLeft();
    int dir = N;
    int count = 0;

    do{
        if(inBounds(current.x, current.y) && paintEdges){
            ballImage[(int)current.y][(int)current.x] = 1000;
        }
        count++;
        b->addEdgePoint(current);
        dir = nextDirection(current, dir);

        switch(dir){
        case N:
            current.y -= 1;
            break;
        case E:
            current.x += 1;
            break;
        case S:
            current.y += 1;
            break;
        case W:
            current.x -= 1;
            break;
        default:
            std::cout << "Illegal direction returned! " << dir  << std::endl;
            return 1000;
        }

    } while(current.x != start.x || current.y != start.y);

    return count;
}


// Returns an int in range [0, 15] corresponding to the configuration of pixels
// that are part of a blob.
// ASSUMPTION: no two blobs can be touching.
int BallImage::neighborhoodState(Point pixel)
{
    int x = pixel.x;
    int y = pixel.y;

    int state = 0;
    if(inBounds(x-1, y-1) && blobImage[y-1][x-1]) state |= 1;
    if(inBounds(x, y-1) && blobImage[y-1][x]) state |= 2;
    if(inBounds(x-1, y) && blobImage[y][x-1]) state |= 4;
    if(inBounds(x, y) && blobImage[y][x]) state |= 8;

    return state;
}

int BallImage::nextDirection(Point current, int prevDir)
{
    int state = neighborhoodState(current);
    // would be better as a LUT
    switch(state){
    case 0: return -1;
    case 1: return W;
    case 2: return N;
    case 3: return W;
    case 4: return S;
    case 5: return S;
    case 6:
        if(prevDir == E) return N;
        if(prevDir == W) return S;
        return -1;
    case 7: return S;
    case 8: return E;
    case 9:
        if(prevDir == S) return E;
        if(prevDir == N) return W;
        return -1;
    case 10: return N;
    case 11: return W;
    case 12: return E;
    case 13: return E;
    case 14: return N;
    default: return -1;
    }
}

Color* BallImage::blobColor(int blobID){
    for(int i = 0; i < blobs.size(); i++){
        if(blobID == blobs.at(i)->getID())
        {
            Blob* b = blobs.at(i);
            double rating = b->getRating();//getAspectRatio() * b->getDensity();rateBlob(b);
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

void BallImage::toggleEdges(bool toggled)
{
    paintEdges = toggled;
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
    sumXY(0),
    minX(10000),
    maxX(0)
{
    topLeft.x = 10000;
    topLeft.y = 10000;
}

void Blob::addPixel(int x, int y, double rating)
{
    count++;
    sumWeights += rating;
    sumX       += x*rating;
    sumY       += y*rating;
    sumX2      += x*x*rating;
    sumY2      += y*y*rating;
    sumXY      += x*y*rating;

    if(y < topLeft.y){
        topLeft.y = y;
        topLeft.x = x;
    }
    else if(y == topLeft.y){
        if(x < topLeft.x){
            topLeft.y = y;
            topLeft.x = x;
        }
    }

    if(x < minX) minX = x;
    if(x > maxX) maxX = x;
}

void Blob::addEdgePoint(Point p)
{
    edge.push_back(p);
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
