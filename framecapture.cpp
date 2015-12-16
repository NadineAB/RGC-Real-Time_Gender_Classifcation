#include "framecapture.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
namespace cognition
{
	// constructor Frame Capture with specific frame rate 
	// capturing the vedio
	FrameCapture::FrameCapture(float framerate)
	{
		captureDevice = new CaptureDevice(0);
		setFramerate(framerate);
	}
	FrameCapture::FrameCapture(CaptureDevicePtr existingCaptureDevice, float framerate)
		:captureDevice(existingCaptureDevice)
	{
		if(captureDevice == 0) // if the user did not provide an empty pointer, construct a capturing device
		captureDevice = new CaptureDevice(0);
		setFramerate(framerate);
	}
     //destructor of VideoCapture does not release the webcam feed, call it here!
	FrameCapture::~FrameCapture()
	{
		captureDevice->release();		
	}
	// start capturing from webcam
	void FrameCapture::startCapturing()
	{
		keepCapturing = true;
	// capture in loop to detect
		captureLoop();
	}
	// stop capturing put capture flag to false
	void FrameCapture::stopCapturing()
	{
		keepCapturing = false;
	}
    // capture method for loop to capture the vedio and image from 
    // loop of capturing
    void FrameCapture::captureLoop()
	{
		while(keepCapturing)
		{
			cv::Mat frame; // open cv instatanc of camera
			*captureDevice >> frame; // 
		     cv::flip( frame, frame, 1); //1 is flip horizontal
			notifyReceivers(frame);
               // using boost to motion during capturing the vedio
			boost::this_thread::interruptible_wait(framerateWaitMs);
		}
	}
     // open webcam strat capture
	void FrameCapture::notifyReceivers(cv::Mat &frame)
	{
		boost::lock_guard<boost::mutex>(this->receiversLock);
		for(ReceiverSetItr itr = receivers.begin(); itr != receivers.end(); ++itr)
			(*itr)->receiveFrame(frame);
	}

	bool FrameCapture::addFrameReceiver(FrameReceiverPtr receiver)
	{
		boost::lock_guard<boost::mutex>(this->receiversLock);

		return receivers.insert(receiver).second; 
	}

	void FrameCapture::removeFrameReceiver(FrameReceiverPtr receiver)
	{
		boost::lock_guard<boost::mutex>(this->receiversLock);

		receivers.erase(receiver);
	}
}