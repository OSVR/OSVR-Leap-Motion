#include <vector>
#include "LeapC.h"

namespace LeapOsvr {

	class HandSelector {

		public:

			static const int NoHandFound = -1;
			static const int MinimumWinningConfidence = 5;

			HandSelector(bool pIsLeft);

			void update(const std::vector<LEAP_HAND> &pHands);
			const int getBestHandIndex() const;

		private:

			bool mIsLeft;
			int mWinningHandId;
			int mWinningHandIndex;

            const int getCandidateIndex(const std::vector<LEAP_HAND> &pHands) const;
			const bool isNewCandidateBetterThanPrevious(
				const LEAP_HAND &pNewCandidate, const LEAP_HAND &pPrevCandidate) const;
			const bool canCandidateBecomeWinner(const LEAP_HAND &pCandidate) const;
			void setNoWinner();

	};

}