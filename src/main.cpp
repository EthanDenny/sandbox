#include "Sandbox.h"

#include <sstream>
#include <iostream>

int main()
{
    PROFILE_FUNCTION();

    std::cout << cv::getBuildInformation() << std::endl;
    cv::setNumThreads(cv::getNumberOfCPUs());

    GameEngine engine;
    engine.changeScene<Scene_Main>("Menu");
    engine.run();

    return 0;
}

