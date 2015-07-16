#include "Tracker.h"

using namespace Leap;
using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Tracker::Tracker(const osvr::pluginkit::DeviceToken& pDeviceToken,
									OSVR_DeviceInitOptions pOptions, const Controller& pController) : 
									mDeviceToken(pDeviceToken), mController(pController), 
									mTrackerInterface(NULL) {
	osvrDeviceTrackerConfigure(pOptions, &mTrackerInterface);

	mChannelMap[Finger::Type::TYPE_THUMB][Bone::TYPE_METACARPAL] = ThumbMeta;
	mChannelMap[Finger::Type::TYPE_THUMB][Bone::TYPE_PROXIMAL] = ThumbProx;
	mChannelMap[Finger::Type::TYPE_THUMB][Bone::TYPE_INTERMEDIATE] = ThumbInter;
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
	Frame frame = mController.frame(0);
	HandList hands = frame.hands();
	int handCount = hands.count();

	for ( int i = 0 ; i < handCount ; ++i ) {
		sendHand(hands[i]);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendHand(const Hand& pHand) {
	if ( !pHand.isValid() ) {
		return;
	}

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
	pose.rotation = getOsvrQuaternion(pBasis);

	OSVR_ChannelCount channel = pChannel+(pIsLeft ? 0 : ChannelsPerHand);

	osvrDeviceTrackerSendPose(mDeviceToken, mTrackerInterface, &pose, channel);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
OSVR_Vec3 Tracker::getOsvrVector(const Vector& pVector) {
	const float scale = 0.001f;

	OSVR_Vec3 vec;
	vec.data[0] =  pVector.x*scale;
	vec.data[1] =  pVector.y*scale;
	vec.data[2] = -pVector.z*scale;
	return vec;
}

/*--------------------------------------------------------------------------------------------*/
OSVR_Quaternion Tracker::getOsvrQuaternion(const Matrix& pBasis) {
	//Code adapted from:
	//http://euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion

	//TODO: Use OSVR util + Eigen instead? Not sure how to include this in the plugin...

	FloatArray mat = pBasis.toArray4x4();
	float trace = mat[0u] + mat[5u] + mat[10u];
	float w, x, y, z;

	if ( trace > 0 ) {
		float s = 0.5f / sqrtf(trace+1.0f);
		w = 0.25f / s;
		x = (mat[6u] - mat[9u]) * s;
		y = (mat[8u] - mat[2u]) * s;
		z = (mat[1u] - mat[4u]) * s;
	}
	else {
		if ( mat[0u] > mat[5u] && mat[0u] > mat[10u] ) {
			float s = 2.0f * sqrtf(1.0f + mat[0u] - mat[5u] - mat[10u]);
			w = (mat[6u] - mat[9u]) / s;
			x = 0.25f * s;
			y = (mat[4u] + mat[1u]) / s;
			z = (mat[8u] + mat[2u]) / s;
		}
		else if ( mat[5u] > mat[10u] ) {
			float s = 2.0f * sqrtf(1.0f + mat[5u] - mat[0u] - mat[10u]);
			w = (mat[8u] - mat[2u]) / s;
			x = (mat[4u] + mat[1u]) / s;
			y = 0.25f * s;
			z = (mat[9u] + mat[6u]) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + mat[10u] - mat[0u] - mat[5u]);
			w = (mat[1u] - mat[4u]) / s;
			x = (mat[8u] + mat[2u]) / s;
			y = (mat[9u] + mat[6u]) / s;
			z = 0.25f * s;
		}
	}

	OSVR_Quaternion quat;
	osvrQuatSetW(&quat, w);
	osvrQuatSetX(&quat, x);
	osvrQuatSetY(&quat, y);
	osvrQuatSetZ(&quat, z);
	return quat;
}