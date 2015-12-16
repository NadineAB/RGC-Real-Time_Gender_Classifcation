#ifndef VIRTUAL_FACE
#define VIRTUAL_FACE
#include <QWidget>
#include <QtGui/Qwidget>
#include <QtCore>
#include <QtGui>
#include "ui_Virtual_Face.h"
#include "../fisherfaceclassifer.h"
#include <phonon/audiooutput.h>
 #include <phonon/seekslider.h>
 #include <phonon/mediaobject.h>
 #include <phonon/volumeslider.h>
 #include <phonon/backendcapabilities.h>

namespace gui
{

	/*! \brief:	Widget that displays the webcam feed, provided by a capture devices
	 *			it also draws the rectangles it recieves from detectors!
	 * 
	 */
class Virtual_Face :public QWidget
{
	Q_OBJECT
 	
   public:
		
          Virtual_Face(QWidget *parent = 0);
		//Virtual_Face();
		 ~Virtual_Face();
           Phonon::MediaObject *mediaObject;
		 Phonon::VideoPlayer *videoPlayer;
		QLabel *status_animation;
		QGridLayout *status_layout;
		std::vector <pair<cv::Mat,int>> PairGender;
	     void play(int sex);
		void finished();
	protected:
		Ui::VirtualFace media;
	
};
}

#endif 
