#include "HandSelector.h"

using namespace Leap;
using namespace LeapOsvr;

//TODO: Improve this selection process, probably by looking at the hands over time. For example, if a 
// candidate hand maintains a higher confidence than the current winner, across several frames of data, 
// then that candidate could become the new winner.


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
HandSelector::HandSelector(bool pIsLeft) : mIsLeft(pIsLeft), 
										mWinningHandId(NoHandFound), mWinningHandIndex(NoHandFound) {
	//do nothing...
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void HandSelector::update(const HandList& pHands) {
	int candidateIndex = getCandidateIndex(pHands);

	if ( candidateIndex == NoHandFound ) {
		setNoWinner();
		return;
	}

	Hand candidate = pHands[candidateIndex];

	if ( !canCandidateBecomeWinner(candidate) ) {
		setNoWinner();
		return;
	}

	mWinningHandId = candidate.id();
	mWinningHandIndex = candidateIndex;
}

/*----------------------------------------------------------------------------------------------------*/
const int HandSelector::getBestHandIndex() const {
	return mWinningHandIndex;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
const int HandSelector::getCandidateIndex(const HandList& pHands) const {
	int handCount = pHands.count();
	int candidateIndex = NoHandFound;
    
	for ( int i = 0 ; i < handCount ; ++i ) {
		Hand hand = pHands[i];

		// Ignore invalid/mismatched hands
        // Note: I'm not assuming hand.isLeft() implies !hand.isRight() or the reverse, and assuming that
        // !hand.isLeft() && !hand.isRight() is possible
        if (hand.isValid() && ((hand.isLeft() && mIsLeft) || (hand.isRight() && !mIsLeft))) {
            //Capture first candidate
            if (candidateIndex == NoHandFound) {
                candidateIndex = i;
                continue;
            }

            //If there are multiple candidates, choose the best one
            Hand prevCandidate = pHands[candidateIndex];
            if (isNewCandidateBetterThanPrevious(hand, prevCandidate)) {
                candidateIndex = i;
                continue;
            }
        }
	}

	return candidateIndex;
}

/*----------------------------------------------------------------------------------------------------*/
const bool HandSelector::isNewCandidateBetterThanPrevious(
							const Leap::Hand& pNewCandidate, const Leap::Hand& pPrevCandidate)  const {
	return (pNewCandidate.confidence() > pPrevCandidate.confidence());
}

/*----------------------------------------------------------------------------------------------------*/
const bool HandSelector::canCandidateBecomeWinner(const Leap::Hand& pCandidate)  const {
	return (pCandidate.confidence() >= MinimumWinningConfidence/100.0f);
}

/*----------------------------------------------------------------------------------------------------*/
void HandSelector::setNoWinner() {
	mWinningHandId = NoHandFound;
	mWinningHandIndex = NoHandFound;
}
