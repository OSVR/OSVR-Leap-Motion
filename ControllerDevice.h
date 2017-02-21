#include <osvr/PluginKit/PluginKit.h>
#include "LeapData.h"
#include "Analog.h"
#include "Imaging.h"
#include "Tracker.h"
#include "Gestures.h"
#include "Configure.h"

namespace LeapOsvr {
	
	class ControllerDevice {

		public:

			ControllerDevice(OSVR_PluginRegContext pContext);
			~ControllerDevice();
			OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);
			OSVR_ReturnCode update();

		private:

			const Leap::Controller mController;
			LeapData* mLeapData;
			Analog* mAnalog;
			//Imaging* mImaging;
			Tracker* mTracker;
			Gestures* mGestures;
			Configure* mConfigure;
			osvr::pluginkit::DeviceToken mDeviceToken;

			void initDevice(OSVR_PluginRegContext pContext);

	};

}
