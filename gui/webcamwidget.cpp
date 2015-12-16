#include "webcamwidget.h"
#include <QImage>
#include <QPainter>
#include <QPen>
#include <qdebug.h>
#include <QKeyEvent>
#include <QtGui>
#include <opencv\cv.h>
#include <boost/thread/locks.hpp>
#include "../framecapture.h"
#include "logger.h"
namespace gui
{

	using boost::shared_ptr;
	WebcamWidget::WebcamWidget(QWidget *parent, FrameCapturePtr existingCaptureDevice)
		: QWidget(parent)
	{
		if(existingCaptureDevice != 0)
			existingCaptureDevice->addFrameReceiver(this);
          previousTime = boost::posix_time::microsec_clock::local_time();
		QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		setSizePolicy(sizePolicy);
		setMinimumSize(640,480);
		}

	WebcamWidget::~WebcamWidget()
	{
	}

	QImage WebcamWidget::convertToQImage(const cv::Mat& mat) const 
	{
		IplImage cvimage = (IplImage)mat;
		return QImage((const uchar *)cvimage.imageData, cvimage.width, cvimage.height, QImage::Format_RGB888).rgbSwapped();
	}

	void WebcamWidget::paintEvent(QPaintEvent* event)
	{
		using cognition::DetailedFaceDetector;
		using cognition ::fisherfaceclassifer;
		loadNextFrameIntoCurrent();
		using cognition::DetailedFaceDetector;
		// start to detect the face and lock 
		// strat to extract the Detailed Faces
		/////// play moive //////////
		double gconfidence=0;
		facesLock.lock();
		cognition::Detector::RectVector facesDetected = faces; 
		DetailedFaceDetector::DetailedFaces detailedFacesDetected = detailedFaces;
		cognition ::fisherfaceclassifer cgender;
		facesLock.unlock();
		if(!currentFrame.empty())
		{
			QImage img = convertToQImage(currentFrame);

			QPainter painter(this);
			painter.drawImage(QPoint(0,0), img);
			
			QString fps;
			fps.setNum(detectionFramerate) += " fps";

			//y coordinate is baseline of font!
			painter.drawText(QPoint(6,15), fps);

			painter.setPen(QPen(Qt::darkMagenta,60));

			for(DetailedFaceDetector::DetailedFaces::iterator i = detailedFacesDetected.begin();
				i != detailedFacesDetected.end(); ++i)
			{
				QString rot;
				float rotation = DetailedFaceDetector::getFaceRotation(i->second);
				rot.setNum(rotation);
				painter.drawText(QPoint((i->first).x, (i->first).y), rot);
                    
				if(rotation > 8.0)
					Logger::getInstance().log(std::string("Rotation Right detected with angle: ") + rot.toStdString());
				else if(rotation < -8.0)
					Logger::getInstance().log(std::string("Rotation Left detected width angle: ") + rot.toStdString());
				QString Gender;
				//draw the face, stored in the first property of the map pair
				painter.setPen(QPen(Qt::darkMagenta, 5));
				painter.drawRect((i->first).x, (i->first).y, (i->first).width, (i->first).height);
				painter.setPen(QPen(Qt::yellow, 25));
				if(flag)
				{
			     VPlayer.close();
			    //qDebug() << "Each Face " << PairGender[p].second;
				if(WebGender==-1)
				{
				Logger::getInstance().log(std::string("Please remove your glasses and look straight to the Camera"));
				Gender="not recognized";

				//painter.drawText(QPoint(((i->first).x), (i->first).y+(i->first).height+15), Gender);
				}
				else if (WebGender==1)
				{
				Gender="Female";
				//Mat image = imread("animation/F_1.jpg", CV_LOAD_IMAGE_COLOR);   
				//imshow( "beautiful", image );
				//painter.drawText(QPoint(((i->first).x), (i->first).y+(i->first).height+15), Gender);
				/*QMovie *movie = new QMovie("qq.wmv");
				QLabel *processLabel = new QLabel(this);
				movie->setCacheMode(QMovie::CacheAll);
				processLabel->setMovie(movie);
				movie->start();*/
				}
				else
				{
			     Gender="Male";
				//Mat image = imread("animation/cool.jpg", CV_LOAD_IMAGE_COLOR);   
				//imshow( "Cool Man", image );
				/*QMovie *movie = new QMovie("animation/F_1.gif");
				QLabel *processLabel = new QLabel(this);
				processLabel->setMovie(movie);
				movie->start();*/
				}
				//////////////////////////Timer////////////////////////////
                   // int startTime = 1, endTime;
                    //endTime = 30* 3600;
                   // clock_t start = clock();
				//cout<< start << endl;
                    //if (start <= 20000)
                    {
                    painter.drawText(QPoint(((i->first).x), (i->first).y+(i->first).height+15), Gender);
			     //WebGender=cgender.classdetection(facesDetected,currentFrame,gconfidence);
				//gconfidence=gconfidence*10;
				//QString cconfidence=QString::number(gconfidence*10);
				QString cconfidence=QString::number(confidence*100);
				cconfidence="confidence= " +cconfidence + "%";
				painter.drawText(QPoint(((i->first).x), (i->first).y+(i->first).height+30), cconfidence);
				}
				/*else 
				{
                    painter.drawText(QPoint(((i->first).x), (i->first).y+(i->first).height+15), "Classifed as: "+Gender);
			     //WebGender=cgender.classdetection(facesDetected,currentFrame,gconfidence);
				//gconfidence=gconfidence*10;
				QString cconfidence=QString::number(gconfidence*100);
				//QString cconfidence=QString::number(0.0*10);
				cconfidence="confidence= " +cconfidence + "%";
				painter.drawText(QPoint(((i->first).x), (i->first).y+(i->first).height+30), cconfidence);
				Logger::getInstance().log(std::string("Click classify button to classify the next face"));
			     }*/
				}
				else 
				{
			     VPlayer.move(-30,-30);
				VPlayer.showNormal();
				}

				//draw the eyes
				painter.setPen(QPen(Qt::blue, 2));
				cv::Rect &leftEye = (i->second)[DetailedFaceDetector::LEFT_EYE];
				cv::Rect &rightEye = (i->second)[DetailedFaceDetector::RIGHT_EYE];
				painter.drawRect(leftEye.x, leftEye.y, leftEye.width, leftEye.height);
				painter.drawRect(rightEye.x, rightEye.y, rightEye.width, rightEye.height);		
			
				painter.setPen(QPen(Qt::cyan, 1));
				painter.drawEllipse(QPoint((leftEye.x+ leftEye.width/2), (leftEye.y+leftEye.height/2)),6,6);
                    painter.drawEllipse(QPoint((rightEye.x+ rightEye.width/2), (rightEye.y+rightEye.height/2)),6,6);
            

				//draw the nose
				painter.setPen(QPen(Qt::green, 3));
				cv::Rect &nose = (i->second)[DetailedFaceDetector::NOSE];
				painter.drawRect(nose.x, nose.y, nose.width, nose.height);

				//draw the mouth
				painter.setPen(QPen(Qt::yellow, 3));
				cv::Rect &mouth = (i->second)[DetailedFaceDetector::MOUTH];
				painter.drawRect(mouth.x, mouth.y, mouth.width, mouth.height);

				//draw the Profile
				painter.setPen(QPen(Qt::darkCyan, 3));
				cv::Rect &pface = (i->second)[DetailedFaceDetector::PFACE];
				painter.drawRect(pface.x, pface.y, pface.width, pface.height);
				
			}
		}
	
	}

	void WebcamWidget::stateChanged(cognition::Detector *detector)
	{
		boost::lock_guard<boost::mutex>(this->facesLock);
		//maybe move this outside critical section
		updateFramerate();
		faces = detector->getAreas();
		detailedFaces = static_cast<cognition::DetailedFaceDetector *>(detector)->getDetailedFaceInfo();
	}

	inline void WebcamWidget::updateFramerate()
	{
		//http://www.boost.org/doc/libs/1_45_0/doc/html/date_time/posix_time.html

		boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration duration = currentTime.time_of_day() - previousTime.time_of_day();
		if(duration.total_milliseconds() != 0)
			detectionFramerate = (10000 / duration.total_milliseconds());  /// kept changing the frame rate to make it slower
		previousTime = currentTime;
	}

	void WebcamWidget::receiveFrame(const cv::Mat &frame)
	{
		frameLoadLock.lock();

		nextFrame = frame.clone();
		
		frameLoadLock.unlock();
		//update causes a paintEvent, which calls loadNextFrameIntoCurrent
		//this could cause deadlock
		update();
	}

	cv::Mat WebcamWidget::getCurrentFrame()
	{
		boost::lock_guard<boost::mutex>(this->frameLoadLock);
		return currentFrame.clone();

	}

	cognition::Detector::RectVector WebcamWidget::getCurrentFaces()
	{
		boost::lock_guard<boost::mutex>(this->facesLock);
		return faces;
	}

	void WebcamWidget::loadNextFrameIntoCurrent()
	{
		boost::lock_guard<boost::mutex>(this->frameLoadLock);

		currentFrame = nextFrame;
	}
void WebcamWidget::keyPressEvent(QKeyEvent *event)  //QKeyEvent *e
{ 
// key press to quit the opengl window
qDebug () << " in key iterative Function " ;
  if(event->type() == QEvent::KeyPress)
{
      switch(event->key())
     {
		case Qt::Key_Escape:
		{
		close();
          qApp->quit();
		
		}
		case Qt::Key_Q:
		{
		qApp->quit();
		}
		
	 }
}
}


}