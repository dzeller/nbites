#include "BallImage.h"
#include <math.h>

namespace tool{
namespace ballimage{

BallImage::BallImage(QWidget *parent) :
    QWidget(parent),
    imageTabs(new QTabWidget(this)),
    topDisplay(this),
    bottomDisplay(this),
    topImage(base()),
    bottomImage(base()),
    currentCamera(Camera::TOP),
    uVector(sqrt(2)),
    vVector(sqrt(2)),
    useSigmoid(false),
    sigMin(4),
    sigMax(8)
{
    imagePainters.addModule(topDisplay);
    imagePainters.addModule(bottomDisplay);
    topDisplay.imageIn.wireTo(&topImage, true);
    bottomDisplay.imageIn.wireTo(&bottomImage, true);

    mainLayout = new QHBoxLayout;

    imageTabs->addTab(&topDisplay, "TopImage");
    imageTabs->addTab(&bottomDisplay, "BottomImage");
    mainLayout->addWidget(imageTabs);

    QHBoxLayout* aboveBall = new QHBoxLayout;
    QCheckBox* sigmoid = new QCheckBox(tr("Use sigmoid filter"));
    sigmoidMin = new QLineEdit(tr("4"));
    sigmoidMax = new QLineEdit(tr("8"));
    aboveBall->addWidget(sigmoid);
    aboveBall->addWidget(sigmoidMin);
    aboveBall->addWidget(sigmoidMax);

    connect(sigmoid, SIGNAL(toggled(bool)),
            this, SLOT(toggleSigmoid(bool)));
    connect(sigmoidMin, SIGNAL(editingFinished()),
            this, SLOT(sigmoidMinChanged()));
    connect(sigmoidMax, SIGNAL(editingFinished()),
            this, SLOT(sigmoidMaxChanged()));


    QVBoxLayout* right = new QVBoxLayout;
    imagePlaceholder.setAlignment(Qt::AlignTop);
    right->addLayout(aboveBall);
    right->addWidget(&imagePlaceholder);
    mainLayout->addLayout(right);

    this->setLayout(mainLayout);

    connect(&topDisplay, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(imageClicked(int, int, int, bool)));
    connect(&bottomDisplay, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(imageClicked(int, int, int, bool)));
    connect(imageTabs, SIGNAL(currentChanged(int)),
            this, SLOT(imageTabChanged(int)));
}

void BallImage::run_()
{
    topImageIn.latch();
    bottomImageIn.latch();

    topImage.setMessage(portals::Message<messages::YUVImage>(
                            &topImageIn.message()));
    bottomImage.setMessage(portals::Message<messages::YUVImage>(
                            &bottomImageIn.message()));
    imagePainters.run();

    updateBallImages();
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
            double upix = uImage.getPixel(j/2, i) - 255/2;
            double vpix = vImage.getPixel(j/2, i) - 255/2;

            if(useSigmoid) applySigmoid(&upix, &vpix);

            double product = 0;
            if(upix != 0 || vpix != 0)
            {
                // Normalize, but only if we're not going to be diving by 0!
                double uComp = upix/sqrt(upix*upix + vpix*vpix);
                double vComp = vpix/sqrt(upix*upix + vpix*vpix);

                // Perform dot product then scale from 0-1
                product = (uComp*uVector + vComp*vVector + 1) / 2;
            }
            int pixScale = 255*product;
            image.setPixel(j, i, qRgb(pixScale, pixScale, pixScale));
        }
    }
    imagePlaceholder.setPixmap(QPixmap::fromImage(image));
}

void BallImage::applySigmoid(double* upix, double* vpix)
{
    // upix and vpix will range from 0-255
    // Scaling *small* magnitude vectors to 0
    int magnitude = sqrt((*upix) * (*upix) + (*vpix) * (*vpix));
    double scale = 0;
    if(sigMin < magnitude && sigMax > magnitude)
    {
        scale = (magnitude - sigMin)/(sigMax-sigMin);
    }
    else if(sigMax < magnitude)
    {
        scale = 1;
    }

    // apply scale to the pixels
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

    std::cout << "At click:" << std::endl;
    std::cout << "yPix: " << ypix << " upix: " << upix << " vpix: " << vpix << std::endl;
    // Normalize
    uVector = upix / sqrt(upix*upix+vpix*vpix);
    vVector = vpix / sqrt(upix*upix+vpix*vpix);

    updateBallImages();
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

void BallImage::sigmoidMinChanged()
{
    bool good;
    int newMin = sigmoidMin->text().toInt(&good);

    if(good)
    {
        if (newMin<sigMax)
        {
            sigMin = newMin;
            std::cout<<"min changed to: "<<newMin << std::endl;
        }
        else
        {
            sigmoidMin->setText(tr("Min must be < Max"));
        }
    }
    else
    {
        sigmoidMin->setText(tr("Must be integer value"));
    }
    updateBallImages();
}

void BallImage::sigmoidMaxChanged()
{
    bool good;
    int newMax = sigmoidMax->text().toInt(&good);

    if(good)
    {
        if (newMax>sigMin)
        {
            sigMax = newMax;
            std::cout<<"max changed to: "<<newMax << std::endl;
        }
        else
        {
            sigmoidMax->setText(tr("Min must be < Max"));
        }
    }
    else
    {
        sigmoidMax->setText(tr("Must be integer value"));
    }
    updateBallImages();
}

}//ballview
}//tool
