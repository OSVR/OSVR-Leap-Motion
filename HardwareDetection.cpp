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
	if ( !mFound ) {
		mFound = true;
		osvr::pluginkit::registerObjectForDeletion(pContext, new ControllerDevice(pContext));
	}

	return OSVR_RETURN_SUCCESS;
}
