#include "Configure.h"

using namespace Leap;
using namespace LeapOsvr;
using namespace ConfigureKey;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Configure::Configure(const osvr::pluginkit::DeviceToken& pDeviceToken, 
					OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData) : 
					mDeviceToken(pDeviceToken), mLeapData(pLeapData)/*,mConfigInterface(NULL)*/ {
	//osvrDeviceConfigConfig(pOptions, &mConfigInterface);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
inline bool isMatch(const std::string& pKeyA, const std::string& pKeyB) {
	return (pKeyA.compare(pKeyB) == 0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Configure::setBool(const std::string& pKey, bool pValue) {
	const Controller& controller = mLeapData.getController();

	if ( isMatch(pKey, Policy_Images) ) {
		if ( pValue ) {
			controller.setPolicy(Controller::POLICY_IMAGES);
		}
		else {
			controller.clearPolicy(Controller::POLICY_IMAGES);
		}
		return;
	}

	if ( isMatch(pKey, Policy_OptimizeHmd) ) {
		if ( pValue ) {
			controller.setPolicy(Controller::POLICY_OPTIMIZE_HMD);
		}
		else {
			controller.clearPolicy(Controller::POLICY_OPTIMIZE_HMD);
		}
		return;
	}

	////

	if ( isMatch(pKey, Gesture_Swipe) ) {
		controller.enableGesture(Gesture::TYPE_SWIPE, pValue);
		return;
	}

	if ( isMatch(pKey, Gesture_Circle) ) {
		controller.enableGesture(Gesture::TYPE_CIRCLE, pValue);
		return;
	}

	if ( isMatch(pKey, Gesture_KeyTap) ) {
		controller.enableGesture(Gesture::TYPE_KEY_TAP, pValue);
		return;
	}

	if ( isMatch(pKey, Gesture_ScreenTap) ) {
		controller.enableGesture(Gesture::TYPE_SCREEN_TAP, pValue);
		return;
	}

	////

	std::cout << "[LEAP] Unsupported bool config key: " << pKey << std::endl;
}

/*----------------------------------------------------------------------------------------------------*/
void Configure::setFloat(const std::string& pKey, float pValue) {
	bool isLeapConfig = (
		isMatch(pKey, Gesture_Circle_MinRadius) ||
		isMatch(pKey, Gesture_Circle_MinArc) ||
		isMatch(pKey, Gesture_Swipe_MinLength) ||
		isMatch(pKey, Gesture_Swipe_MinVelocity) ||
		isMatch(pKey, Gesture_KeyTap_MinDownVelocity) ||
		isMatch(pKey, Gesture_KeyTap_HistorySeconds) ||
		isMatch(pKey, Gesture_KeyTap_MinDistance) ||
		isMatch(pKey, Gesture_ScreenTap_MinForwardVelocity) ||
		isMatch(pKey, Gesture_ScreenTap_HistorySeconds) ||
		isMatch(pKey, Gesture_ScreenTap_MinDistance)
	);

	if ( isLeapConfig ) {
		Config config = mLeapData.getController().config();
		config.setFloat(pKey, pValue);
		config.save();
		return;
	}

	////

	std::cout << "[LEAP] Unsupported float config key: " << pKey << std::endl;
}
