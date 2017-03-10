#include <osvr/PluginKit/PluginKit.h>
#include "LeapC.h"

namespace LeapOsvr {

    class HardwareDetection {

    public:

        HardwareDetection();
        ~HardwareDetection();
        OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);

    private:
        LEAP_CONNECTION mConnection = nullptr;
        bool mFound = false;
        bool mDeviceAdded = false;
    };

}
