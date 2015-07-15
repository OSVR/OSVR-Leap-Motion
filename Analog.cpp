#include "Analog.h"

using namespace Leap;
using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Analog::Analog(const osvr::pluginkit::DeviceToken& pDeviceToken, 
									OSVR_DeviceInitOptions pOptions, const Controller& pController) : 
									mDeviceToken(pDeviceToken), mController(pController), 
									mAnalogInterface(NULL), mValues(new OSVR_AnalogState[3]) {
	osvrDeviceAnalogConfigure(pOptions, &mAnalogInterface, 3);
}

/*----------------------------------------------------------------------------------------------------*/
void Analog::update() {
	Frame frame = mController.frame(0);

	mValues[HandCount] = frame.hands().count();
	mValues[FingerCount] = frame.fingers().count();
	mValues[ToolCount] = frame.tools().count();

	osvrDeviceAnalogSetValues(mDeviceToken, mAnalogInterface, mValues, 0);
}
