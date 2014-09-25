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
    uVector(sqrt(2)),
    vVector(sqrt(2))
{
    imagePainters.addModule(topDisplay);
    imagePainters.addModule(bottomDisplay);
    topDisplay.imageIn.wireTo(&topImage, true);
    bottomDisplay.imageIn.wireTo(&bottomImage, true);

    mainLayout = new QHBoxLayout;

    imageTabs->addTab(&topDisplay, "TopImage");
    imageTabs->addTab(&bottomDisplay, "BottomImage");
    mainLayout->addWidget(imageTabs);
    imagePlaceholder.setAlignment(Qt::AlignTop);
    mainLayout->addWidget(&imagePlaceholder);

    this->setLayout(mainLayout);

    connect(&topDisplay, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(imageClicked(int, int, int, bool)));
    connect(&bottomDisplay, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(imageClicked(int, int, int, bool)));
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
    messages::MemoryImage8 uImage = topImageIn.message().uImage();
    messages::MemoryImage8 vImage = topImageIn.message().vImage();

    QImage image(uImage.width()*2,
                 uImage.height(),
                 QImage::Format_RGB32);

    for( int i=0; i<uImage.height(); i++){
        for( int j=0; j<uImage.width(); j++){
            // Center about origin
            int upix = uImage.getPixel(j/2, i) - 255/2;
            int vpix = vImage.getPixel(j/2, i) - 255/2;

            // Normalize
            double uComp = upix/sqrt(upix*upix + vpix*vpix);
            double vComp = vpix/sqrt(upix*upix + vpix*vpix);

            // Perform dot product then scale from 0-1
            double product = (uComp*uVector + vComp*vVector + 1) / 2;
            int pixScale = 255*product;

            image.setPixel(2*j, i, qRgb(pixScale, pixScale, pixScale));
            image.setPixel(2*j + 1, i, qRgb(pixScale, pixScale, pixScale));
        }
    }
    imagePlaceholder.setPixmap(QPixmap::fromImage(image));
}

void BallImage::imageClicked(int x, int y, int brushSize, bool leftClick)
{
    messages::MemoryImage8 yImage = topImageIn.message().yImage();
    messages::MemoryImage8 uImage = topImageIn.message().uImage();
    messages::MemoryImage8 vImage = topImageIn.message().vImage();

    int ypix = yImage.getPixel(x,y);
    // Center about the orgin
    int upix = uImage.getPixel(x/2, y) - (255/2);
    int vpix = vImage.getPixel(x/2, y) - (255/2);

    std::cout << "At click:" << std::endl;
    std::cout << "yPix: " << ypix << " upix: " << upix << " vpix: " << vpix << std::endl;
    // Normalize
    uVector = upix / sqrt(upix*upix+vpix*vpix);
    vVector = vpix / sqrt(upix*upix+vpix*vpix);

    std::cout << "Vector produced is of form <u,v>: <"<<uVector<<","<<vVector<<">\n";
    updateBallImages();
}

}//ballview
}//tool
