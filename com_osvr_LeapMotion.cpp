#include <osvr/PluginKit/PluginKit.h>
#include "ControllerDevice.h"

OSVR_PLUGIN(com_osvr_LeapMotion) {
	osvr::pluginkit::PluginContext context(ctx);
	context.registerHardwareDetectCallback(new LeapOsvr::ControllerDevice());
	return OSVR_RETURN_SUCCESS;
}
