#include "LeapData.h"
#include <iostream>

using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
LeapData::LeapData(const LEAP_CONNECTION pConnection) : mConnection(pConnection),
    mHandSelectL(HandSelector(true)),
    mHandSelectR(HandSelector(false)) {
    //do nothing...
}

LeapData::~LeapData() {
    if (mConnection) {
        LeapDestroyConnection(mConnection);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
bool LeapData::update() {

    eLeapRS result;
    LEAP_CONNECTION_MESSAGE msg;
    result = LeapPollConnection(mConnection, 1000, &msg);
    if (LEAP_FAILED(result)) {
        //std::cerr << "com_osvr_LeapMotion - LeapData::update(): LeapPollConnection call failed." << std::endl;
        return false;
    }

    switch (msg.type) {
    case eLeapEventType_Connection:
        // @todo
        break;
    case eLeapEventType_ConnectionLost:
        // @todo
        break;
    case eLeapEventType_Device:
        // @todo
        break;
    case eLeapEventType_DeviceLost:
        // @todo
        break;
    case eLeapEventType_DeviceFailure:
        // @todo
        break;
    case eLeapEventType_Tracking:
        copyFrame(*(msg.tracking_event));
        return true;
        break;
    case eLeapEventType_ImageComplete:
        // @todo
        break;
    case eLeapEventType_ImageRequestError:
        // @todo
        break;
    case eLeapEventType_LogEvent:
        // @todo
        break;
    case eLeapEventType_Policy:
        // @todo
        break;
    case eLeapEventType_ConfigChange:
        // @todo
        break;
    case eLeapEventType_ConfigResponse:
        // @todo
        break;
    default:
        // @todo log msg.type?
        break;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
const LEAP_CONNECTION LeapData::getConnection() const {
    return mConnection;
}

/*----------------------------------------------------------------------------------------------------*/
const LEAP_TRACKING_EVENT& LeapData::getFrame() const {
    return mFrame;
}

/*----------------------------------------------------------------------------------------------------*/
const bool LeapData::hasBestHand(HandSide pSide) const {
    return (getBestHandIndex(pSide) != NoHandFound);
}

/*----------------------------------------------------------------------------------------------------*/
const LEAP_HAND& LeapData::getBestHand(HandSide pSide) const {
    const int index = getBestHandIndex(pSide);

    if (index == NoHandFound) {
        throw std::out_of_range("No 'best hand' was found.");
    }

    return mFrame.pHands[index];
}


const bool LeapData::isConnected() const {
    LEAP_CONNECTION_INFO connectionInfo;
    eLeapRS result = LeapGetConnectionInfo(mConnection, &connectionInfo);
    return LEAP_SUCCEEDED(result) && connectionInfo.status == eLeapConnectionStatus_Connected;
}

const bool LeapData::isDeviceConnected() const {
    uint32_t numDevices = 0;
    eLeapRS result = LeapGetDeviceList(mConnection, nullptr, &numDevices);
    return LEAP_SUCCEEDED(result) && numDevices > 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
const int LeapData::getBestHandIndex(HandSide pSide) const {
    return (pSide == Left ? mHandSelectL : mHandSelectR).getBestHandIndex();
}

void LeapData::copyFrame(const LEAP_TRACKING_EVENT& frame) {
    mFrame = frame;
    mHands.resize(frame.nHands);
    for (uint32_t i = 0; i < frame.nHands; i++) {
        mHands[i] = frame.pHands[i];
    }
    mFrame.pHands = mHands.data();
    mHandSelectL.update(mHands);
    mHandSelectR.update(mHands);
}
