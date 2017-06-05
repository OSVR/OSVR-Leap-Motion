#include <osvr/PluginKit/PluginKit.h>
#include "LeapC.h"
#include "LeapData.h"

namespace LeapOsvr {
    static const char* kLeapDriverName = "LeapMotion";

    class HardwareDetection {
    public:

        HardwareDetection();
        ~HardwareDetection();
        OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext, const char *params);
        OSVR_ReturnCode HardwareDetection::operator()(OSVR_PluginRegContext pContext);

    private:
        LeapOsvr::LeapConfig mConfig;
        LEAP_CONNECTION mConnection = nullptr;
        bool mFound = false;
        bool mDeviceAdded = false;
    };

}
