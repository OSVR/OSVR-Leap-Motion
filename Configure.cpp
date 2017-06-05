#include <iostream>
#include "Configure.h"

using namespace LeapOsvr;


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

void Configure::setPolicy(eLeapPolicyFlag flag, bool pValue) {
    eLeapRS result;
    LEAP_CONNECTION connection = mLeapData.getConnection();
    if (pValue) {
        result = LeapSetPolicyFlags(connection, flag, 0);
        if (LEAP_FAILED(result)) {
            std::cerr << "com_osvr_LeapMotion - Configure::setPolicy(" << flag << ", " << pValue << "): LeapSetPolicyFlags call failed." << std::endl;
        }
    }
    else {
        result = LeapSetPolicyFlags(connection, 0, flag);
        if (LEAP_FAILED(result)) {
            std::cerr << "com_osvr_LeapMotion - Configure::setPolicy(" << flag << ", " << pValue << "): LeapSetPolicyFlags call failed." << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Configure::setBoolDirect(const std::string& pKey, bool pValue) {
    LEAP_CONNECTION connection = mLeapData.getConnection();
    LEAP_VARIANT value;
    value.type = eLeapValueType_Boolean;
    value.boolValue = pValue;

    eLeapRS result = LeapSaveConfigValue(connection, pKey.c_str(), &value, nullptr);
    if (LEAP_FAILED(result)) {
        std::cerr << "com_osvr_LeapMotion - Configure::setBoolDirect(" << pKey << ", " << pValue << "): LeapSaveConfigValue call failed." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Configure::setIntDirect(const std::string& pKey, int pValue) {
    LEAP_CONNECTION connection = mLeapData.getConnection();
    LEAP_VARIANT value;
    value.type = eLeapValueType_Int32;
    value.iValue = pValue;

    eLeapRS result = LeapSaveConfigValue(connection, pKey.c_str(), &value, nullptr);
    if (LEAP_FAILED(result)) {
        std::cerr << "com_osvr_LeapMotion - Configure::setIntDirect(" << pKey << ", " << pValue << "): LeapSaveConfigValue call failed." << std::endl;
    }
}
