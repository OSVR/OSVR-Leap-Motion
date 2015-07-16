#include "Imaging.h"

using namespace Leap;
using namespace LeapOsvr;

//TODO: also supply the 64x64 (32-bit float) distortion map data?


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Imaging::Imaging(const osvr::pluginkit::DeviceToken& pDeviceToken, 
						OSVR_DeviceInitOptions pOptions, const Controller& pController) : 
						mDeviceToken(pDeviceToken), mController(pController), mImagingInterface(NULL) {
	osvrDeviceImagingConfigure(pOptions, &mImagingInterface, 4);
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
		sendCameraImage(images[i], time);
		sendCalibrationImage(images[i], time);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Imaging::sendCameraImage(const Image& pImage, const OSVR_TimeValue& pTime) {
	//From OSVR docs: "...a copy of the image data, will be destroyed when no longer needed..."

	OSVR_ImagingMetadata meta;
	meta.width = pImage.width();
	meta.height = pImage.height();
	meta.depth = pImage.bytesPerPixel();
	meta.channels = 1;
	meta.type = OSVR_ImagingValueType::OSVR_IVT_UNSIGNED_INT;

	int index = pImage.id();
	int dataSize = meta.width * meta.height * meta.depth;
	const unsigned char* imageData = pImage.data();
	OSVR_ImageBufferElement buffer[640*240*1] = {};

	for ( int i = 0 ; i < dataSize ; i++ ) {
		buffer[i] = imageData[i];

		//TODO: the "Imaging_cpp.exe" test only shows images when console output is enabled (why?)
		//if ( i/640 == 120 && i%64 == 0 ) { std::cout << (int)imageData[i] << "\t"; }
	}

	//std::cout << std::endl;// << std::flush;
	osvrDeviceImagingReportFrame(mDeviceToken, mImagingInterface, meta, buffer, index, &pTime);
}

/*----------------------------------------------------------------------------------------------------*/
void Imaging::sendCalibrationImage(const Image& pImage, const OSVR_TimeValue& pTime) {
	OSVR_ImagingMetadata meta;
	meta.width = 64;
	meta.height = 64*2;
	meta.depth = 1;
	meta.channels = 1;
	meta.type = OSVR_ImagingValueType::OSVR_IVT_UNSIGNED_INT;

	int index = 2+pImage.id();
	const int dataSize = 64*64*2;
	const int halfSize = dataSize/2;
	const float* imageData = pImage.distortion();
	OSVR_ImageBufferElement buffer[dataSize] = {};

	for ( int i = 0 ; i < dataSize ; i++ ) {
		int buffI = (i%2 == 0 ? i/2 : (i-1)/2+halfSize);
		float imageVal = imageData[i];
		int buffVal = 0;

		if ( imageVal >= 0 && imageVal <= 1 ) {
			buffVal = imageVal*254+1;
		}

		buffer[buffI] = buffVal;
	}

	osvrDeviceImagingReportFrame(mDeviceToken, mImagingInterface, meta, buffer, index, &pTime);
}
