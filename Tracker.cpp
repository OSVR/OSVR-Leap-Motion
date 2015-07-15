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
void Tracker::sendHand(Leap::Hand pHand) {
	if ( !pHand.isValid() ) {
		return;
	}

	bool isLeft = pHand.isLeft();
	FingerList fingers = pHand.fingers();
	int fingerCount = fingers.count();

	sendPose(Palm, isLeft, pHand.palmPosition(), pHand.basis());

	for ( int i = 0 ; i < fingerCount ; i++ ) {
		sendFinger(fingers[i], isLeft);
	}
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendFinger(Leap::Finger pFinger, bool pIsLeft) {
	Finger::Type type = pFinger.type();
	Bone bone = pFinger.bone(Bone::TYPE_DISTAL);
	Channel channel;

	switch ( type ) {
		case Finger::Type::TYPE_THUMB:
			channel = ThumbTip;
			break;

		case Finger::Type::TYPE_INDEX:
			channel = IndexTip;
			break;

		case Finger::Type::TYPE_MIDDLE:
			channel = MiddleTip;
			break;

		case Finger::Type::TYPE_RING:
			channel = RingTip;
			break;

		case Finger::Type::TYPE_PINKY:
			channel = PinkyTip;
			break;
	}

	sendPose(channel, pIsLeft, bone.center(), bone.basis());
}

/*----------------------------------------------------------------------------------------------------*/
void Tracker::sendPose(Channel pChannel, bool pIsLeft, Leap::Vector pPosition, Leap::Matrix pBasis) {
	OSVR_PoseState pose;
	pose.translation = getOsvrVector(pPosition);
	pose.rotation = getOsvrQuaternion(pBasis);

	OSVR_ChannelCount channel = pChannel+(pIsLeft ? 0 : ChannelsPerHand);

	osvrDeviceTrackerSendPose(mDeviceToken, mTrackerInterface, &pose, channel);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
OSVR_Vec3 Tracker::getOsvrVector(Leap::Vector pVector) {
	const float scale = 0.001f;

	OSVR_Vec3 vec;
	vec.data[0] =  pVector.x*scale;
	vec.data[1] =  pVector.y*scale;
	vec.data[2] = -pVector.z*scale;
	return vec;
}

/*--------------------------------------------------------------------------------------------*/
OSVR_Quaternion Tracker::getOsvrQuaternion(Leap::Matrix pBasis) {
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