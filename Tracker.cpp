#include "Tracker.h"
#include <Eigen/Geometry>

#include "com_osvr_LeapMotion_json.h"

using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Tracker::Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
    OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData) :
    mDeviceToken(pDeviceToken), mLeapData(pLeapData),
    mTrackerInterface(NULL) {

    // note we have two skeleton sensors, one for each hand, each with their own spec
    // this is because the articulation specs for each hand are not connected by a parent joint
    osvrDeviceSkeletonConfigure(pOptions, &mSkeletonInterface, com_osvr_LeapMotion_json, 2);

    osvrDeviceTrackerConfigure(pOptions, &mTrackerInterface);

    // The Leap SDK has a fake thumb intermediate joint so that each finger has the same number of
    // joints, but it's not a real joint.
    mChannelMap[Thumb][MetaCarpal] = ThumbMeta;
    mChannelMap[Thumb][Proximal] = ThumbProx;
    mChannelMap[Thumb][Intermediate] = ThumbInter; // NOT USED
    mChannelMap[Thumb][Distal] = ThumbDist;

    mChannelMap[Index][MetaCarpal] = IndexMeta;
    mChannelMap[Index][Proximal] = IndexProx;
    mChannelMap[Index][Intermediate] = IndexInter;
    mChannelMap[Index][Distal] = IndexDist;

    mChannelMap[Middle][MetaCarpal] = MiddleMeta;
    mChannelMap[Middle][Proximal] = MiddleProx;
    mChannelMap[Middle][Intermediate] = MiddleInter;
    mChannelMap[Middle][Distal] = MiddleDist;

    mChannelMap[Ring][MetaCarpal] = RingMeta;
    mChannelMap[Ring][Proximal] = RingProx;
    mChannelMap[Ring][Intermediate] = RingInter;
    mChannelMap[Ring][Distal] = RingDist;

    mChannelMap[Pinky][MetaCarpal] = PinkyMeta;
    mChannelMap[Pinky][Proximal] = PinkyProx;
    mChannelMap[Pinky][Intermediate] = PinkyInter;
    mChannelMap[Pinky][Distal] = PinkyDist;
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::update() {
    OSVR_TimeValue timestamp;
    osvrTimeValueGetNow(&timestamp);

    if (mLeapData.hasBestHand(LeapData::HandSide::Left)) {
        sendHand(mLeapData.getBestHand(LeapData::HandSide::Left));
        osvrDeviceSkeletonComplete(mSkeletonInterface, 0, &timestamp);
    }

    if (mLeapData.hasBestHand(LeapData::HandSide::Right)) {
        sendHand(mLeapData.getBestHand(LeapData::HandSide::Right));
        osvrDeviceSkeletonComplete(mSkeletonInterface, 1, &timestamp);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendHand(const LEAP_HAND &pHand) {
    bool isLeft = pHand.type == eLeapHandType_Left;
    const LEAP_BONE &arm = pHand.arm;

    sendPose(Elbow, isLeft, arm.prev_joint, arm.rotation);
    sendPose(Wrist, isLeft, arm.next_joint, arm.rotation);
    sendPose(Palm, isLeft, pHand.palm.position, pHand.palm.orientation);

    sendFinger(pHand.thumb, Thumb, isLeft);
    sendFinger(pHand.index, Index, isLeft);
    sendFinger(pHand.middle, Middle, isLeft);
    sendFinger(pHand.ring, Ring, isLeft);
    sendFinger(pHand.pinky, Pinky, isLeft);
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendFinger(const LEAP_DIGIT &pFinger, FingerType pFingerType, bool pIsLeft) {
    sendBone(pFinger.metacarpal, pFingerType, MetaCarpal, pIsLeft);
    sendBone(pFinger.proximal, pFingerType, Proximal, pIsLeft);
    sendBone(pFinger.intermediate, pFingerType, Intermediate, pIsLeft);
    sendBone(pFinger.distal, pFingerType, Distal, pIsLeft);
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendBone(const LEAP_BONE &pBone, FingerType pFingerType, JointType pJointType, bool pIsLeft) {
    Channel channel = mChannelMap[pFingerType][pJointType];
    sendPose(channel, pIsLeft, pBone.prev_joint, pBone.rotation);
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendPose(Channel pChannel, bool pIsLeft,
    const LEAP_VECTOR &pPosition, const LEAP_QUATERNION &pRotation) {
    OSVR_PoseState pose;
    pose.translation = getOsvrVector(pPosition);
    pose.rotation = getOsvrQuaternion(pRotation, pIsLeft);

    OSVR_ChannelCount channel = pChannel + (pIsLeft ? 0 : ChannelsPerHand);

    osvrDeviceTrackerSendPose(mDeviceToken, mTrackerInterface, &pose, channel);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
OSVR_Vec3 Tracker::getOsvrVector(const LEAP_VECTOR &pVector) {
    const float scale = 0.001f;

    OSVR_Vec3 vec;
    vec.data[0] = pVector.x*scale;
    vec.data[1] = pVector.y*scale;
    vec.data[2] = pVector.z*scale;
    return vec;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_Quaternion Tracker::getOsvrQuaternion(const LEAP_QUATERNION &pRotation, bool pIsLeft) {
    //int zMult = (pIsLeft ? 1 : 1);
    //auto xBasis = pBasis.xBasis.normalized();
    //auto yBasis = pBasis.yBasis.normalized();
    //auto zBasis = pBasis.zBasis.normalized();
    //Eigen::Matrix3f mat = Eigen::Matrix3f();
    //mat << //uses column-major order
    //    pBasis.xBasis.x, pBasis.yBasis.x, pBasis.zBasis.x*zMult,
    //    pBasis.xBasis.y, pBasis.yBasis.y, pBasis.zBasis.y*zMult,
    //    pBasis.xBasis.z, pBasis.yBasis.z, pBasis.zBasis.z*zMult;

    //Eigen::Quaternionf eigenQuat = Eigen::Quaternionf(mat);
    //eigenQuat.normalize();

    //OSVR_Quaternion quat;
    //osvrQuatSetW(&quat, eigenQuat.w());
    //osvrQuatSetX(&quat, eigenQuat.x());
    //osvrQuatSetY(&quat, eigenQuat.y());
    //osvrQuatSetZ(&quat, eigenQuat.z());
    //return quat;

    OSVR_Quaternion quat;
    osvrQuatSetW(&quat, pRotation.w);
    osvrQuatSetX(&quat, pRotation.x);
    osvrQuatSetY(&quat, pRotation.y);
    osvrQuatSetZ(&quat, pRotation.z);
    return quat;
}
