#pragma warning(push)
#pragma warning ( disable : 4005 )
#include "Virtual_Face.h"
#include <QMessageBox>
bool rselect_v=0;
bool rselect_v2=0;
namespace gui
{
Virtual_Face::Virtual_Face(QWidget *parent)
{
     
	media.setupUi(this); // consrtuctor to class
     media.videoPlayer->load(Phonon::MediaSource(QString("animation/nothing1.avi"))); 
	media.videoPlayer->play();
	//connect(videoPlayer, SIGNAL(finished()), SLOT(finished()));
	
}
Virtual_Face::~Virtual_Face()
{
 // deconsrtuct 
}
void Virtual_Face:: finished()
{
	media.videoPlayer->play();
}
void Virtual_Face::play(int sex)
{
	 
	if (sex==1)
	{
	  media.videoPlayer->load(Phonon::MediaSource(QString("animation/F.avi"))); 
	  media.videoPlayer->play();
	  media.videoPlayer->show();
	}
	else if (sex==0)
	{
       media.videoPlayer->load(Phonon::MediaSource(QString("animation/M.avi"))); 
	  media.videoPlayer->play();
	  media.videoPlayer->show();
	}
 
}
}