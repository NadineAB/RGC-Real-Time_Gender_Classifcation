#include "gui/genderclassification.h"
#include <QtGui/QApplication>
#if !defined VK_ESCAPE
    #define VK_ESCAPE 0x1B      // Escape character (27)
#endif
int main(int argc, char *argv[])
{
	// run the application
	QApplication a(argc, argv);
	gui::genderclassification w;
	w.move(700,100);
	w.show();
	return a.exec();
}
