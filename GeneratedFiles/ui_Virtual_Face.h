/********************************************************************************
** Form generated from reading UI file 'Virtual_Face.ui'
**
** Created: Wed 28. Aug 16:25:57 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIRTUAL_FACE_H
#define UI_VIRTUAL_FACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include <phonon/videoplayer.h>

QT_BEGIN_NAMESPACE

class Ui_VirtualFace
{
public:
    Phonon::VideoPlayer *videoPlayer;

    void setupUi(QWidget *VirtualFace)
    {
        if (VirtualFace->objectName().isEmpty())
            VirtualFace->setObjectName(QString::fromUtf8("VirtualFace"));
        VirtualFace->resize(656, 499);
        videoPlayer = new Phonon::VideoPlayer(VirtualFace);
        videoPlayer->setObjectName(QString::fromUtf8("videoPlayer"));
        videoPlayer->setGeometry(QRect(10, 10, 640, 480));

        retranslateUi(VirtualFace);

        QMetaObject::connectSlotsByName(VirtualFace);
    } // setupUi

    void retranslateUi(QWidget *VirtualFace)
    {
        VirtualFace->setWindowTitle(QApplication::translate("VirtualFace", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VirtualFace: public Ui_VirtualFace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIRTUAL_FACE_H
