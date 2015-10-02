#include "Analog.h"

using namespace Leap;
using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Analog::Analog(const osvr::pluginkit::DeviceToken& pDeviceToken, 
							OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData) : 
							mDeviceToken(pDeviceToken), mLeapData(pLeapData), mAnalogInterface(NULL) {
	osvrDeviceAnalogConfigure(pOptions, &mAnalogInterface, AnalogChannelCount);
	//mTest = 0;
}

/*----------------------------------------------------------------------------------------------------*/
void Analog::update() {
	const Controller& controller = mLeapData.getController();

	bool hasL = mLeapData.hasBestHand(LeapData::HandSide::Left);
	bool hasR = mLeapData.hasBestHand(LeapData::HandSide::Right);

	const Hand* handL = (hasL ? &mLeapData.getBestHand(LeapData::HandSide::Left) : NULL);
	const Hand* handR = (hasR ? &mLeapData.getBestHand(LeapData::HandSide::Right) : NULL);

	mValues[IsDeviceConnected] = (controller.isConnected() ? 1 : 0);
	mValues[IsServiceConnected] = (controller.isServiceConnected() ? 1 : 0);

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

	osvrDeviceAnalogSetValues(mDeviceToken, mAnalogInterface, mValues, AnalogChannelCount);

	/*for ( int i = 0 ; i < AnalogChannelCount ; i++ ) {
		osvrDeviceAnalogSetValue(mDeviceToken, mAnalogInterface, mValues[i]+mTest, i);
	}

	std::cout <<  "test " << mValues[0] << " / " << mTest << std::endl;
	mTest += 0.01;*/
}
