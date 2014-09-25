#include "BallImageTool.h"

namespace tool {

BallImageTool::BallImageTool(const char* title) :
    ballImage(this),
    EmptyTool(title)
{
    toolTabs->addTab(&ballImage, tr("BallImageeeeeeeeeeeeeee"));

    connect(&diagram, SIGNAL(signalUnloggersReady()),
            this, SLOT(setUpModules()) );
}

BallImageTool::~BallImageTool() {
}

void BallImageTool::setUpModules()
{
    if(diagram.connectToUnlogger<messages::YUVImage>(ballImage.topImageIn, "top") &&
        diagram.connectToUnlogger<messages::YUVImage>(ballImage.bottomImageIn, "bottom"))
        std::cout << "Loaded image logs" << std::endl;
    else std::cout << "No images in this log..." << std::endl;

    diagram.addModule(ballImage);
}

} // namespace tool
