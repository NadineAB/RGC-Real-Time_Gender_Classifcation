#ifndef VC_LOGGER_H
#define VC_LOGGER_H
#include <string>
/*Log list show all head rotation and direction*/
class QListWidget;

class Logger
{
public:
	int GenderType;
	void log(const std::string &msg);
     static Logger& getInstance();
     void setLogWidget(QListWidget *logWidget);
protected:
	Logger(); // protected constructor
private:
	QListWidget *logWidget;
	static Logger instance;
};
#endif //VC_LOGGER_H