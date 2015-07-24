#include <osvr/PluginKit/PluginKit.h>
#include "Leap.h"
#include "Analog.h"
#include "Imaging.h"
#include "Tracker.h"
#include "Configure.h"

namespace LeapOsvr {
	
	class ControllerDevice {

		public:

			ControllerDevice();
			OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);
			OSVR_ReturnCode update();

		private:

			Leap::Controller mController;
			Analog* mAnalog;
			Imaging* mImaging;
			Tracker* mTracker;
			Configure* mConfigure;
			osvr::pluginkit::DeviceToken mDeviceToken;

			void initDevice(OSVR_PluginRegContext pContext);

	};

}
