/********************************************************************************
** Form generated from reading UI file 'inpaint.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPAINT_H
#define UI_INPAINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_inpaintClass
{
public:
    QWidget *centralWidget;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QLabel *label;
    QPushButton *inpaintButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *inpaintClass)
    {
        if (inpaintClass->objectName().isEmpty())
            inpaintClass->setObjectName(QStringLiteral("inpaintClass"));
        inpaintClass->resize(600, 400);
        centralWidget = new QWidget(inpaintClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        loadButton = new QPushButton(centralWidget);
        loadButton->setObjectName(QStringLiteral("loadButton"));
        loadButton->setGeometry(QRect(480, 10, 75, 31));
        saveButton = new QPushButton(centralWidget);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(480, 160, 75, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 431, 331));
        label->setMaximumSize(QSize(431, 16777215));
        label->setStyleSheet(QStringLiteral(""));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        inpaintButton = new QPushButton(centralWidget);
        inpaintButton->setObjectName(QStringLiteral("inpaintButton"));
        inpaintButton->setGeometry(QRect(480, 90, 75, 31));
        inpaintClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(inpaintClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        inpaintClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(inpaintClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        inpaintClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(inpaintClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        inpaintClass->setStatusBar(statusBar);

        retranslateUi(inpaintClass);

        QMetaObject::connectSlotsByName(inpaintClass);
    } // setupUi

    void retranslateUi(QMainWindow *inpaintClass)
    {
        inpaintClass->setWindowTitle(QApplication::translate("inpaintClass", "inpaint", 0));
        loadButton->setText(QApplication::translate("inpaintClass", "\350\275\275\345\205\245\345\233\276\347\211\207", 0));
        saveButton->setText(QApplication::translate("inpaintClass", "\344\277\235\345\255\230\345\233\276\347\211\207", 0));
        label->setText(QString());
        inpaintButton->setText(QApplication::translate("inpaintClass", "inpaint", 0));
    } // retranslateUi

};

namespace Ui {
    class inpaintClass: public Ui_inpaintClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPAINT_H
