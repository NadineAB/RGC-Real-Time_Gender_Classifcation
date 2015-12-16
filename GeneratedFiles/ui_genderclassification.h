/********************************************************************************
** Form generated from reading UI file 'genderclassification.ui'
**
** Created: Wed 28. Aug 16:25:41 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENDERCLASSIFICATION_H
#define UI_GENDERCLASSIFICATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_genderclassification
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *genderclassification)
    {
        if (genderclassification->objectName().isEmpty())
            genderclassification->setObjectName(QString::fromUtf8("genderclassification"));
        genderclassification->resize(944, 680);
        centralWidget = new QWidget(genderclassification);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMaximumSize(QSize(1000, 1000));
        genderclassification->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(genderclassification);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 944, 26));
        genderclassification->setMenuBar(menuBar);
        mainToolBar = new QToolBar(genderclassification);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        genderclassification->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(genderclassification);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        genderclassification->setStatusBar(statusBar);

        retranslateUi(genderclassification);

        QMetaObject::connectSlotsByName(genderclassification);
    } // setupUi

    void retranslateUi(QMainWindow *genderclassification)
    {
        genderclassification->setWindowTitle(QApplication::translate("genderclassification", "Gender_Classification", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class genderclassification: public Ui_genderclassification {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENDERCLASSIFICATION_H
