#include "HardwareDetection.h"
#include "ControllerDevice.h"

using namespace LeapOsvr;

static const auto PREFIX = "[OSVR-Leap-Motion] ";

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
HardwareDetection::HardwareDetection() : mFound(false) {
	//do nothing...
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode HardwareDetection::operator()(OSVR_PluginRegContext pContext) {
    Leap::Controller controller;

    if (!controller.isConnected()) {
        /// sometimes it takes a little while for device to respond
        std::cout << PREFIX << "Waiting for Leap Motion controller to connect"
                  << std::endl;
        int numTries = 0;
        /// wait for a second for device to respond
        while (!controller.isConnected() && numTries++ < 5) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        if (controller.isConnected()) {
            std::cout << PREFIX << "Controller is connected" << std::endl;
        } else {
            std::cout << PREFIX << "Controller is not connected" << std::endl;
            mFound = false;
            return OSVR_RETURN_FAILURE;
        }
    }

    if (!mFound) {
        mFound = true;
        osvr::pluginkit::registerObjectForDeletion(
            pContext, new ControllerDevice(pContext));
    }

    return OSVR_RETURN_SUCCESS;
}
