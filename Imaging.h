#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/ImagingInterface.h>
#include "LeapData.h"

namespace LeapOsvr {

	class Imaging {

		public:

			Imaging(osvr::pluginkit::DeviceToken& pDeviceToken,
				OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData);
			~Imaging();
			void update();

		private:

			const LeapData& mLeapData;
			osvr::pluginkit::DeviceToken& mDeviceToken;
			OSVR_ImagingDeviceInterfaceObject* mImagingDeviceInterface;
			osvr::pluginkit::ImagingInterface mImagingInterface;
			cv::Mat* mDistortionCache[2];
			OSVR_TimeValue_Seconds mNextDistortionSecond;

			void sendCameraImage(const Leap::Image& pImage);
			void sendDistortionImage(const Leap::Image& pImage);
			void cacheDistortionImage(const Leap::Image& pImage);

	};

}
