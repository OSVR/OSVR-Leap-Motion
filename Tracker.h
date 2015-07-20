#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/TrackerInterfaceC.h>
#include <osvr/Util/Pose3C.h>
#include "Leap.h"

namespace LeapOsvr {

	class Tracker {

		public:

			Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
				OSVR_DeviceInitOptions pOptions, const Leap::Controller& pController);
			void update();

		private:

			enum Channel {
				Elbow,
				Wrist,
				
				Palm,
				
				ThumbMeta,
				ThumbProx,
				ThumbInter,
				ThumbDist,

				IndexMeta,
				IndexProx,
				IndexInter,
				IndexDist,

				MiddleMeta,
				MiddleProx,
				MiddleInter,
				MiddleDist,

				RingMeta,
				RingProx,
				RingInter,
				RingDist,

				PinkyMeta,
				PinkyProx,
				PinkyInter,
				PinkyDist,

				ChannelsPerHand //must be the last element
			};

			const Leap::Controller& mController;
			const osvr::pluginkit::DeviceToken& mDeviceToken;
			OSVR_TrackerDeviceInterface mTrackerInterface;
			Channel mChannelMap[5][4];

			void sendHand(const Leap::Hand& pHand);
			void sendFinger(const Leap::Finger& pFinger, bool pIsLeft);
			void sendBone(const Leap::Bone& pBone, Leap::Finger::Type pFingerType, bool pIsLeft);
			void sendPose(Channel pChannel, bool pIsLeft, 
				const Leap::Vector& pPosition, const Leap::Matrix& pBasis);

			OSVR_Vec3 getOsvrVector(const Leap::Vector& pVector);
			OSVR_Quaternion getOsvrQuaternion(const Leap::Matrix& pBasis);

	};

}