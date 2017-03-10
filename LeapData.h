#if !defined(__LeapData_h__)
#define __LeapData_h__

#include <vector>
#include "LeapC.h"
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

        LeapData(const LEAP_CONNECTION pController);
        ~LeapData();

        bool update();

        const LEAP_CONNECTION getConnection() const;
        const LEAP_TRACKING_EVENT& getFrame() const;
        const bool hasBestHand(HandSide pSide) const;
        const LEAP_HAND& getBestHand(HandSide pSide) const;
        const bool isConnected() const;
        const bool isDeviceConnected() const;

    private:

        const LEAP_CONNECTION mConnection = nullptr;
        LEAP_TRACKING_EVENT mFrame;
        HandSelector mHandSelectL;
        HandSelector mHandSelectR;
        std::vector<LEAP_HAND> mHands;
        const int getBestHandIndex(HandSide pSide) const;
        void copyFrame(const LEAP_TRACKING_EVENT& frame);
    };

}

#endif //__LeapData_h__
