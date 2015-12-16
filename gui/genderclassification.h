#ifndef GENDERCLASSIFICATION_H
#define GENDERCLASSIFICATION_H

#include <QtGui/QMainWindow>
#include <opencv/highgui.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "../framecapture.h"
#include "../detailedfacedetector.h"
#include "../util.h"
#include "fisherfaceclassifer.h"
#include "Virtual_Face.h"
class QListWidget;
class QPushButton;

namespace gui
{
	class WebcamWidget;

	/*! \brief: Main application window
	 * 
	 */
	class genderclassification : public QMainWindow
	{
		Q_OBJECT

	public:
		genderclassification(QWidget *parent = 0, Qt::WFlags flags = 0);
		~genderclassification();
		
	public slots:
		void captureTrainingImage();
		void trainRecognizer();
	     void recognizeFaces();

	protected:

		static const cv::Size testImageSize;

		WebcamWidget *webcamWidget;
		QListWidget *logWidget;

		QPushButton *captureTrainingImageButton;
		QPushButton *trainRecognizerButton;
		QPushButton *recognizeFaceButton;

		boost::thread frameCaptureThread;
		boost::thread faceDetectorThread;
		Virtual_Face VPlayer;
		//auto_ptr would suffice here too, for our purposes! 
		boost::shared_ptr<cv::VideoCapture> videoCapture;
		boost::shared_ptr<cognition::FrameCapture> frameCapture;
		boost::shared_ptr<cognition::DetailedFaceDetector> faceDetector;
		boost::shared_ptr<cognition::fisherfaceclassifer> recognizer;

		void closeEvent(QCloseEvent *event);

		void setupFramework();
		void setupGUI();
		void keyPressEvent(QKeyEvent *event);
		void captureFrameAndFaces(cognition::Detector::RectVector &rects, cv::Mat &frame);

	};

}
#endif // GenderClassification_H
