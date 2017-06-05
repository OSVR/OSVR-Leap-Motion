//#include "Imaging.h"
//#include <chrono>
//
//using namespace Leap;
//using namespace LeapOsvr;
//using namespace std::chrono;
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///*----------------------------------------------------------------------------------------------------*/
//Imaging::Imaging(osvr::pluginkit::DeviceToken& pDeviceToken, 
//				OSVR_DeviceInitOptions pOptions, const LeapData& pLeapData) : 
//				mDeviceToken(pDeviceToken), mLeapData(pLeapData), 
//				mImagingInterface(osvr::pluginkit::ImagingInterface(pOptions, ImagingChannelCount)) {
//	mDistortionCache[0] = NULL;
//	mDistortionCache[1] = NULL;
//	mNextDistortionSecond = 0;
//}
//
///*----------------------------------------------------------------------------------------------------*/
//Imaging::~Imaging() {
//	delete mDistortionCache[0];
//	delete mDistortionCache[1];
//}
//
///*----------------------------------------------------------------------------------------------------*/
//void Imaging::update() {
//	ImageList images = mLeapData.getController().images();
//	int imageCount = images.count();
//
//	int64 currSecond = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
//	bool sendDistortion = (currSecond >= mNextDistortionSecond);
//	
//	if ( sendDistortion ) {
//		mNextDistortionSecond = currSecond+2; //send every X seconds
//	}
//
//	if ( imageCount > 2 ) {
//		std::cout << "This plugin does not support more than two Leap Motion images." << std::endl;
//		imageCount = 2;
//	}
//
//	for ( int i = 0 ; i < imageCount ; i++ ) {
//		Image image = images[i];
//
//		sendCameraImage(image);
//
//		if ( sendDistortion ) {
//			sendDistortionImage(image);
//		}
//	}
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///*----------------------------------------------------------------------------------------------------*/
//void Imaging::sendCameraImage(const Image& pImage) {
//	int width = pImage.width();
//	int height = pImage.height();
//	int srcLen = width*height;
//	OSVR_ChannelCount channel = pImage.id();
//	const unsigned char* imageData = pImage.data();
//	cv::Mat matrix = cv::Mat(height, width, CV_8UC1);
//
//	for ( int i = 0 ; i < srcLen ; i++ ) {
//		matrix.at<unsigned char>(i/width, i%width) = imageData[i];
//	}
//
//	mDeviceToken.send(mImagingInterface, osvr::pluginkit::ImagingMessage(matrix, channel));
//}
//
///*----------------------------------------------------------------------------------------------------*/
//void Imaging::sendDistortionImage(const Image& pImage) {
//	int id = pImage.id();
//
//	if ( mDistortionCache[id] == NULL ) {
//		cacheDistortionImage(pImage);
//	}
//
//	OSVR_ChannelCount channel = 2+id;
//	cv::Mat matrix = mDistortionCache[id]->clone();
//
//	mDeviceToken.send(mImagingInterface, osvr::pluginkit::ImagingMessage(matrix, channel));
//}
//
///*----------------------------------------------------------------------------------------------------*/
//void Imaging::cacheDistortionImage(const Image& pImage) {
//	const int width = 64;
//	const int height = 64;
//	const int srcLen = width*height;
//	const int srcChannels = 2;
//
//	int id = pImage.id();
//	const float* srcData = pImage.distortion();
//	cv::Mat* cache = new cv::Mat(height, width, CV_32FC3);
//
//	for ( int i = 0 ; i < srcLen ; i++ ) {
//		float* cell = cache->at<float[3]>(i/width, i%width);
//		cell[0] = srcData[i*2];
//		cell[1] = srcData[i*2+1];
//		cell[2] = 0;
//	}
//
//	mDistortionCache[id] = cache;
//}
