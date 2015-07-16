#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/ImagingInterfaceC.h>
#include <iostream>
#include "Leap.h"

namespace LeapOsvr {

	class Imaging {

		public:

			Imaging(const osvr::pluginkit::DeviceToken& pDeviceToken,
				OSVR_DeviceInitOptions pOptions, const Leap::Controller& pController);
			void update();

		private:

			const Leap::Controller& mController;
			const osvr::pluginkit::DeviceToken& mDeviceToken;
			OSVR_ImagingDeviceInterfaceObject* mImagingInterface;

			void sendCameraImage(const Leap::Image& pImage, const OSVR_TimeValue& pTime);
			void sendCalibrationImage(const Leap::Image& pImage, const OSVR_TimeValue& pTime);

	};

}