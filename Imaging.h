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

			void sendCameraImage(Leap::Image* pImage, OSVR_TimeValue* pTime);

	};

}