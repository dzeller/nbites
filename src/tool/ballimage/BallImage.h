/**
 * Test tool for creating an image which rates each pixel in how much of a
 * certain color it is. 0-255.
 *
 * @author Daniel Zeller
 * @date September 2014
 */
#pragma once

#include <QtGui>
#include <QCheckBox>
#include <vector>
#include <iostream>

#include "RoboGrams.h"
#include "image/ImageDisplayModule.h"
#include "Images.h"

namespace tool{
namespace ballimage{

class BallImage: public QWidget, public portals::Module
{
    Q_OBJECT;
public:
    BallImage(QWidget* parent = 0);
    ~BallImage(){}

public:
    portals::InPortal<messages::YUVImage> topImageIn;
    portals::InPortal<messages::YUVImage> bottomImageIn;

protected slots:
    void imageClicked(int x, int y, int brushSize, bool leftClick);

protected:
    virtual void run_();

private:
    void updateBallImages();

    QTabWidget* imageTabs;

    image::ImageDisplayListener topDisplay;
    image::ImageDisplayListener bottomDisplay;

    portals::RoboGram imagePainters;

    portals::OutPortal<messages::YUVImage> topImage;
    portals::OutPortal<messages::YUVImage> bottomImage;

    QHBoxLayout* mainLayout;

    double uVector, vVector;

    QLabel imagePlaceholder;
//    QImage ballImage;
};
}
}
