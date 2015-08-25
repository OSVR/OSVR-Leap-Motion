#if !defined(__LeapData_h__)
#define __LeapData_h__

#include "Leap.h"
#include "HandSelector.h"

namespace LeapOsvr {

	class LeapData {

		public:

			enum HandSide {
				Left,
				Right
			};

			static const int NoHandFound = -1;
			static const int MinimumWinningConfidence = 5;

			LeapData(const Leap::Controller& pController);

			void update();

			const Leap::Controller& getController() const;
			const Leap::Frame& getFrame() const;
			const bool hasBestHand(HandSide pSide) const;
			const Leap::Hand getBestHand(HandSide pSide) const;

		private:

			const Leap::Controller& mController;
			Leap::Frame mFrame;
			HandSelector mHandSelectL;
			HandSelector mHandSelectR;

			const int getBestHandIndex(HandSide pSide) const;

	};

}

#endif //__LeapData_h__
