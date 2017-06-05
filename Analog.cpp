#include "Analog.h"

using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Analog::Analog(const osvr::pluginkit::DeviceToken& pDeviceToken,
    OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData) :
    mDeviceToken(pDeviceToken), mLeapData(pLeapData), mAnalogInterface(NULL), mNullHand() {
    osvrDeviceAnalogConfigure(pOptions, &mAnalogInterface, AnalogChannelCount);
}

/*----------------------------------------------------------------------------------------------------*/
void Analog::update() {
    const LEAP_CONNECTION connection = mLeapData.getConnection();

    bool hasL = mLeapData.hasBestHand(LeapData::HandSide::Left);
    bool hasR = mLeapData.hasBestHand(LeapData::HandSide::Right);

    const LEAP_HAND &handL = (hasL ? mLeapData.getBestHand(LeapData::HandSide::Left) : mNullHand);
    const LEAP_HAND &handR = (hasR ? mLeapData.getBestHand(LeapData::HandSide::Right) : mNullHand);

    mValues[IsDeviceConnected] = (mLeapData.isDeviceConnected() ? 1 : 0);
    mValues[IsServiceConnected] = (mLeapData.isConnected() ? 1 : 0);

    mValues[IsLeftHandAvailable] = hasL;
    mValues[IsRightHandAvailable] = hasR;

    mValues[LeftHandConfidence] = handL.confidence;
    mValues[RightHandConfidence] = handR.confidence;

    mValues[LeftHandGrabStrength] = handL.grab_strength;
    mValues[RightHandGrabStrength] = handR.grab_strength;

    mValues[LeftHandPinchStrength] = handL.pinch_strength;
    mValues[RightHandPinchStrength] = handR.pinch_strength;

    mValues[LeftHandPalmWidth] = handL.palm.width;
    mValues[RightHandPalmWidth] = handR.palm.width;

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
