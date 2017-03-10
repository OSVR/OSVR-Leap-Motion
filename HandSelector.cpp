#include "HandSelector.h"

using namespace LeapOsvr;

//TODO: Improve this selection process, probably by looking at the hands over time. For example, if a 
// candidate hand maintains a higher confidence than the current winner, across several frames of data, 
// then that candidate could become the new winner.


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
HandSelector::HandSelector(bool pIsLeft) :
    mIsLeft(pIsLeft), mWinningHandId(NoHandFound), mWinningHandIndex(NoHandFound)
{
    //do nothing...
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void HandSelector::update(const std::vector<LEAP_HAND> &pHands) {
    int candidateIndex = getCandidateIndex(pHands);

    if (candidateIndex == NoHandFound) {
        setNoWinner();
        return;
    }

    const LEAP_HAND &candidate = pHands[candidateIndex];

    if (!canCandidateBecomeWinner(candidate)) {
        setNoWinner();
        return;
    }

    mWinningHandId = candidate.id;
    mWinningHandIndex = candidateIndex;
}

/*----------------------------------------------------------------------------------------------------*/
const int HandSelector::getBestHandIndex() const {
    return mWinningHandIndex;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
const int HandSelector::getCandidateIndex(const std::vector<LEAP_HAND> &pHands) const {
    int handCount = static_cast<int>(pHands.size());
    int candidateIndex = NoHandFound;

    for (int i = 0; i < handCount; ++i) {
        const LEAP_HAND &hand = pHands[i];

        // Ignore invalid/mismatched hands
        // Note: I'm not assuming hand.isLeft() implies !hand.isRight() or the reverse, and assuming that
        // !hand.isLeft() && !hand.isRight() is possible
        bool isValid = true; // the C API doesn't have this, so... always valid?
        if (isValid && ((hand.type == eLeapHandType_Left && mIsLeft) || (hand.type == eLeapHandType_Right && !mIsLeft))) {
            //Capture first candidate
            if (candidateIndex == NoHandFound) {
                candidateIndex = i;
                continue;
            }

            //If there are multiple candidates, choose the best one
            const LEAP_HAND &prevCandidate = pHands[candidateIndex];
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
    const LEAP_HAND &pNewCandidate, const LEAP_HAND &pPrevCandidate)  const {
    return (pNewCandidate.confidence > pPrevCandidate.confidence);
}

/*----------------------------------------------------------------------------------------------------*/
const bool HandSelector::canCandidateBecomeWinner(const LEAP_HAND &pCandidate)  const {
    return (pCandidate.confidence >= MinimumWinningConfidence / 100.0f);
}

/*----------------------------------------------------------------------------------------------------*/
void HandSelector::setNoWinner() {
    mWinningHandId = NoHandFound;
    mWinningHandIndex = NoHandFound;
}
