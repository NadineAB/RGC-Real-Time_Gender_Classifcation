#ifndef WEBCAMWIDGET_H
#define WEBCAMWIDGET_H
#include <QWidget>
#include "../framereceiver.h"
#include "../controller.h"
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../detector.h"
#include "../detailedfacedetector.h"
#include "fisherfaceclassifer.h"
#include "genderclassification.h"
#include "Virtual_Face.h"
//forward declaration
namespace cognition{class FrameCapture;}
namespace gui
{

	/*! \brief:	Widget that displays the webcam feed, provided by a capture devices
	 *			it also draws the rectangles it recieves from detectors!
	 * 
	 */
	class WebcamWidget : public QWidget, 
		public cognition::FrameReceiver,
		public cognition::controller  
	{
		Q_OBJECT

	public:
		typedef cognition::FrameCapture* FrameCapturePtr;
          WebcamWidget(QWidget *parent, FrameCapturePtr existingCaptureDevice);
		WebcamWidget();
		virtual ~WebcamWidget();
          QImage convertToQImage(const cv::Mat& mat) const;
          void receiveFrame(const cv::Mat &frame);
		//refactor this in another class, this is just for testing!
		void stateChanged(cognition::Detector *detector);
          //just for convenience!
		cv::Mat getCurrentFrame();
		cognition::Detector::RectVector getCurrentFaces();
		int flag;
		int WebGender;
		double confidence;
		int gtimer;
		std::vector <pair<cv::Mat,int>> PairGender;
	     Virtual_Face VPlayer;
	protected:
		
		cognition::Detector::RectVector faces;
		cognition::DetailedFaceDetector::DetailedFaces detailedFaces;
		boost::mutex facesLock;
          cv::Mat currentFrame;
		cv::Mat nextFrame;
		boost::shared_ptr<cognition::fisherfaceclassifer> webrecognizer;
		//lock for loading new frames
		boost::mutex frameLoadLock;
          //copies nextFrame to currentFrame, taking concurrency into account
		virtual void loadNextFrameIntoCurrent();
		void paintEvent(QPaintEvent* event);
		void keyPressEvent(QKeyEvent *event);
          boost::posix_time::ptime previousTime;
		//std::time_t previousTime;
		volatile int detectionFramerate;
          void updateFramerate();
};
}

#endif // WEBCAMWIDGET_H
