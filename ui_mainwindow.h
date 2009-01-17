/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Thu Jan 1 21:16:14 2009
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>
#include "analogpad.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    AnalogPad *analogPad2;
    AnalogPad *analogPad3;
    AnalogPad *analogPad1;
    QWebView *webView;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QMainWindow *MainWindowClass)
    {
    if (MainWindowClass->objectName().isEmpty())
        MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
    MainWindowClass->resize(466, 377);
    centralWidget = new QWidget(MainWindowClass);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(-1, 9, -1, -1);
    analogPad2 = new AnalogPad(centralWidget);
    analogPad2->setObjectName(QString::fromUtf8("analogPad2"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(analogPad2->sizePolicy().hasHeightForWidth());
    analogPad2->setSizePolicy(sizePolicy1);
    analogPad2->setMinimumSize(QSize(150, 150));

    gridLayout->addWidget(analogPad2, 1, 1, 3, 1);

    analogPad3 = new AnalogPad(centralWidget);
    analogPad3->setObjectName(QString::fromUtf8("analogPad3"));
    sizePolicy1.setHeightForWidth(analogPad3->sizePolicy().hasHeightForWidth());
    analogPad3->setSizePolicy(sizePolicy1);
    analogPad3->setMinimumSize(QSize(100, 100));

    gridLayout->addWidget(analogPad3, 1, 2, 2, 1);

    analogPad1 = new AnalogPad(centralWidget);
    analogPad1->setObjectName(QString::fromUtf8("analogPad1"));
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(analogPad1->sizePolicy().hasHeightForWidth());
    analogPad1->setSizePolicy(sizePolicy2);
    analogPad1->setMinimumSize(QSize(100, 100));
    analogPad1->setBaseSize(QSize(0, 0));

    gridLayout->addWidget(analogPad1, 1, 0, 1, 1);

    webView = new QWebView(centralWidget);
    webView->setObjectName(QString::fromUtf8("webView"));
    sizePolicy2.setHeightForWidth(webView->sizePolicy().hasHeightForWidth());
    webView->setSizePolicy(sizePolicy2);
    webView->setMinimumSize(QSize(200, 200));
    webView->setMaximumSize(QSize(16777215, 200));
    webView->setBaseSize(QSize(0, 0));
    webView->setUrl(QUrl("http://www.google.fi/"));

    gridLayout->addWidget(webView, 0, 0, 1, 3);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer, 2, 0, 2, 1);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer_2, 3, 2, 1, 1);

    MainWindowClass->setCentralWidget(centralWidget);

    retranslateUi(MainWindowClass);

    QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
    MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "AnalogPad demo", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MainWindowClass);
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
