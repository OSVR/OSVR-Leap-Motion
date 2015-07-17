#include "Imaging.h"

using namespace Leap;
using namespace LeapOsvr;


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
	const int width = 64;
	const int height = 64;
	const int srcChannels = 2;
	const int destChannels = 3; //using 2 channels is not supported
	const int srcLen = width*height*srcChannels;

	OSVR_ImagingMetadata meta;
	meta.width = width;
	meta.height = height;
	meta.depth = 4;
	meta.channels = destChannels;
	meta.type = OSVR_ImagingValueType::OSVR_IVT_FLOATING_POINT;

	int index = 2+pImage.id();
	const float* imageData = pImage.distortion();
	float imageCopy[width*height*destChannels] = {};

	for ( int i = 0 ; i < srcLen ; i++ ) {
		int copyI = (i/srcChannels*destChannels + i%srcChannels);
		imageCopy[copyI] = imageData[i];
	}

	OSVR_ImageBufferElement* buffer = reinterpret_cast<OSVR_ImageBufferElement*>(imageCopy);
	osvrDeviceImagingReportFrame(mDeviceToken, mImagingInterface, meta, buffer, index, &pTime);
}
