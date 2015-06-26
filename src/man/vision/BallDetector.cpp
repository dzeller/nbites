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

    // Build dummy image so that destruction is guarenteed to succeed
    int igw = 1;
    int igh = 1;
    uint8_t* buff = new uint8_t[igw * igh];
    ImageLiteU8 ig(igw, igh, igw, buff);
    inverseGreen = ig;
}

BallDetector::~BallDetector()
{
    delete inverseGreen.pixelAddr();
}

bool BallDetector::findBall(ImageLiteU8 orange, ImageLiteU8 green, ImageLiteU8 white, double cameraHeight)
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
#ifdef OFFLINE
            std::cout << "BLOB's above horizon:" << std::endl;
#endif
            continue;
        }

        Ball b((*i), x_rel, -1 * y_rel, cameraHeight, orange.height(), orange.width());
        if (b.confidence() > .5) {
#ifdef OFFLINE
            candidates.push_back(b);
            std::cout << "accepted ball because:\n" << b.properties() << std::endl;
#endif
            if (b.confidence() > _best.confidence()) {
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
         Blob b = _best.getBlob();
         int x0 = b.centerX() - b.firstPrincipalLength() * 3;
         int y0 = b.centerY() - b.firstPrincipalLength() * 3;
         buildInverseGreen(green, white, x0, y0, b.firstPrincipalLength()*6,
                           b.firstPrincipalLength()*6);
         return true;
    }
    else {
        return false;
    }
}

ImageLiteU8 BallDetector::buildInverseGreen(const ImageLiteU8 green, const ImageLiteU8 white,
                                            int x0, int y0, int ht, int wd)
{
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x0 + wd > green.width()) wd = green.width() - x0;
    if (y0 + ht > green.height()) ht = green.height() - y0;

    ImageLiteU8 gWindow(green, x0, y0, wd, ht);
    ImageLiteU8 wWindow(white, x0, y0, wd, ht);
    uint8_t* buffer = new uint8_t[ht * wd];

    for (int i=0; i<ht; i++) {
        for (int j=0; j<wd; j++) {
            uint8_t gv = 255 - (*gWindow.pixelAddr(j, i));
            uint8_t wv = 255 - (*wWindow.pixelAddr(j, i));
            buffer[i*wd + j] = min(gv, wv);
        }
    }

    ImageLiteU8 ig(wd, ht, wd, buffer);
    return ig;
}

Ball::Ball(Blob& b, double x_, double y_, double cameraH_, int imgHeight_, int imgWidth_) :
    blob(b),
    radThresh(.1, .5),
    thresh(.5, .8),
    x_rel(x_),
    y_rel(y_),
    cameraH(cameraH_),
    imgHeight(imgHeight_),
    imgWidth(imgWidth_),
    _confidence(0)
{
    compute();
}

Ball::Ball() :
    blob(0),
    thresh(0, 0),
    radThresh(0, 0),
    _confidence(0)
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

    //_confidence = (density > thresh).f() * (aspectRatio > thresh).f() * (diameterRatio > radThresh).f();
    _confidence = ((density > thresh) & (aspectRatio > thresh) & (diameterRatio > radThresh)).f();
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
