#include <osvr/PluginKit/PluginKit.h>
#include "HardwareDetection.h"

OSVR_PLUGIN(com_osvr_LeapMotion) {
	osvr::pluginkit::PluginContext context(ctx);
    auto hd = new LeapOsvr::HardwareDetection();

    context.registerDriverInstantiationCallback(LeapOsvr::kLeapDriverName, hd);
    context.registerHardwareDetectCallback(hd);

	return OSVR_RETURN_SUCCESS;
}
