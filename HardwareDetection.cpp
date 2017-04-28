#include "HardwareDetection.h"
#include "ControllerDevice.h"
#include <iostream>

#include <json/reader.h>
#include <json/value.h>

using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
HardwareDetection::HardwareDetection() {
    //do nothing...
}

HardwareDetection::~HardwareDetection() {
    if (mConnection) {
        LeapDestroyConnection(mConnection);
    }
}

OSVR_ReturnCode HardwareDetection::operator()(OSVR_PluginRegContext pContext) {
    return (*this)(pContext, nullptr);
}
/*----------------------------------------------------------------------------------------------------*/
OSVR_ReturnCode HardwareDetection::operator()(OSVR_PluginRegContext pContext, const char *params) {
    eLeapRS result;
    
    if(!mConnection) {
        result = LeapCreateConnection(nullptr, &mConnection);
        if (LEAP_FAILED(result)) {
            std::cerr << "com_osvr_LeapMotion - HardwareDetection::operator(): LeapCreateConnection call failed." << std::endl;
            return OSVR_RETURN_FAILURE;
        }

        result = LeapOpenConnection(mConnection);
        if (LEAP_FAILED(result)) {
            std::cerr << "com_osvr_LeapMotion - HardwareDetection::operator(): LeapOpenConnection call failed." << std::endl;
            LeapDestroyConnection(mConnection);
            mConnection = nullptr;
            return OSVR_RETURN_FAILURE;
        }
    }
    
    if (!mFound) {
        if (params) {
            Json::Value root;
            Json::Reader reader;
            if (!reader.parse(params, root)) {
                std::cerr << "Couldn't parse JSON for " << kLeapDriverName << std::endl;
                return OSVR_RETURN_FAILURE;
            }

            if (root.isMember("hmdMode")) {
                mConfig.hmdMode = root.get("hmdMode", mConfig.hmdMode).asBool();
            }
        }

        for (int i = 0; i < 5; i++) {
            LEAP_CONNECTION_MESSAGE msg;
            result = LeapPollConnection(mConnection, 1000, &msg);
            if (LEAP_FAILED(result)) {
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): LeapPollConnection call failed." << std::endl;
                return OSVR_RETURN_FAILURE;
            }

            switch (msg.type) {
            case eLeapEventType_Connection:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_Connection event." << std::endl;
                // @todo
                break;
            case eLeapEventType_ConnectionLost:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_ConnectionLost event." << std::endl;
                // @todo
                break;
            case eLeapEventType_Device:
            {
                std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_Device event." << std::endl;
                const LEAP_DEVICE_EVENT* deviceEvent = msg.device_event;
                if (deviceEvent->status & eLeapDeviceStatus_Streaming) {
                    mFound = true;
                    osvr::pluginkit::registerObjectForDeletion(pContext, new ControllerDevice(pContext, mConnection, mConfig));
                    mConnection = nullptr; // pass ownership to the controller device
                }
            }
            break;
            case eLeapEventType_DeviceLost:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_DeviceLost event." << std::endl;
                break;
            case eLeapEventType_DeviceFailure:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_DeviceFailure event." << std::endl;
                break;
            case eLeapEventType_Tracking:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_Tracking event." << std::endl;
                break;
            case eLeapEventType_ImageComplete:
                // @todo
                break;
            case eLeapEventType_ImageRequestError:
                // @todo
                break;
            case eLeapEventType_LogEvent:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_LogEvent event." << std::endl;
                break;
            case eLeapEventType_Policy:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_Policy event." << std::endl;
                break;
            case eLeapEventType_ConfigChange:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_ConfigChange event." << std::endl;
                break;
            case eLeapEventType_ConfigResponse:
                //std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got eLeapEventType_ConfigResponse event." << std::endl;
                break;
            default:
                std::cout << "com_osvr_LeapMotion - HardwareDetection::operator(): got unknown leap event type." << std::endl;
                break;
            }
        }
    }

    return OSVR_RETURN_SUCCESS;
}
