#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/AnalogInterfaceC.h>
#include "Leap.h"

namespace LeapOsvr {

	class Analog {

		public:

			Analog(const osvr::pluginkit::DeviceToken& pDeviceToken, 
				OSVR_DeviceInitOptions pOptions, const Leap::Controller& pController);
			void update();

		private:

			enum Channel {
				HandCount,
				FingerCount,
				ToolCount
			};

			const Leap::Controller& mController;
			const osvr::pluginkit::DeviceToken& mDeviceToken;
			OSVR_AnalogDeviceInterface mAnalogInterface;
			OSVR_AnalogState* mValues;

	};

}