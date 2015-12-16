#include <QString>
#include "genderclassification.h"
#include "webcamwidget.h"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <QDebug>
#include <QPainter>
#include <QImage>
#include <QKeyEvent>
#include <QtGui>
#include <QPen>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <logger.h>
#include "../facedetector.h"
#include "../trainerimage.h"
#include "fisherfaceclassifer.h"
#include <iostream>
#include <fstream>
#include <sstream>
namespace gui
{
	int sex;
     const cv::Size genderclassification::testImageSize(180,200);

	genderclassification::genderclassification(QWidget *parent, Qt::WFlags flags)
		: QMainWindow(parent, flags)
	{

		setupFramework();
		setupGUI();

		setMinimumSize(QSize(640,480));

		frameCaptureThread = boost::thread(
			boost::bind(&cognition::FrameCapture::startCapturing, frameCapture));

		faceDetectorThread = boost::thread(
			boost::bind(&cognition::Detector::threadStart, faceDetector));
         
		Logger::getInstance().log("Ready...");
		}
	genderclassification::~genderclassification()
	{
	}
     void genderclassification::setupFramework()
	{
		using boost::shared_ptr;
		using cognition::DetailedFaceDetector;
		using cognition ::fisherfaceclassifer;
          frameCapture = shared_ptr<cognition::FrameCapture>( new cognition::FrameCapture(24) );
		// update the path of the classifier to the local path of your openCV installation
		// detecting classifier
		// haarcascades/haarcascade_frontalface_default.xml
		// haarcascades/haarcascade_frontalface_alt.xml
		// lbpcascade_frontalface.xml  // most efficent so far
		//const char *eyeCascadeFilename1 = "haarcascades/haarcascade_eye.xml";// Basic eye detector for open eyes only.
          //const char *eyeCascadeFilename2 = "haarcascades/haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.

		faceDetector = shared_ptr<DetailedFaceDetector>( 
			new DetailedFaceDetector(DetailedFaceDetector::ALL, 
			"haarcascades/lbpcascade_frontalface.xml",
			frameCapture.get(), false, 1.16));
		faceDetector->loadCascade(DetailedFaceDetector::EYES, "haarcascades/haarcascade_eye_tree_eyeglasses.xml");
		faceDetector->loadCascade(DetailedFaceDetector::NOSE, "haarcascades/haarcascade_mcs_nose.xml");
		faceDetector->loadCascade(DetailedFaceDetector::MOUTH, "haarcascades/haarcascade_mcs_mouth.xml");
          frameCapture->addFrameReceiver(faceDetector.get());
		recognizer = shared_ptr<fisherfaceclassifer>( new cognition::fisherfaceclassifer );

	}
	void genderclassification::closeEvent(QCloseEvent *event)
	{
		faceDetector->removeController(webcamWidget);
		//qDebug() << "controller count = " << faceDetector->getControllerCount();
          frameCapture->stopCapturing();
          faceDetector->requestTreadStop();
		frameCaptureThread.interrupt();
          faceDetectorThread.interrupt();
		//qDebug() << "Close event called, joining";
		frameCaptureThread.join();
          faceDetectorThread.join();
		//event->accept();
		//QMainWindow::closeEvent(event);
	}

	void genderclassification::setupGUI()
	{
		webcamWidget = new WebcamWidget(this, frameCapture.get());
		webcamWidget->flag=0;
		faceDetector->addController(webcamWidget); 
		logWidget = new QListWidget;
		captureTrainingImageButton = new QPushButton(tr("Capture face and store it as training image"));
		connect(captureTrainingImageButton, SIGNAL(clicked()), this, SLOT(captureTrainingImage()));
		trainRecognizerButton = new QPushButton(tr("Train classifier "));
		connect(trainRecognizerButton, SIGNAL(clicked()), this, SLOT(trainRecognizer()));
		recognizeFaceButton = new QPushButton(tr("classifier recognize Visible Faces as Male or Female"));
		connect(recognizeFaceButton, SIGNAL(clicked()), this, SLOT(recognizeFaces()));
		QWidget *centralWidget = new QWidget;
		QVBoxLayout *mainLayout = new QVBoxLayout;
		QHBoxLayout *buttonsLayout = new QHBoxLayout;
          buttonsLayout->addWidget(captureTrainingImageButton);
		buttonsLayout->addWidget(trainRecognizerButton);
		buttonsLayout->addWidget(recognizeFaceButton);
		mainLayout->addWidget(webcamWidget);
		mainLayout->addLayout(buttonsLayout);
		mainLayout->addWidget(logWidget);
          centralWidget->setLayout(mainLayout);
          Logger::getInstance().setLogWidget(logWidget);
		setCentralWidget(centralWidget);
		
	}
// Capture Training Image
	void genderclassification::captureTrainingImage()
	{
          std::string file_name ="Train/TrainingExamples.csv";
          const char *sptr = file_name.c_str() ;
          FILE* trainfile; 
          trainfile = fopen(sptr, "a+");
  		cv::Mat frame;
		cognition::Detector::RectVector faces;
		captureFrameAndFaces(faces, frame);
		cognition::TrainerImage convertor;
          
		if(faces.size() == 0)
		{
			QMessageBox::information(this, "No faces found", "The detector did not find any faces!");
		} 
		else 
		{
			frame = frame.clone();
			cv::Mat faceRegion;
			for(std::vector<cv::Rect>::iterator face = faces.begin(); face != faces.end(); ++face)
			{  
				faceRegion = frame(*face);
				QString filename = QInputDialog::getText(this,
					tr("Image name"),
					tr("enter image name (enter extension too, it determines the image format please!)"));
				     QString Gender = QInputDialog::getText(this,
					tr("Gender type"),
					tr("enter the gender please, F for M"));
                         if(Gender=="F")
		               {
					sex=1;
					cognition::TrainerImage convertor(testImageSize, true, "Train/example/sfemale/women");
					convertor.processAndSaveImage(faceRegion, filename.toStdString(),"Train/example/sfemale/women/");
				     fprintf(trainfile, "Train/example/sfemale/women/%s",filename.toLocal8Bit().constData());
                         fprintf(trainfile, ";%d\n",sex);
    	                    fclose(trainfile);
					 }
	                     else if (Gender=="M")
		                {
					faceRegion = frame(*face);
					sex=0;
					cognition::TrainerImage convertor(testImageSize, true, "Train/example/male/men");
					convertor.processAndSaveImage(faceRegion, filename.toStdString(),"Train/example/male/men/");
					fprintf(trainfile, "Train/example/male/men/%s",filename.toLocal8Bit().constData());
                         fprintf(trainfile, ";%d\n",sex);
    	                    fclose(trainfile); 
					 }
				if(filename.size() < 1) continue;
				/*if(Gender=="F")
				{
				if(!convertor.processAndSaveImage(faceRegion, filename.toStdString(),"Train/example/sfemale/women/"))
				QMessageBox::information(this, "Failed", "Could not process and save the image!");
				}
				else if (Gender=="M")
		          {
				if(!convertor.processAndSaveImage(faceRegion, filename.toStdString(),"Train/example/male/men/"))
				QMessageBox::information(this, "Failed", "Could not process and save the image!");
				}*/
				if (!trainfile) 
	                {	
                      fprintf(stderr, "failed: can't open file \"%s\" to write.\n",
                      trainfile);
                      exit(1);
                  }
			   }
	             }
	             } 
     // test image that i will apply the classify on it
     void genderclassification::trainRecognizer()
{ 
	for(int i=0;i<3;i++)
	{
		recognizer->train();
	}
	if(recognizer->train())
	{
	QMessageBox::information(this, "Success", "The Classifier has succesfully finished training!");
	}
	else
	QMessageBox::information(this, "Error", "The Classifier  has indicated that it did not train correctly!");

}

///////////////////////////////////////////////////////////////////////////////////
     void genderclassification::recognizeFaces()
{

	cv::Mat frame;
			cognition::Detector::RectVector faces;
			captureFrameAndFaces(faces, frame);
		     frame = frame.clone();
			cognition::TrainerImage convertor(testImageSize, true);
			cv::Mat faceRegion;
			cv::Mat processedFace;
			double gconfidence;
			if(recognizer->isTrained)
		     {
		     int Gender=recognizer->classdetection(faces, frame,gconfidence);
			
				for(std::vector<cv::Rect>::iterator face = faces.begin(); face != faces.end(); ++face)
				{
					faceRegion = frame(*face);
					processedFace = convertor.processImage(faceRegion);
				}
			webcamWidget->flag=1;
			webcamWidget->WebGender=Gender;
			VPlayer.move(-30,-30);
			VPlayer.show();
			VPlayer.play(Gender);
			webcamWidget->confidence=gconfidence;
			
			
			}
			else
				QMessageBox::information(this, "Error", "The Classifier  has indicated that it did not train correctly, please train the classifier!");
}
//////////////////////////////////////////////////////////////////////////////////////////////////
     void genderclassification::captureFrameAndFaces(cognition::Detector::RectVector &rects, cv::Mat &frame)
	{
		rects = webcamWidget->getCurrentFaces();
		frame = webcamWidget->getCurrentFrame();
		
	}
void genderclassification::keyPressEvent(QKeyEvent *event)  //QKeyEvent *e
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
		close();
		qApp->quit();
		}
		
	 }
  }
}
}