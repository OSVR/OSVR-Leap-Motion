#include <osvr/PluginKit/PluginKit.h>
#include "LeapC.h"
#include "LeapData.h"
#include "Analog.h"
//#include "Imaging.h"
#include "Tracker.h"
//#include "Gestures.h"
#include "Configure.h"

namespace LeapOsvr {
	
	class ControllerDevice {

		public:

            ControllerDevice(OSVR_PluginRegContext pContext, LEAP_CONNECTION connection, LeapConfig config);
			~ControllerDevice();
			OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);
			OSVR_ReturnCode update();

		private:
            LeapConfig mConfigOptions;
			const LEAP_CONNECTION mConnection;
			LeapData* mLeapData;
			Analog* mAnalog;
			//Imaging* mImaging; // not currently working, also needs migration to C API
			Tracker* mTracker;
			//Gestures* mGestures; // deprecated in 3.0.0 SDK
			Configure* mConfigure;
			osvr::pluginkit::DeviceToken mDeviceToken;

			void initDevice(OSVR_PluginRegContext pContext);

	};

}
