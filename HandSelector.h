#include "Leap.h"

namespace LeapOsvr {

	class HandSelector {

		public:

			static const int NoHandFound = -1;
			static const int MinimumWinningConfidence = 5;

			HandSelector(const Leap::Controller& pController, bool pIsLeft);
			const int getBestHandIndex(const Leap::HandList& pHands);

		private:

			const Leap::Controller& mController;
			bool mIsLeft;
			int mWinningHandId;

			const int getCandidateIndex(const Leap::HandList& pHands);
			const bool isNewCandidateBetterThanPrevious(
				const Leap::Hand& pNewCandidate, const Leap::Hand& pPrevCandidate);
			const bool canCandidateBecomeWinner(const Leap::Hand& pCandidate);
			const int setNoWinner();

	};

}