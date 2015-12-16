#ifndef COGNITION_RECOGNIZER_H
#define COGNITION_RECOGNIZER_H

#include <string>
#include "FrameCapture.h" 
#include "recognizer.h"
#include "facedetector.h"
#include "trainerimage.h"
namespace cv {
	class Mat;
}
namespace cognition
{

	/*!
	 * \brief: Base class for Classifier. Provides the interface  
	 *		  for different kinds of recognizers.
	 */
	class Recognizer
	{
	public:

		Recognizer()
			:isTrained(false){}
		
		/*!
		 * \brief: Adds a training image path to the training set of known images gender
		 *		  after you have added 2 or more images, call train to learn and
		 *		  set yourself up for recognition. All images must be the same size!
		 * 
		 * \param filename	the path where to find the image (all of the same size!)
		 * \param name		the name you want to attach to the image
		 * \return bool		true if the path is added succesfully
		 */
	
		/*!
		 * \brief: starts the learning process on all the known images that
		 *		  are added trough addTrainingImage. You can add more training images
		 *		  after training, but you have to call train again. While training
		 *		  you cannot use recognize()!
		 * 
		 * \return bool		true if trained and ready, false otherwise
		 */
		virtual bool train() = 0;
		int classdetection( cognition::Detector::RectVector &faces,cv::Mat &frame);

		/*!
		 * \brief: does recognition on the face, and returns the most likely match. 
		 *		  This face must grayscale and be exactly the same size as the training
		 *		  images.
		 * 
		 * \param face		the matrix containing the face
		 * \return string	name of closest match in the set of training images
		 */
		
		/*!
		 * \brief: gets the number of training images available
		 * 
		 * \return size_t	the number of registered training images
		 */
		bool trained() { return isTrained; }

	//protected:
		bool isTrained;
	};
}
#endif //COGNITION_RECOGNIZER_H