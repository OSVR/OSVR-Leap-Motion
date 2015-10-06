#include "Analog.h"

using namespace Leap;
using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Analog::Analog(const osvr::pluginkit::DeviceToken& pDeviceToken, 
							OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData) : 
							mDeviceToken(pDeviceToken), mLeapData(pLeapData), mAnalogInterface(NULL) {
	osvrDeviceAnalogConfigure(pOptions, &mAnalogInterface, AnalogChannelCount);
}

/*----------------------------------------------------------------------------------------------------*/
void Analog::update() {
	const Controller& controller = mLeapData.getController();

	bool hasL = mLeapData.hasBestHand(LeapData::HandSide::Left);
	bool hasR = mLeapData.hasBestHand(LeapData::HandSide::Right);

	const Hand handL = (hasL ? mLeapData.getBestHand(LeapData::HandSide::Left) : mNullHand);
	const Hand handR = (hasR ? mLeapData.getBestHand(LeapData::HandSide::Right) : mNullHand);

	mValues[IsDeviceConnected] = (controller.isConnected() ? 1 : 0);
	mValues[IsServiceConnected] = (controller.isServiceConnected() ? 1 : 0);

	mValues[IsLeftHandAvailable] = hasL;
	mValues[IsRightHandAvailable] = hasR;

	mValues[LeftHandConfidence] = handL.confidence();
	mValues[RightHandConfidence] = handR.confidence();

	mValues[LeftHandGrabStrength] = handL.grabStrength();
	mValues[RightHandGrabStrength] = handR.grabStrength();

	mValues[LeftHandPinchStrength] = handL.pinchStrength();
	mValues[RightHandPinchStrength] = handR.pinchStrength();

	mValues[LeftHandPalmWidth] = handL.palmWidth();
	mValues[RightHandPalmWidth] = handR.palmWidth();

	/*std::cout << "ANALOG" <<
		" / " << mValues[IsDeviceConnected] <<
		" , " << mValues[IsServiceConnected] <<
		" / " << mValues[IsLeftHandAvailable] <<
		" , " << mValues[IsRightHandAvailable] <<
		" / " << handL.id() <<
		" , " << handR.id() <<
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
		osvrDeviceAnalogSetValue(mDeviceToken, mAnalogInterface, mValues[i], i);
	}*/
}
