#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/ImagingInterface.h>
#include <iostream>
#include <string>
#include "Leap.h"

namespace LeapOsvr {

	class Imaging {

		public:

			Imaging(osvr::pluginkit::DeviceToken& pDeviceToken,
				OSVR_DeviceInitOptions pOptions, const Leap::Controller& pController);
			~Imaging();
			void update();

		private:

			const Leap::Controller& mController;
			osvr::pluginkit::DeviceToken& mDeviceToken;
			OSVR_ImagingDeviceInterfaceObject* mImagingDeviceInterface;
			osvr::pluginkit::ImagingInterface mImagingInterface;
			cv::Mat* mDistortionCache[2];

			void sendCameraImage(const Leap::Image& pImage);
			void sendCalibrationImage(const Leap::Image& pImage);
			void cacheCalibrationImage(const Leap::Image& pImage);

	};

}
