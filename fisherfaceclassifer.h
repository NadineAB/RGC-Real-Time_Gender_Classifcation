#ifndef COGNITION_RECOGNIZER_FISHERFACECLASSIFER_H
#define COGNITION_RECOGNIZER_FISHERFACECLASSIFER_H
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS 
#include <map>
#include <opencv\cv.h>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "FrameCapture.h" 
#include "recognizer.h"
#include "facedetector.h"
#include "trainerimage.h"
using namespace cv;
using namespace std;
namespace cognition
{
	class fisherfaceclassifer: public Recognizer
	{
	public:
		fisherfaceclassifer(void);
		virtual ~fisherfaceclassifer(void);
		int im_width;
          int im_height;
		static Ptr<FaceRecognizer> model;
         /*!
		 * \brief: Adds a training image path to the training set of known images
		 *		  after you have added 2 or more images, call train to learn and
		 *		  set yourself up for recognition. All images must be the same size!
		 *		  
		 * \note warning! method does not check if the filename exists and the application
		 *		  can crash if it does not exist! this check should be added (in train?)!
		 * 
		 * \param filename	the path where to find the image (all of the same size!)
		 * \param name		the name you want to attach to the image
		 * \return bool		true if the path is added succesfully
		 */
		// to read the csv that have all my images to use Fisherfaceclassifer
		void read_csv(const string& filename, char separator);
		/*!
		 * \brief: starts the learning process on all the known images that
		 *		  are added trough addTrainingImage. You can add more training images
		 *		  after training, but you have to call train again. While training
		 *		  you cannot use recognize()! 
		 *
		 * \note make sure all image paths exist
		 * 
		 * \return bool		true if trained, false otherwise
		 */
		bool train();
		int  classdetection(cognition::Detector::RectVector &faces,cv::Mat &frame,double &gconfidence);
		/*!
		 * \brief: does recognition on the face, and returns the most likely match. 
		 *		  This face must grayscale and be exactly the same size as the training
		 *		  images.
		 * 
		 * \param face		the matrix containing the face
		 * \return string	name of closest match in the set of training images
		 */
		double getSimilarity(const Mat A, const Mat B);
		Mat reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace);
		void showTrainingDebugData(const Ptr<FaceRecognizer> model, const int faceWidth, const int faceHeight);
////////////////////////////////////////////////////////////////////////////////////////////////////
		/*!
		 * \brief does recognition on the gender, and returns the most likely match. 
		 *		  This face must grayscale and be exactly the same size as the training
		 *		  images.
		 * 
		 * \return size_t	the number of registered training images
		 */
		
	protected:
		void freeMemory(int fromIndex = -1);
	     typedef std::map<std::string, std::string> StringMap;
		StringMap trainingImages;
		typedef std::pair<std::string, std::string> StringPair;
		std::vector<StringPair> recognitionDescriptor;
	     static  vector<Mat> csvimages;//array of trained images
		vector<int> labels;          // label for male or female
	};

}
#endif //COGNITION_RECOGNIZER_FISHERFACERECOGNIZER_H