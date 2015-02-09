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

typedef struct point_{
    double x;
    double y;
} Point;


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
    void toggleEdges(bool toggled);
    void togglePaintBlobs(bool toggled);
    void dotSigmoidMinChanged();
    void dotSigmoidMaxChanged();
    void resetStats();
    void writeStatistics();

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

    int walkBlobEdge(Blob* b);
    int neighborhoodState(Point pixel);
    int nextDirection(Point current, int prevDir);

    double rateBlob(Blob* b);

    // Statistics functions
    void ballClicked(int x, int y);


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
    QLineEdit* logPath;

    bool useDotProduct, paintBlobs;

    double yVector, uVector, vVector;
    bool paintEdges;
    int dotSigMin, dotSigMax;

    int xpos, ypos;

    QLabel imagePlaceholder;

    // For stats gathering
    std::vector<point_> balls;

    static const int N = 0;
    static const int E = 1;
    static const int S = 2;
    static const int W = 3;
};

class Blob{
public:
    Blob(int bID = 0);
    //~Blob() {};

    void addPixel(int x, int y, double rating);

    int getID(){ return blobID; }

    double getSum(){ return sumWeights; };
    int getCount() { return count; };
    double getDensity(){ return sumWeights/count; };
    double getMeanX(){ return sumX/sumWeights; };
    double getMeanY(){ return sumY/sumWeights; };
    void setColor(Color* c){ color = c; };
    Color* getColor(){ return color; };
    double getAspect(){ return secondLength/firstLength; };

    double getFirstLength(){ compute(); return firstLength; };
    double getSecondLength(){ compute(); return secondLength; };
    double getAspectRatio() { compute(); return aspectRatio; };

    // NOT a smart solution. Treats blob as a circle with radius of secondLength
    bool contains(point_ location);
    void setRating(double _rating) { rating = _rating; };
    void setCircum(int _circum) { circum = _circum; };
    double getRating() { return rating; };
    int getCircum() { return circum; };
    Point getTopLeft() { return topLeft; };

    int getWidth() { return maxX - minX; };

private:
    void compute();
private:
    int blobID;
    Color* color;
    int count;
    double sumWeights, sumX, sumY, sumX2, sumY2, sumXY;
    double mx, my, mxy, length;
    double firstLength, secondLength, aspectRatio;
    double rating;
    int circum, minX, maxX;
    Point topLeft;
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
