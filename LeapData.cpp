#include "LeapData.h"

using namespace Leap;
using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
LeapData::LeapData(const Controller& pController) : mController(pController),
																	mHandSelectL(HandSelector(true)),
																	mHandSelectR(HandSelector(false)) {
	//do nothing...
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void LeapData::update() {
	mFrame = mController.frame(0);

	const HandList& hands = mFrame.hands();

	mHandSelectL.update(hands);
	mHandSelectR.update(hands);

	//std::cout << "HAND INDEXES: " << getBestHandIndex(Left) << " / " << 
	//	getBestHandIndex(Right) << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
const Controller& LeapData::getController() const {
	return mController;
}

/*----------------------------------------------------------------------------------------------------*/
const Frame& LeapData::getFrame() const {
	return mFrame;
}

/*----------------------------------------------------------------------------------------------------*/
const bool LeapData::hasBestHand(HandSide pSide) const {
	return (getBestHandIndex(pSide) != NoHandFound);
}

/*----------------------------------------------------------------------------------------------------*/
const Hand LeapData::getBestHand(HandSide pSide) const {
	const int index = getBestHandIndex(pSide);

	if ( index == NoHandFound ) {
		throw std::out_of_range("No 'best hand' was found.");
	}

	return mFrame.hands()[index];
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
const int LeapData::getBestHandIndex(HandSide pSide) const {
	return (pSide == Left ? mHandSelectL : mHandSelectR).getBestHandIndex();
}
