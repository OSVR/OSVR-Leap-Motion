#include "Imaging.h"

using namespace Leap;
using namespace LeapOsvr;

//TODO: also supply the 64x64 (32-bit float) distortion map data?


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Imaging::Imaging(const osvr::pluginkit::DeviceToken& pDeviceToken, 
						OSVR_DeviceInitOptions pOptions, const Controller& pController) : 
						mDeviceToken(pDeviceToken), mController(pController), mImagingInterface(NULL) {
	osvrDeviceImagingConfigure(pOptions, &mImagingInterface, 2);
}

/*----------------------------------------------------------------------------------------------------*/
void Imaging::update() {
	ImageList images = mController.images();
	int imageCount = images.count();
	OSVR_TimeValue time;

	osvrTimeValueGetNow(&time);

	if ( imageCount > 2 ) {
		std::cout << "This plugin does not support more than two Leap Motion images." << std::endl;
		imageCount = 2;
	}

	for ( int i = 0 ; i < imageCount ; i++ ) {
		sendCameraImage(&images[i], &time);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Imaging::sendCameraImage(Image* pImage, OSVR_TimeValue* pTime) {
	//From OSVR docs: "...a copy of the image data, will be destroyed when no longer needed..."

	OSVR_ImagingMetadata meta;
	meta.width = pImage->width();
	meta.height = pImage->height();
	meta.depth = pImage->bytesPerPixel();
	meta.channels = 1;
	meta.type = OSVR_ImagingValueType::OSVR_IVT_UNSIGNED_INT;

	int index = pImage->id();
	int dataSize = meta.width * meta.height * meta.depth;
	const unsigned char* imageData = pImage->data();
	OSVR_ImageBufferElement buffer[640*240*1];

	//TODO: copy data vs. sending: (OSVR_ImageBufferElement*)pImage->data()

	for ( int d = 0 ; d < dataSize ; d++ ) {
		buffer[d] = imageData[d];
		
		//TODO: the "Imaging_cpp.exe" test only shows images when console output is enabled (why?)
		//if ( d/640 == 120 && d%64 == 0 ) { std::cout << (int)imageData[d] << "\t"; }
	}

	//std::cout << std::endl;// << std::flush;
	osvrDeviceImagingReportFrame(mDeviceToken, mImagingInterface, meta, buffer, index, pTime);
}
