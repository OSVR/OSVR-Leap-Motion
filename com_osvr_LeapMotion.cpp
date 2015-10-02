#include <osvr/PluginKit/PluginKit.h>
#include "HardwareDetection.h"

OSVR_PLUGIN(com_osvr_LeapMotion) {
	osvr::pluginkit::PluginContext context(ctx);
	context.registerHardwareDetectCallback(new LeapOsvr::HardwareDetection());
	return OSVR_RETURN_SUCCESS;
}
