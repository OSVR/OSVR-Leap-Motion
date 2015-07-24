#include <osvr/PluginKit/PluginKit.h>
//#include <osvr/PluginKit/ConfigInterfaceC.h>
#include "Leap.h"

namespace LeapOsvr {

	class Configure {

		public:

			Configure(const osvr::pluginkit::DeviceToken& pDeviceToken, 
				OSVR_DeviceInitOptions pOptions, const Leap::Controller& pController);

			void setBool(const std::string& pKey, bool pValue);
			void setFloat(const std::string& pKey, float pValue);

		private:

			const Leap::Controller& mController;
			const osvr::pluginkit::DeviceToken& mDeviceToken;
			//OSVR_ConfigDeviceInterface mConfigInterface;

	};

	namespace ConfigureKey {

		static const std::string Policy_Images = "Policy.Images";
		static const std::string Policy_OptimizeHmd = "Policy.OptimizeHmd";

		static const std::string Gesture_Swipe = "Gesture.Swipe";
		static const std::string Gesture_Circle = "Gesture.Swipe";
		static const std::string Gesture_KeyTap = "Gesture.KeyTap";
		static const std::string Gesture_ScreenTap = "Gesture.ScreenTap";

		static const std::string Gesture_Circle_MinRadius = "Gesture.Circle.MinRadius";
		static const std::string Gesture_Circle_MinArc = "Gesture.Circle.MinArc";
		static const std::string Gesture_Swipe_MinLength = "Gesture.Swipe.MinLength";
		static const std::string Gesture_Swipe_MinVelocity = "Gesture.Swipe.MinVelocity";
		static const std::string Gesture_KeyTap_MinDownVelocity = "Gesture.KeyTap.MinDownVelocity";
		static const std::string Gesture_KeyTap_HistorySeconds = "Gesture.KeyTap.HistorySeconds";
		static const std::string Gesture_KeyTap_MinDistance = "Gesture.KeyTap.MinDistance";
		static const std::string Gesture_ScreenTap_MinForwardVelocity = "Gesture.ScreenTap.MinForwardVelocity";
		static const std::string Gesture_ScreenTap_HistorySeconds = "Gesture.ScreenTap.HistorySeconds";
		static const std::string Gesture_ScreenTap_MinDistance = "Gesture.ScreenTap.MinDistance";

	}

}