#include <osvr/PluginKit/PluginKit.h>
//#include <osvr/PluginKit/GestureInterfaceC.h>
#include "Leap.h"

namespace LeapOsvr {

	class Gestures {

		public:

			Gestures(const osvr::pluginkit::DeviceToken& pDeviceToken,
				OSVR_DeviceInitOptions pOptions, const Leap::Controller& pController);
			void update();

		private:

			const Leap::Controller& mController;
			const osvr::pluginkit::DeviceToken& mDeviceToken;
			//OSVR_GestureDeviceInterface mGestureInterface;

			void sendGesture(const Leap::Gesture& pGesture);

	};

	namespace GestureKey {

		static const std::string Swipe = "Swipe";
		static const std::string Circle = "Swipe";
		static const std::string KeyTap = "KeyTap";
		static const std::string ScreenTap = "ScreenTap";

	}

}