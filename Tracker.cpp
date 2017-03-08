#include "Tracker.h"
#include <Eigen/Geometry>

#include "com_osvr_LeapMotion_json.h"

using namespace Leap;
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
	mChannelMap[Finger::Type::TYPE_THUMB][Bone::TYPE_METACARPAL] = ThumbMeta;
	mChannelMap[Finger::Type::TYPE_THUMB][Bone::TYPE_PROXIMAL] = ThumbProx;
	mChannelMap[Finger::Type::TYPE_THUMB][Bone::TYPE_INTERMEDIATE] = ThumbInter; // NOT USED
	mChannelMap[Finger::Type::TYPE_THUMB][Bone::TYPE_DISTAL] = ThumbDist;

	mChannelMap[Finger::Type::TYPE_INDEX][Bone::TYPE_METACARPAL] = IndexMeta;
	mChannelMap[Finger::Type::TYPE_INDEX][Bone::TYPE_PROXIMAL] = IndexProx;
	mChannelMap[Finger::Type::TYPE_INDEX][Bone::TYPE_INTERMEDIATE] = IndexInter;
	mChannelMap[Finger::Type::TYPE_INDEX][Bone::TYPE_DISTAL] = IndexDist;

	mChannelMap[Finger::Type::TYPE_MIDDLE][Bone::TYPE_METACARPAL] = MiddleMeta;
	mChannelMap[Finger::Type::TYPE_MIDDLE][Bone::TYPE_PROXIMAL] = MiddleProx;
	mChannelMap[Finger::Type::TYPE_MIDDLE][Bone::TYPE_INTERMEDIATE] = MiddleInter;
	mChannelMap[Finger::Type::TYPE_MIDDLE][Bone::TYPE_DISTAL] = MiddleDist;

	mChannelMap[Finger::Type::TYPE_RING][Bone::TYPE_METACARPAL] = RingMeta;
	mChannelMap[Finger::Type::TYPE_RING][Bone::TYPE_PROXIMAL] = RingProx;
	mChannelMap[Finger::Type::TYPE_RING][Bone::TYPE_INTERMEDIATE] = RingInter;
	mChannelMap[Finger::Type::TYPE_RING][Bone::TYPE_DISTAL] = RingDist;

	mChannelMap[Finger::Type::TYPE_PINKY][Bone::TYPE_METACARPAL] = PinkyMeta;
	mChannelMap[Finger::Type::TYPE_PINKY][Bone::TYPE_PROXIMAL] = PinkyProx;
	mChannelMap[Finger::Type::TYPE_PINKY][Bone::TYPE_INTERMEDIATE] = PinkyInter;
	mChannelMap[Finger::Type::TYPE_PINKY][Bone::TYPE_DISTAL] = PinkyDist;
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::update() {
    OSVR_TimeValue timestamp;
    osvrTimeValueGetNow(&timestamp);

	if ( mLeapData.hasBestHand(LeapData::HandSide::Left) ) {
		sendHand(mLeapData.getBestHand(LeapData::HandSide::Left));
        osvrDeviceSkeletonComplete(mSkeletonInterface, 0, &timestamp);
	}

	if ( mLeapData.hasBestHand(LeapData::HandSide::Right) ) {
		sendHand(mLeapData.getBestHand(LeapData::HandSide::Right));
        osvrDeviceSkeletonComplete(mSkeletonInterface, 1, &timestamp);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendHand(const Hand& pHand) {
	bool isLeft = pHand.isLeft();
	Arm arm = pHand.arm();
	FingerList fingers = pHand.fingers();
	int fingerCount = fingers.count();

	sendPose(Elbow, isLeft, arm.elbowPosition(), arm.basis());
	sendPose(Wrist, isLeft, arm.wristPosition(), arm.basis());
	sendPose(Palm, isLeft, pHand.palmPosition(), pHand.basis());

	for ( int i = 0 ; i < fingerCount ; i++ ) {
		sendFinger(fingers[i], isLeft);
	}
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendFinger(const Finger& pFinger, bool pIsLeft) {
	Finger::Type type = pFinger.type();

	sendBone(pFinger.bone(Bone::TYPE_METACARPAL), type, pIsLeft);
	sendBone(pFinger.bone(Bone::TYPE_PROXIMAL), type, pIsLeft);
	sendBone(pFinger.bone(Bone::TYPE_INTERMEDIATE), type, pIsLeft);
	sendBone(pFinger.bone(Bone::TYPE_DISTAL), type, pIsLeft);
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendBone(const Bone& pBone, Finger::Type pFingerType, bool pIsLeft) {
	Channel channel = mChannelMap[pFingerType][pBone.type()];
	sendPose(channel, pIsLeft, pBone.center(), pBone.basis());
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendPose(Channel pChannel, bool pIsLeft, const Vector& pPosition, const Matrix& pBasis) {
	OSVR_PoseState pose;
	pose.translation = getOsvrVector(pPosition);
	pose.rotation = getOsvrQuaternion(pBasis, pIsLeft);

	OSVR_ChannelCount channel = pChannel+(pIsLeft ? 0 : ChannelsPerHand);

	osvrDeviceTrackerSendPose(mDeviceToken, mTrackerInterface, &pose, channel);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
OSVR_Vec3 Tracker::getOsvrVector(const Vector& pVector) {
	const float scale = 0.001f;

	OSVR_Vec3 vec;
	vec.data[0] = pVector.x*scale;
	vec.data[1] = pVector.y*scale;
	vec.data[2] = pVector.z*scale;
	return vec;
}

/*----------------------------------------------------------------------------------------------------*/
OSVR_Quaternion Tracker::getOsvrQuaternion(const Matrix& pBasis, bool pIsLeft) {
	int zMult = (pIsLeft ? 1 : 1);
    auto xBasis = pBasis.xBasis.normalized();
    auto yBasis = pBasis.yBasis.normalized();
    auto zBasis = pBasis.zBasis.normalized();
	Eigen::Matrix3f mat = Eigen::Matrix3f();
	mat << //uses column-major order
		pBasis.xBasis.x, pBasis.yBasis.x, pBasis.zBasis.x*zMult,
		pBasis.xBasis.y, pBasis.yBasis.y, pBasis.zBasis.y*zMult,
		pBasis.xBasis.z, pBasis.yBasis.z, pBasis.zBasis.z*zMult;

	Eigen::Quaternionf eigenQuat = Eigen::Quaternionf(mat);
	eigenQuat.normalize();

	OSVR_Quaternion quat;
	osvrQuatSetW(&quat, eigenQuat.w());
	osvrQuatSetX(&quat, eigenQuat.x());
	osvrQuatSetY(&quat, eigenQuat.y());
	osvrQuatSetZ(&quat, eigenQuat.z());
	return quat;
}
