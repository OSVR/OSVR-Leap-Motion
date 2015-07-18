#include "Imaging.h"

using namespace Leap;
using namespace LeapOsvr;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
Imaging::Imaging(osvr::pluginkit::DeviceToken& pDeviceToken, 
				OSVR_DeviceInitOptions pOptions, const Controller& pController) : 
				mDeviceToken(pDeviceToken), mController(pController), mImagingDeviceInterface(NULL) {
	osvrDeviceImagingConfigure(pOptions, &mImagingDeviceInterface, 4);
	mImagingInterface = osvr::pluginkit::ImagingInterface(pOptions);
	mDistortionCache[0] = NULL;
	mDistortionCache[1] = NULL;
}

/*----------------------------------------------------------------------------------------------------*/
Imaging::~Imaging() {
	delete mDistortionCache[0];
	delete mDistortionCache[1];
}

/*----------------------------------------------------------------------------------------------------*/
void Imaging::update() {
	ImageList images = mController.images();
	int imageCount = images.count();

	if ( imageCount > 2 ) {
		std::cout << "This plugin does not support more than two Leap Motion images." << std::endl;
		imageCount = 2;
	}

	for ( int i = 0 ; i < imageCount ; i++ ) {
		Image image = images[i];

		sendCameraImage(image);
		sendCalibrationImage(image);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
void Imaging::sendCameraImage(const Image& pImage) {
	int width = pImage.width();
	int height = pImage.height();
	int srcLen = width*height;
	OSVR_ChannelCount channel = pImage.id();
	const unsigned char* imageData = pImage.data();
	cv::Mat matrix = cv::Mat(height, width, CV_8UC1);

	for ( int i = 0 ; i < srcLen ; i++ ) {
		matrix.at<unsigned char>(i/width, i%width) = imageData[i];
	}

	mDeviceToken.send(mImagingInterface, osvr::pluginkit::ImagingMessage(matrix, channel));
}

/*----------------------------------------------------------------------------------------------------*/
void Imaging::sendCalibrationImage(const Image& pImage) {
	int id = pImage.id();

	if ( mDistortionCache[id] == NULL ) {
		cacheCalibrationImage(pImage);
	}

	OSVR_ChannelCount channel = 2+id;
	cv::Mat matrix = mDistortionCache[id]->clone();

	mDeviceToken.send(mImagingInterface, osvr::pluginkit::ImagingMessage(matrix, channel));
}

/*----------------------------------------------------------------------------------------------------*/
void Imaging::cacheCalibrationImage(const Image& pImage) {
	const int width = 64;
	const int height = 64;
	const int srcLen = width*height;
	const int srcChannels = 2;

	int id = pImage.id();
	const float* srcData = pImage.distortion();
	cv::Mat* cache = new cv::Mat(height, width, CV_32FC3);

	for ( int i = 0 ; i < srcLen ; i++ ) {
		float* cell = cache->at<float[3]>(i/width, i%width);
		cell[0] = srcData[i*2];
		cell[1] = srcData[i*2+1];
		cell[2] = 0;
	}

	mDistortionCache[id] = cache;
}
