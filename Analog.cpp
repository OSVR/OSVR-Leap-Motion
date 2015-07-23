#include "Analog.h"
//#include <iostream>

using namespace Leap;
using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Analog::Analog(const osvr::pluginkit::DeviceToken& pDeviceToken, 
							OSVR_DeviceInitOptions pOptions, const Controller& pController) : 
							mDeviceToken(pDeviceToken), mController(pController), 
							mAnalogInterface(NULL), mValues(new OSVR_AnalogState[AnalogChannelCount]) {
	osvrDeviceAnalogConfigure(pOptions, &mAnalogInterface, AnalogChannelCount);
}

/*----------------------------------------------------------------------------------------------------*/
void Analog::update() {
	Frame frame = mController.frame(0);
	HandList hands = frame.hands();
	int handCount = hands.count();
	Hand* handL = NULL;
	Hand* handR = NULL;

	for ( int i = 0 ; i < handCount ; ++i ) {
		Hand hand = hands[i];

		if ( !hand.isValid() ) {
			continue;
		}

		if ( hand.isLeft() ) {
			handL = &hand;
		}
		else {
			handR = &hand;
		}
	}

	bool hasL = (handL != NULL);
	bool hasR = (handR != NULL);

	mValues[IsDeviceConnected] = (mController.isConnected() ? 1 : 0);
	mValues[IsServiceConnected] = (mController.isServiceConnected() ? 1 : 0);

	mValues[IsLeftHandAvailable] = hasL;
	mValues[IsRightHandAvailable] = hasR;

	mValues[LeftHandConfidence] = (hasL ? handL->confidence() : 0);
	mValues[RightHandConfidence] = (hasR ? handR->confidence() : 0);

	mValues[LeftHandGrabStrength] = (hasL ? handL->grabStrength() : 0);
	mValues[RightHandGrabStrength] = (hasR ? handR->grabStrength() : 0);

	mValues[LeftHandPinchStrength] = (hasL ? handL->pinchStrength() : 0);
	mValues[RightHandPinchStrength] = (hasR ? handR->pinchStrength() : 0);

	mValues[LeftHandPalmWidth] = (hasL ? handL->palmWidth() : 0);
	mValues[RightHandPalmWidth] = (hasR ? handR->palmWidth() : 0);

	/*std::cout << "ANALOG" <<
		" / " << mValues[IsDeviceConnected] <<
		" , " << mValues[IsServiceConnected] <<
		" / " << mValues[IsLeftHandAvailable] <<
		" , " << mValues[IsRightHandAvailable] <<
		" / " << mValues[LeftHandConfidence] <<
		" , " << mValues[RightHandConfidence] <<
		" / " << mValues[LeftHandGrabStrength] <<
		" , " << mValues[RightHandGrabStrength] <<
		" / " << mValues[LeftHandPinchStrength] <<
		" , " << mValues[RightHandPinchStrength] <<
		" / " << mValues[LeftHandPalmWidth] <<
		" , " << mValues[RightHandPalmWidth] <<
		std::endl;*/

	osvrDeviceAnalogSetValues(mDeviceToken, mAnalogInterface, mValues, 0);
}
