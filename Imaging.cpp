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

/*----------------------------------------------------------------------------------------------------* /
void Imaging::sendCalibrationImage(const Image& pImage, const OSVR_TimeValue& pTime) {
	const int channels = 2; //uses RG values
	const int depth = 2; //use 16-bit values

	OSVR_ImagingMetadata meta;
	meta.width = 64;
	meta.height = 64*channels; //double height, rather than using 2 channels (which is not supported)
	meta.depth = depth;
	meta.channels = 1;
	meta.type = OSVR_ImagingValueType::OSVR_IVT_UNSIGNED_INT;

	int index = 2+pImage.id();
	const int srcSize = 64*64*channels;
	const int srcHalfSize = srcSize/2;
	const int destSize = srcSize*depth;
	const float* imageData = pImage.distortion();
	OSVR_ImageBufferElement buffer[destSize] = {};
	int buffValMax = pow(2, depth*8)-1;

	for ( int i = 0 ; i < srcSize ; i++ ) {
		float imageVal = imageData[i];

		if ( imageVal < 0 || imageVal > 1 ) {
			continue;
		}

		int buffVal = imageVal*buffValMax+1; //+1 avoids zero, which represents "out of bounds"
		int buffI = i*depth;
		//int buffI = (i%2 == 0 ? i/2 : (i-1)/2+srcHalfSize)*depth; //split into top/bottom sections

		buffer[buffI] = buffVal%256;
		buffer[buffI+1] = buffVal>>8;
	}

	osvrDeviceImagingReportFrame(mDeviceToken, mImagingInterface, meta, buffer, index, &pTime);
}

/*----------------------------------------------------------------------------------------------------*/
void Imaging::sendCalibrationImage(const Image& pImage, const OSVR_TimeValue& pTime) {
	const int srcChannels = 2;
	const int destDepth = 2; //use 16-bit values
	const int destChannels = 3; //using 2 channels is not supported

	OSVR_ImagingMetadata meta;
	meta.width = 64;
	meta.height = 64;
	meta.depth = destDepth;
	meta.channels = destChannels;
	meta.type = OSVR_ImagingValueType::OSVR_IVT_UNSIGNED_INT;

	int index = 2+pImage.id();
	const int srcSize = 64*64*srcChannels;
	const int srcHalfSize = srcSize/2;
	const int destSize = srcSize*destDepth*destChannels;
	const float* imageData = pImage.distortion();
	OSVR_ImageBufferElement buffer[destSize] = {};
	int buffValMax = pow(2, destDepth*8)-1;

	for ( int i = 0 ; i < srcSize ; i++ ) {
		float imageVal = imageData[i];

		if ( imageVal < 0 || imageVal > 1 ) {
			continue;
		}

		int buffVal = imageVal*buffValMax+1; //+1 avoids zero, which represents "out of bounds"
		int buffI = (i/srcChannels*destChannels + i%srcChannels)*destDepth;

		buffer[buffI] = buffVal%256;
		buffer[buffI+1] = buffVal>>8;
	}

	osvrDeviceImagingReportFrame(mDeviceToken, mImagingInterface, meta, buffer, index, &pTime);
}

/*

DISTORTION: SOURCE

0  1  2  3  4  5
0x 0y 1x 1y 2x 2y

DISTORTION: RGB BUFFER

0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
0xa 0xb 0ya 0yb 0.. 0.. 1xa 1xb 1ya 1yb 1.. 1.. 2xa 2xb 2ya 2yb 2.. 2..

DISTORTION: MAPPING

0 => 0,1
1 => 2,3
2 => 6,7
3 => 8,9
4 => 12,13
5 => 14,15

(src/srcChannels*destChannels + src%srcChannels)*destDepth

src=3
(3/2*3 + 3%2)*2
(1*3 + 1)*2
4*2
8

src=4
(4/2*3 + 4%2)*2
(2*3 + 0)*2
6*2
12

src=5
(5/2*3 + 5%2)*2
(2*3 + 1)*2
7*2
14

*/
