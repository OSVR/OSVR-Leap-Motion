#include "ControllerDevice.h"
#include "com_osvr_LeapMotion_json.h"
#include "LeapC.h"

using namespace LeapOsvr;
using namespace osvr::pluginkit;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
ControllerDevice::ControllerDevice(OSVR_PluginRegContext pContext, LEAP_CONNECTION connection, LeapConfig config) :
    mConnection(connection), mLeapData(NULL), mAnalog(NULL),
    /*mImaging(NULL), */mTracker(NULL), /*mGestures(NULL),*/ mConfigure(NULL), mConfigOptions(config) {

    //mController.setPolicy(Controller::POLICY_BACKGROUND_FRAMES);
    //mController.setPolicy(Controller::POLICY_IMAGES);
    //mController.setPolicy(Controller::POLICY_OPTIMIZE_HMD);

    OSVR_DeviceInitOptions options = osvrDeviceCreateInitOptions(pContext);

    mLeapData = new LeapData(mConnection);
    mAnalog = new Analog(mDeviceToken, options, *mLeapData);
    //mImaging = new Imaging(mDeviceToken, options, *mLeapData);
    mTracker = new Tracker(mDeviceToken, options, *mLeapData);
    //mGestures = new Gestures(mDeviceToken, options, *mLeapData);
    mConfigure = new Configure(mDeviceToken, options, *mLeapData);

    mConfigure->setPolicy(eLeapPolicyFlag_BackgroundFrames, true);
    mConfigure->setPolicy(eLeapPolicyFlag_OptimizeHMD, mConfigOptions.hmdMode);

    //mConfigure->setBool(ConfigureKey::Policy_Images, true);
    //mConfigure->setBool(ConfigureKey::Policy_OptimizeHmd, false); // @todo: make this a configurable setting
    //mConfigure->setBool(ConfigureKey::Gesture_Swipe, true);
    //mConfigure->setBool(ConfigureKey::Gesture_Circle, true);
    //mConfigure->setBool(ConfigureKey::Gesture_KeyTap, true);
    //mConfigure->setBool(ConfigureKey::Gesture_ScreenTap, true);

    // just let the user set these in their control panel?
    //mConfigure->setBoolDirect("image_processing_auto_flip", false);
    //mConfigure->setBoolDirect("robust_mode_enabled", false);
    //mConfigure->setBoolDirect("head_mounted_display_mode", false);
    //mConfigure->setBoolDirect("avoid_poor_performance", false);
    //mConfigure->setIntDirect("low_resource_mode_enabled", 0);

    mConfigure->setBoolDirect("tracking_hand_enabled", true);
    mConfigure->setBoolDirect("tracking_tool_enabled", true);

    mConfigure->setIntDirect("images_mode", 2);
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
    //delete mGestures;
    delete mConfigure;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode ControllerDevice::update() {
    if (mLeapData->update()) {
        mAnalog->update();
        //mImaging->update();
        mTracker->update();
        //mGestures->update();
    }
    return OSVR_RETURN_SUCCESS;
}
