#include "ControllerDevice.h"
#include "com_osvr_LeapMotion_json.h"

using namespace Leap;
using namespace LeapOsvr;
using namespace osvr::pluginkit;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
ControllerDevice::ControllerDevice() : mLeapData(NULL), mAnalog(NULL), mImaging(NULL),
													mTracker(NULL), mGestures(NULL), mConfigure(NULL) {
	//do nothing...
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode ControllerDevice::operator()(OSVR_PluginRegContext pContext) {
	if ( mAnalog == NULL ) {
		initDevice(pContext);
	}

	return OSVR_RETURN_SUCCESS;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode ControllerDevice::update() {
	mLeapData->update();
	mAnalog->update();
	mImaging->update();
	mTracker->update();
	mGestures->update();
	return OSVR_RETURN_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void ControllerDevice::initDevice(OSVR_PluginRegContext pContext) {
	mLeapData = new LeapData(mController);

	mController.setPolicy(Controller::POLICY_BACKGROUND_FRAMES);
	//mController.setPolicy(Controller::POLICY_IMAGES);
	//mController.setPolicy(Controller::POLICY_OPTIMIZE_HMD);

	OSVR_DeviceInitOptions options = osvrDeviceCreateInitOptions(pContext);

	mAnalog = new Analog(mDeviceToken, options, *mLeapData);
	mImaging = new Imaging(mDeviceToken, options, *mLeapData);
	mTracker = new Tracker(mDeviceToken, options, *mLeapData);
	mGestures = new Gestures(mDeviceToken, options, *mLeapData);
	mConfigure = new Configure(mDeviceToken, options, *mLeapData);

	mConfigure->setBool(ConfigureKey::Policy_Images, true); //TEST
	mConfigure->setBool(ConfigureKey::Policy_OptimizeHmd, true); //TEST
	mConfigure->setBool(ConfigureKey::Gesture_Swipe, true); //TEST
	mConfigure->setBool(ConfigureKey::Gesture_Circle, true); //TEST
	mConfigure->setBool(ConfigureKey::Gesture_KeyTap, true); //TEST
	mConfigure->setBool(ConfigureKey::Gesture_ScreenTap, true); //TEST

	registerObjectForDeletion(pContext, mAnalog);
	registerObjectForDeletion(pContext, mImaging);
	registerObjectForDeletion(pContext, mTracker);
	registerObjectForDeletion(pContext, mGestures);
	registerObjectForDeletion(pContext, mConfigure);

	mDeviceToken.initAsync(pContext, "Controller", options);
	mDeviceToken.sendJsonDescriptor(com_osvr_LeapMotion_json);
	mDeviceToken.registerUpdateCallback(this);
}
