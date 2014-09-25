#pragma once

#include "EmptyTool.h"

#include "ballimage/BallImage.h"

namespace tool {

class BallImageTool : public EmptyTool {
    Q_OBJECT;

public:
    BallImageTool(const char* title = "BallImageTool");
    ~BallImageTool();

public slots:
    void setUpModules();

protected:
    ballimage::BallImage ballImage;
};

} // namespace tool
