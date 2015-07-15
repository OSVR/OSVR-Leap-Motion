#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/TrackerInterfaceC.h>
#include <osvr/Util/Pose3C.h>
#include <iostream>
#include "Leap.h"

namespace LeapOsvr {

	class Tracker {

		public:

			Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
				OSVR_DeviceInitOptions pOptions, const Leap::Controller& pController);
			void update();

		private:

			enum Channel {
				Palm,
				ThumbTip,
				IndexTip,
				MiddleTip,
				RingTip,
				PinkyTip,
				ChannelsPerHand //must be the last element
			};

			const Leap::Controller& mController;
			const osvr::pluginkit::DeviceToken& mDeviceToken;
			OSVR_TrackerDeviceInterface mTrackerInterface;

			void sendHand(Leap::Hand pHand);
			void sendFinger(Leap::Finger pFinger, bool pIsLeft);
			void sendPose(Channel pChannel, bool pIsLeft, Leap::Vector pPosition, Leap::Matrix pBasis);

			OSVR_Vec3 getOsvrVector(Leap::Vector pVector);
			OSVR_Quaternion getOsvrQuaternion(Leap::Matrix pBasis);

	};

}