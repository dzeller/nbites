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
#include <QLineEdit>
#include <QString>
#include <vector>
#include <iostream>

#include "RoboGrams.h"
#include "image/ImageDisplayModule.h"
#include "Images.h"
#include "Camera.h"

namespace tool{
namespace ballimage{

class Blob;
class Color;

class BallImage: public QWidget, public portals::Module
{
    Q_OBJECT;
public:
    BallImage(QWidget* parent = 0);
    ~BallImage();

public:
    portals::InPortal<messages::YUVImage> topImageIn;
    portals::InPortal<messages::YUVImage> bottomImageIn;

protected slots:
    void imageClicked(int x, int y, int brushSize, bool leftClick);
    void imageTabChanged(int i);
    void toggleSigmoid(bool toggled);
    void togglePaintBlobs(bool toggled);
    void dotSigmoidMinChanged();
    void dotSigmoidMaxChanged();

protected:
    virtual void run_();

private:
    void updateBallImages();
    void paintBallImages();
    void applySigmoid(double* ypix, double* upix, double* vpix);
    void allocateBallImage();
    int buildMask(int topSetBit, int bottomSetBit);
    void findBlobs();
    void blobFrom(int x, int y, Blob* blob);
    Color* blobColor(int blobID);
    bool inBounds(int x, int y);

private:
    int** ballImage;
    int** blobImage;
    int yHeight, yWidth;

    int orangeThresh;

    std::vector<Blob*> blobs;
    Color* black;

    image::ImageDisplayListener topDisplay;
    image::ImageDisplayListener bottomDisplay;

    portals::RoboGram imagePainters;

    portals::OutPortal<messages::YUVImage> topImage;
    portals::OutPortal<messages::YUVImage> bottomImage;

    Camera::Type currentCamera;

    QHBoxLayout* mainLayout;

    QTabWidget* imageTabs;

    QLineEdit* dotSigmoidMin;
    QLineEdit* dotSigmoidMax;

    QLineEdit* thetaBase;
    QLineEdit* thetaSigmoidMin;
    QLineEdit* thetaSigmoidMax;
    QLineEdit* radiusSigmoidMin;
    QLineEdit* radiusSigmoidMax;

    bool useDotProduct, paintBlobs;

    double yVector, uVector, vVector;
    bool useSigmoid;
    int dotSigMin, dotSigMax;

    int xpos, ypos;

    QLabel imagePlaceholder;
//    QImage ballImage;
};

class Blob{
public:
    Blob(int bID = 0);
    //~Blob() {};

    void addPixel(int x, int y, double rating);

    int getID(){ return blobID; }

    double getSum(){ return sumWeights; };
    double getDensity(){ return sumWeights/count; };
    double getMeanX(){ return sumX/sumWeights; };
    double getMeanY(){ return sumY/sumWeights; };
    void setColor(Color* c){ color = c; };
    Color* getColor(){ return color; };
    double getAspect(){ return secondLength/firstLength; };

    double getFirstLength(){ compute(); return firstLength; };
    double getSecondLength(){ compute(); return secondLength; };
    double getAspectRatio() { compute(); return aspectRatio; };

private:
    void compute();
private:
    int blobID;
    Color* color;
    int count;
    double sumWeights, sumX, sumY, sumX2, sumY2, sumXY;
    double mx, my, mxy, length;
    double firstLength, secondLength, aspectRatio;
};

class Color{
public:
    Color(int r = 0, int g = 0, int b = 0) { red=r; green=g; blue=b; };
    int getRed() { return red; };
    int getGreen() { return green; };
    int getBlue() { return blue; };
private:
    int red, green, blue;
};

}
}
