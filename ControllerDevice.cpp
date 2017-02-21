#include "ControllerDevice.h"
#include "com_osvr_LeapMotion_json.h"

using namespace Leap;
using namespace LeapOsvr;
using namespace osvr::pluginkit;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
ControllerDevice::ControllerDevice(OSVR_PluginRegContext pContext) : mLeapData(NULL), mAnalog(NULL),
									/*mImaging(NULL), */mTracker(NULL), mGestures(NULL), mConfigure(NULL) {
	mController.setPolicy(Controller::POLICY_BACKGROUND_FRAMES);
	//mController.setPolicy(Controller::POLICY_IMAGES);
	//mController.setPolicy(Controller::POLICY_OPTIMIZE_HMD);

	OSVR_DeviceInitOptions options = osvrDeviceCreateInitOptions(pContext);

	mLeapData = new LeapData(mController);
	mAnalog = new Analog(mDeviceToken, options, *mLeapData);
	//mImaging = new Imaging(mDeviceToken, options, *mLeapData);
	mTracker = new Tracker(mDeviceToken, options, *mLeapData);
	mGestures = new Gestures(mDeviceToken, options, *mLeapData);
	mConfigure = new Configure(mDeviceToken, options, *mLeapData);

	mConfigure->setBool(ConfigureKey::Policy_Images, true);
	mConfigure->setBool(ConfigureKey::Policy_OptimizeHmd, true);
	mConfigure->setBool(ConfigureKey::Gesture_Swipe, true);
	mConfigure->setBool(ConfigureKey::Gesture_Circle, true);
	mConfigure->setBool(ConfigureKey::Gesture_KeyTap, true);
	mConfigure->setBool(ConfigureKey::Gesture_ScreenTap, true);

	mConfigure->setIntDirect("head_mounted_display_mode", 1);
	mConfigure->setIntDirect("tracking_hand_enabled", 1);
	mConfigure->setIntDirect("tracking_tool_enabled", 1);
	mConfigure->setIntDirect("images_mode", 2);
	mConfigure->setIntDirect("robust_mode_enabled", 0);
	mConfigure->setIntDirect("image_processing_auto_flip", 1);
	mConfigure->setIntDirect("low_resource_mode_enabled", 0);
	mConfigure->setIntDirect("avoid_poor_performance", 0);
	mConfigure->setIntDirect("klaatu_barada_nikto", 1);

	mDeviceToken.initAsync(pContext, "Controller", options);
	mDeviceToken.sendJsonDescriptor(com_osvr_LeapMotion_json);
	mDeviceToken.registerUpdateCallback(this);
}

/*----------------------------------------------------------------------------------------------------*/
ControllerDevice::~ControllerDevice() {
	delete mLeapData;
	delete mAnalog;
	//delete mImaging;
	delete mTracker;
	delete mGestures;
	delete mConfigure;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode ControllerDevice::update() {
	mLeapData->update();
	mAnalog->update();
	//mImaging->update();
	mTracker->update();
	mGestures->update();
	return OSVR_RETURN_SUCCESS;
}
