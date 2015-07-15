#include <osvr/PluginKit/PluginKit.h>
#include <iostream>
#include "Leap.h"
#include "Analog.h"
#include "Imaging.h"
#include "Tracker.h"

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
			osvr::pluginkit::DeviceToken mDeviceToken;

			void initDevice(OSVR_PluginRegContext pContext);

	};

}
