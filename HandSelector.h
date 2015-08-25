#include "Leap.h"

namespace LeapOsvr {

	class HandSelector {

		public:

			static const int NoHandFound = -1;
			static const int MinimumWinningConfidence = 5;

			HandSelector(bool pIsLeft);

			void update(const Leap::HandList& pHands);
			const int getBestHandIndex() const;

		private:

			bool mIsLeft;
			int mWinningHandId;
			int mWinningHandIndex;

			const int getCandidateIndex(const Leap::HandList& pHands) const;
			const bool isNewCandidateBetterThanPrevious(
				const Leap::Hand& pNewCandidate, const Leap::Hand& pPrevCandidate) const;
			const bool canCandidateBecomeWinner(const Leap::Hand& pCandidate) const;
			void setNoWinner();

	};

}