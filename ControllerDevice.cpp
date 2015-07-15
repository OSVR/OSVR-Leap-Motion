#include "ControllerDevice.h"
#include "com_osvr_LeapMotion_json.h"

using namespace Leap;
using namespace LeapOsvr;
using namespace osvr::pluginkit;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
ControllerDevice::ControllerDevice() : mAnalog(NULL), mImaging(NULL), mTracker(NULL) {
	//do nothing...
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode ControllerDevice::operator()(OSVR_PluginRegContext pContext) {
	std::cout << " ### LeapMotionController.isConnected(): " << mController.isConnected() << std::endl;

	/*if ( !mController.isConnected() ) {
		return OSVR_RETURN_FAILURE;
	}*/

	if ( mAnalog == NULL ) {
		initDevice(pContext);
	}

	return OSVR_RETURN_SUCCESS;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode ControllerDevice::update() {
	mAnalog->update();
	mImaging->update();
	mTracker->update();
	return OSVR_RETURN_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void ControllerDevice::initDevice(OSVR_PluginRegContext pContext) {
	mController.setPolicy(Controller::POLICY_BACKGROUND_FRAMES);
	mController.setPolicy(Controller::POLICY_IMAGES);
	mController.setPolicy(Controller::POLICY_OPTIMIZE_HMD);

	OSVR_DeviceInitOptions options = osvrDeviceCreateInitOptions(pContext);

	mAnalog = new Analog(mDeviceToken, options, mController);
	mImaging = new Imaging(mDeviceToken, options, mController);
	mTracker = new Tracker(mDeviceToken, options, mController);

	registerObjectForDeletion(pContext, mAnalog);
	registerObjectForDeletion(pContext, mImaging);
	registerObjectForDeletion(pContext, mTracker);

	mDeviceToken.initAsync(pContext, "Controller", options);
	mDeviceToken.sendJsonDescriptor(com_osvr_LeapMotion_json);
	mDeviceToken.registerUpdateCallback(this);
}
