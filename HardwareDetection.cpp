#include "HardwareDetection.h"
#include "ControllerDevice.h"

using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
HardwareDetection::HardwareDetection() : mFound(false) {
	//do nothing...
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode HardwareDetection::operator()(OSVR_PluginRegContext pContext) {
	Leap::Controller controller;

	if ( !controller.isConnected() ) {
		mFound = false;
		return OSVR_RETURN_FAILURE;
	}

	if ( !mFound ) {
		mFound = true;
		osvr::pluginkit::registerObjectForDeletion(pContext, new ControllerDevice(pContext));
	}

	return OSVR_RETURN_SUCCESS;
}
