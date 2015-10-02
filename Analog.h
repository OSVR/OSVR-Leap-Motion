#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/AnalogInterfaceC.h>
#include "LeapData.h"

namespace LeapOsvr {

	class Analog {

		public:

			Analog(const osvr::pluginkit::DeviceToken& pDeviceToken, 
				OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData);
			void update();

		private:

			enum Channel {
				IsDeviceConnected,
				IsServiceConnected,
				IsLeftHandAvailable,
				IsRightHandAvailable,
				LeftHandConfidence,
				RightHandConfidence,
				LeftHandGrabStrength,
				RightHandGrabStrength,
				LeftHandPinchStrength,
				RightHandPinchStrength,
				LeftHandPalmWidth,
				RightHandPalmWidth,

				AnalogChannelCount //must be the last element
			};

			const LeapData& mLeapData;
			const osvr::pluginkit::DeviceToken& mDeviceToken;
			OSVR_AnalogDeviceInterface mAnalogInterface;
			OSVR_AnalogState mValues[AnalogChannelCount];
			//double mTest;

	};

}