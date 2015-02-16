
#pragma once

#include <vector>
#include <iostream>

#include "RoboGrams.h"
#include "Images.h"
#include "Camera.h"
#include "ballimage/BallImage.h"
#include "unlog/UnlogModule.h"

namespace tool {
namespace ballimage {

class BallTest: public portals::Module
{
public:
    BallTest();

private:
    //UnlogModule<messages::YUVImage> unlogger;

protected:
    void run_();

};

}
}
