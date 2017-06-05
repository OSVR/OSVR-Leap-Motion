#include <osvr/PluginKit/PluginKit.h>
//#include <osvr/PluginKit/ConfigInterfaceC.h>
#include "LeapData.h"

namespace LeapOsvr {

    class Configure {

    public:

        Configure(const osvr::pluginkit::DeviceToken& pDeviceToken,
            OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData);

        void setPolicy(eLeapPolicyFlag flag, bool pValue);
        void setBoolDirect(const std::string& pKey, bool pValue);
        void setIntDirect(const std::string& pKey, int pValue);

    private:

        const LeapData& mLeapData;
        const osvr::pluginkit::DeviceToken& mDeviceToken;
        //OSVR_ConfigDeviceInterface mConfigInterface;

    };
}