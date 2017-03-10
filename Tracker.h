#include <osvr/PluginKit/PluginKit.h>
#include <osvr/PluginKit/TrackerInterfaceC.h>
#include <osvr/PluginKit/SkeletonInterfaceC.h>
#include <osvr/Util/Pose3C.h>
#include "LeapData.h"

namespace LeapOsvr {

    /** @brief Handles tracker and skeleton interfaces */
    class Tracker {

    public:

        Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
            OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData);
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

        enum FingerType {
            Thumb,
            Index,
            Middle,
            Ring,
            Pinky,

            NumFingerTypes // must be the last element
        };

        enum JointType {
            MetaCarpal,
            Proximal,
            Intermediate,
            Distal,
            
            NumJointTypes // must be last element
        };

        const LeapData& mLeapData;
        const osvr::pluginkit::DeviceToken& mDeviceToken;
        OSVR_TrackerDeviceInterface mTrackerInterface;
        OSVR_SkeletonDeviceInterface mSkeletonInterface;

        Channel mChannelMap[5][4];

        void sendHand(const LEAP_HAND &pHand);
        void sendFinger(const LEAP_DIGIT &pFinger, FingerType pFingerType, bool pIsLeft);
        void sendBone(const LEAP_BONE &pBone, FingerType pFingerType, JointType pJointType, 
            bool pIsLeft);
        void sendPose(Channel pChannel, bool pIsLeft,
            const LEAP_VECTOR &pPosition, const LEAP_QUATERNION &pRotation);

        OSVR_Vec3 getOsvrVector(const LEAP_VECTOR &pVector);
        OSVR_Quaternion getOsvrQuaternion(const LEAP_QUATERNION &pRotation, bool pIsLeft);

    };

}