/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *vl2;
    QLabel *logo_label;
    QSpacerItem *verticalSpacer_5;
    QLabel *label;
    QSpacerItem *verticalSpacer_4;
    QLineEdit *username;
    QSpacerItem *verticalSpacer;
    QLineEdit *passwd;
    QSpacerItem *verticalSpacer_2;
    QPushButton *logon;
    QSpacerItem *verticalSpacer_3;
    QPushButton *showLog;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(230, 80, 321, 371));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        verticalLayoutWidget_2 = new QWidget(widget);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(50, 14, 231, 341));
        vl2 = new QVBoxLayout(verticalLayoutWidget_2);
        vl2->setObjectName("vl2");
        vl2->setSizeConstraint(QLayout::SetDefaultConstraint);
        vl2->setContentsMargins(0, 0, 0, 0);
        logo_label = new QLabel(verticalLayoutWidget_2);
        logo_label->setObjectName("logo_label");

        vl2->addWidget(logo_label);

        verticalSpacer_5 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        vl2->addItem(verticalSpacer_5);

        label = new QLabel(verticalLayoutWidget_2);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

        vl2->addWidget(label);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        vl2->addItem(verticalSpacer_4);

        username = new QLineEdit(verticalLayoutWidget_2);
        username->setObjectName("username");
        username->setFocusPolicy(Qt::WheelFocus);
        username->setMaxLength(150);

        vl2->addWidget(username);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        vl2->addItem(verticalSpacer);

        passwd = new QLineEdit(verticalLayoutWidget_2);
        passwd->setObjectName("passwd");
        passwd->setFocusPolicy(Qt::WheelFocus);
        passwd->setMaxLength(100);
        passwd->setEchoMode(QLineEdit::Password);

        vl2->addWidget(passwd);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        vl2->addItem(verticalSpacer_2);

        logon = new QPushButton(verticalLayoutWidget_2);
        logon->setObjectName("logon");

        vl2->addWidget(logon);

        verticalSpacer_3 = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        vl2->addItem(verticalSpacer_3);

        showLog = new QPushButton(verticalLayoutWidget_2);
        showLog->setObjectName("showLog");

        vl2->addWidget(showLog);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(0, 470, 801, 71));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 37));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        logo_label->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "SRP6-Logon", nullptr));
        username->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215\346\210\226\347\224\265\345\255\220\351\202\256\344\273\266", nullptr));
        passwd->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        logon->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
        showLog->setText(QCoreApplication::translate("MainWindow", "\346\230\276\347\244\272\351\232\220\350\227\217\346\227\245\345\277\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
