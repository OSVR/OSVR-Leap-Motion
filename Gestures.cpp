//#include "Gestures.h"
//
//using namespace Leap;
//using namespace LeapOsvr;
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///*----------------------------------------------------------------------------------------------------*/
//Gestures::Gestures(const osvr::pluginkit::DeviceToken& pDeviceToken,
//					OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData) : 
//					mDeviceToken(pDeviceToken), mLeapData(pLeapData)/*,mGestureInterface(NULL)*/ {
//	//osvrDeviceGestureConfig(pOptions, &mGestureInterface, 4);
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///*----------------------------------------------------------------------------------------------------*/
//void Gestures::update() {
//	Frame frame = mLeapData.getFrame();
//	GestureList gestures = frame.gestures();
//	int gestureCount = gestures.count();
//
//	for ( int i = 0 ; i < gestureCount ; i++ ) {
//		sendGesture(gestures[i]);
//	}
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///*----------------------------------------------------------------------------------------------------*/
//void Gestures::sendGesture(const Gesture& pGesture) {
//	if ( !pGesture.isValid() ) {
//		return;
//	}
//
//	Gesture::Type type = pGesture.type();
//	Gesture::State state = pGesture.state();
//	int64_t duration = pGesture.duration();
//
//	Leap::Vector position;
//	Leap::Vector direction;
//	float progress = 0;
//	float speed = 0;
//	float radius = 0;
//	std::string typeKey;
//	std::string stateName;
//
//	////
//
//	if ( type == Gesture::TYPE_CIRCLE ) {
//		CircleGesture circle = (CircleGesture)pGesture;
//		position = circle.center();
//		progress = circle.progress();
//		radius = circle.radius();
//		typeKey = GestureKey::Circle;
//	}
//	else if ( type == Gesture::TYPE_SWIPE ) {
//		SwipeGesture swipe = (SwipeGesture)pGesture;
//		position = swipe.startPosition();
//		direction = swipe.direction();
//		speed = swipe.speed();
//		typeKey = GestureKey::Swipe;
//	}
//	else if ( type == Gesture::TYPE_KEY_TAP ) {
//		KeyTapGesture keyTap = (KeyTapGesture)pGesture;
//		position = keyTap.position();
//		direction = keyTap.direction();
//		progress = keyTap.progress();
//		typeKey = GestureKey::KeyTap;
//	}
//	else if ( type == Gesture::TYPE_SCREEN_TAP ) {
//		ScreenTapGesture screenTap = (ScreenTapGesture)pGesture;
//		position = screenTap.position();
//		direction = screenTap.direction();
//		progress = screenTap.progress();
//		typeKey = GestureKey::ScreenTap;
//	}
//	else {
//		std::cout << "[LEAP] Unsupported gesture type: " << type << std::endl;
//		return;
//	}
//
//	////
//
//	switch ( state ) {
//		case Gesture::STATE_INVALID:
//			stateName = "Invalid";
//			break;
//
//		case Gesture::STATE_START:
//			stateName = "Start";
//			break;
//
//		case Gesture::STATE_UPDATE:
//			stateName = "Update";
//			break;
//
//		case Gesture::STATE_STOP:
//			stateName = "Stop";
//			break;
//
//		default:
//			std::cout << "[LEAP] Unsupported state type: " << state << std::endl;
//			return;
//	}
//
//	////
//
//	std::cout << "GESTURE MESSAGE" <<
//		"\n - type: " << type << " (" << typeKey << ")" <<
//		"\n - stat: " << state << " (" << stateName << ")" <<
//		"\n - dura: " << duration <<
//		"\n - pos:  " << position <<
//		"\n - dir:  " << direction <<
//		"\n - prog: " << progress <<
//		"\n - radi: " << radius <<
//		"\n - spd:  " << speed <<
//		"\n" << std::endl;
//}
