#include "BallTest.h"
#include <iostream>

namespace tool {
namespace ballimage{

BallTest::BallTest(){

    std::cout << "GOtcha" << std::endl;

    unlog::UnlogModule<messages::YUVImage> unlogger("LOL");

}

void BallTest::run_(){

}

}
}
