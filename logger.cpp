#include "Logger.h"
#include <QListWidget>

Logger Logger::instance;

Logger::Logger(void)
	:logWidget(0)
{
}
void Logger::log(const std::string &msg)
{
	if(logWidget != 0)
		logWidget->insertItem(0,QString(msg.c_str()));
     if(GenderType== 0)
	{
	}	// do nothing 
	else if(GenderType== 1) 
	{
	logWidget->insertItem(0,QString("Male!!!!!!"));
	}
     else if(GenderType== 2) 
	{
	logWidget->insertItem(0,QString("Female!!!!!!"));
	}
}

Logger& Logger::getInstance()
{
	return Logger::instance;
}

void Logger::setLogWidget(QListWidget *logWidget)
{
	this->logWidget = logWidget;
}
