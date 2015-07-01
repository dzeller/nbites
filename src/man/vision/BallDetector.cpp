#include "BallDetector.h"

#include <algorithm>
#include <iostream>


using std::to_string;

namespace man {
namespace vision {

BallDetector::BallDetector(FieldHomography* homography_, bool topCamera_):
    blobber(),
    homography(homography_),
    topCamera(topCamera_)
{
    blobber.secondThreshold(115);
    blobber.minWeight(4);
}

BallDetector::~BallDetector() { }

bool BallDetector::findBall(ImageLiteU8 orange, double cameraHeight)
{
#ifdef OFFLINE
    candidates.clear();
#endif

    blobber.run(orange.pixelAddr(), orange.width(), orange.height(), orange.pitch());

    Ball reset;
    _best = reset;

    // TODO: Sort blobber list by size
    for (auto i=blobber.blobs.begin(); i!=blobber.blobs.end(); i++) {
        double x_rel, y_rel;

        double bIX = ((*i).centerX() - orange.width()/2);
        double bIY = (orange.height() / 2 - (*i).centerY()) - (*i).firstPrincipalLength();

        bool belowHoriz = homography->fieldCoords(bIX, bIY, x_rel, y_rel);

        // This blob is above the horizon. Can't be a ball
        if (!belowHoriz) {
            continue;
        }

        Ball b((*i), x_rel, -1 * y_rel, cameraHeight, orange, topCamera);

        if (b.confidence() > .5) {
#ifdef OFFLINE
            candidates.push_back(b);
            std::cout << "accepted ball because:\n" << b.properties() << std::endl;
#endif
            if (b.dist < _best.dist) {
                _best = b;
            }
        }
        else {
            // TODO: ball debug flag
#ifdef OFFLINE
            std::cout << "declined ball because:\n" << b.properties() << std::endl;
#endif
        }
    }
    if (_best.confidence() > .5) {
        return true;
    }
    else {
        return false;
    }
}

std::vector<int> BallDetector::octantCheck(Blob& b, ImageLiteU8 orange, int thresh)
{
    std::vector<int> toReturn;

    // TODO: maybe round rather than just cast?
    int cX = static_cast<int>(b.centerX());
    int cY = static_cast<int>(b.centerY());
    int pL = static_cast<int>(b.firstPrincipalLength());
    int iH = orange.height();
    int iW = orange.width();

    for (int dx=-1; dx <= 1; dx++) {
        for (int dy=-1; dy <= 1; dy++) {
            if (dx==0 && dy==0) continue;

            int currX = cX;
            int currY = cY;
            int count=0;

            for(; count < pL; count++) {
                // TODO make this look nice
                if (currX == 0
                    || currY == 0
                    || currX == iH-1
                    || currY == iW-1)
                    break;

                currX += dx;
                currY += dy;

                int px = *(orange.pixelAddr(currX, currY));
                if (px < thresh) {
                    int count2 = 0;
                    bool stillOK = false;
                    for (; count2 < pL / 5 && count + count2 < pL; count2++) {
                        // TODO make this look nice
                        if (currX == 0
                            || currY == 0
                            || currX == iH-1
                            || currY == iW-1)
                            break;
                        currX += dx;
                        currY += dy;
                        px = *(orange.pixelAddr(currX, currY));
                        if (px > thresh) {
                            stillOK = true;
                            break;
                        }
                    }
                    if (stillOK) {
                        count += count2;
                        continue;
                    }
                    break;
                }
            }
            toReturn.push_back(count);
        }
    }
    return toReturn;
}

Ball::Ball(Blob& b, double x_, double y_, double cameraH_, ImageLiteU8 orange_, bool top) :
    blob(b),
    radThresh(.3, .7),
    thresh(.5, .8),
    x_rel(x_),
    y_rel(y_),
    cameraH(cameraH_),
    orange(orange_),
    imgHeight(orange.height()),
    imgWidth(orange.width()),
    _confidence(0)
{
    if (!top) {
        radThresh = thresh;
    }
    compute();
}

Ball::Ball() :
    blob(0),
    thresh(0, 0),
    radThresh(0, 0),
    _confidence(0),
    dist(200000)
{ }

void Ball::compute()
{
    dist = hypot(x_rel, y_rel);
    double density = blob.area() / blob.count();
    double aspectRatio = (blob.secondPrincipalLength() /
                          blob.firstPrincipalLength());

    double hypotDist = hypot(dist, cameraH);

    expectedDiam = pixDiameterFromDist(hypotDist);

    diameterRatio;
    if (expectedDiam > 2 * blob.firstPrincipalLength())
        diameterRatio = 2*blob.firstPrincipalLength() / expectedDiam;
    else
        diameterRatio = expectedDiam / (2 * blob.firstPrincipalLength());

    expectedArea = M_PI * pow(blob.firstPrincipalLength(), 2);
    areaRatio;

    if (expectedArea > blob.count())
        areaRatio = blob.count() / expectedArea;
    else
        areaRatio = expectedArea / blob.count();

    //_confidence = (density > thresh).f() * (aspectRatio > thresh).f() * (diameterRatio > radThresh).f();
    _confidence = ((density > thresh) & (aspectRatio > thresh)
                   & (areaRatio > thresh) & (diameterRatio > radThresh)).f();

    // Hack/Sanity check to ensure we don't see crazy balls
    if (dist > 600) _confidence = 0;
    std::vector<int> octants = BallDetector::octantCheck(blob, orange, 175);
    std::sort(octants.begin(), octants.end());

    if ( 2 * octants[0] < octants[7] && blob.firstPrincipalLength() > 3) {
#ifdef OFFLINE
        std::cout << "Killing ball because of octants:" << std::endl;
        for (int i=0; i<8; i++){
            std::cout << "\t" << octants[i] << std::endl;
        }

#endif
        _confidence = 0;
    }
}

std::string Ball::properties()
{
    std::string d("====Ball properties:====\n");
    d += "\tRelativePosition: " + to_string(x_rel) + " "+ to_string( y_rel) + "\n";
    d += "\tHomographyDistance is: " + to_string(dist) + "\n";
    d += "\tdensity is: " + to_string(blob.area() / blob.count()) + "\n";
    d += "\tcount is: " + to_string(blob.count()) + "\n";
    d += "\tlocated: (" + to_string(blob.centerX()) +  ", " +
        to_string(blob.centerY()) + ")\n";
    d += "\tprinceLens: " + to_string(blob.firstPrincipalLength()) + " " +
        to_string(blob.secondPrincipalLength()) + "\n";
    d += "\taspectR is: " + to_string(blob.secondPrincipalLength() /
                                      blob.firstPrincipalLength()) + "\n";
    d += "\texpect ball to be this many pix: " + to_string(expectedDiam) + "\n";
    d += "\tdiamRatio: " + to_string(diameterRatio) + "\n";
    d += "\tdiam Confidence: " + to_string((diameterRatio> radThresh).f()) + "\n";
    d += "\texpect ball area to be: " + to_string(expectedArea) + "\n";
    d += "\tareaRatio: " + to_string(areaRatio) + "\n";
    d += "\n\tconfidence is: " + to_string(_confidence) + "\n====================\n";
    return d;
}

// The expected diameter of ball in image at distance d in CM
double Ball::pixDiameterFromDist(double d) const
{
    double trig = atan(BALL_RADIUS / d);
    return 2 * imgHeight * trig / VERT_FOV_RAD;
}

}
}
