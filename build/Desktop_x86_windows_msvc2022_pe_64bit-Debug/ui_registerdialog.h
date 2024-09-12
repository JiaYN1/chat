/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *user_label;
    QLineEdit *user_edit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *email_label;
    QLineEdit *email_edit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *pass_label;
    QLineEdit *pass_edit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *pass_label_2;
    QLineEdit *pass_edit_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *verify_label;
    QLineEdit *verify_edit;
    QSpacerItem *horizontalSpacer;
    QPushButton *get_btn;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *cofirm_btn;
    QPushButton *cancel_btn;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(300, 500);
        RegisterDialog->setMinimumSize(QSize(300, 500));
        RegisterDialog->setMaximumSize(QSize(300, 500));
        verticalLayoutWidget = new QWidget(RegisterDialog);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(9, 9, 281, 281));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        user_label = new QLabel(verticalLayoutWidget);
        user_label->setObjectName("user_label");
        user_label->setMinimumSize(QSize(60, 25));
        user_label->setMaximumSize(QSize(60, 25));
        user_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(user_label);

        user_edit = new QLineEdit(verticalLayoutWidget);
        user_edit->setObjectName("user_edit");
        user_edit->setMinimumSize(QSize(0, 25));
        user_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(user_edit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        email_label = new QLabel(verticalLayoutWidget);
        email_label->setObjectName("email_label");
        email_label->setMinimumSize(QSize(60, 25));
        email_label->setMaximumSize(QSize(60, 25));
        email_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(email_label);

        email_edit = new QLineEdit(verticalLayoutWidget);
        email_edit->setObjectName("email_edit");
        email_edit->setMinimumSize(QSize(0, 25));
        email_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_2->addWidget(email_edit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        pass_label = new QLabel(verticalLayoutWidget);
        pass_label->setObjectName("pass_label");
        pass_label->setMinimumSize(QSize(60, 25));
        pass_label->setMaximumSize(QSize(60, 25));
        pass_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_3->addWidget(pass_label);

        pass_edit = new QLineEdit(verticalLayoutWidget);
        pass_edit->setObjectName("pass_edit");
        pass_edit->setMinimumSize(QSize(0, 25));
        pass_edit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_3->addWidget(pass_edit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        pass_label_2 = new QLabel(verticalLayoutWidget);
        pass_label_2->setObjectName("pass_label_2");
        pass_label_2->setMinimumSize(QSize(60, 25));
        pass_label_2->setMaximumSize(QSize(60, 25));
        pass_label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_4->addWidget(pass_label_2);

        pass_edit_2 = new QLineEdit(verticalLayoutWidget);
        pass_edit_2->setObjectName("pass_edit_2");
        pass_edit_2->setMinimumSize(QSize(0, 25));
        pass_edit_2->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_4->addWidget(pass_edit_2);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        verify_label = new QLabel(verticalLayoutWidget);
        verify_label->setObjectName("verify_label");
        verify_label->setMinimumSize(QSize(60, 25));
        verify_label->setMaximumSize(QSize(60, 25));
        verify_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_5->addWidget(verify_label);

        verify_edit = new QLineEdit(verticalLayoutWidget);
        verify_edit->setObjectName("verify_edit");
        verify_edit->setMinimumSize(QSize(120, 25));
        verify_edit->setMaximumSize(QSize(120, 25));

        horizontalLayout_5->addWidget(verify_edit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        get_btn = new QPushButton(verticalLayoutWidget);
        get_btn->setObjectName("get_btn");
        get_btn->setMinimumSize(QSize(60, 25));
        get_btn->setMaximumSize(QSize(60, 25));

        horizontalLayout_5->addWidget(get_btn);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        cofirm_btn = new QPushButton(verticalLayoutWidget);
        cofirm_btn->setObjectName("cofirm_btn");
        cofirm_btn->setMinimumSize(QSize(100, 30));
        cofirm_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout_6->addWidget(cofirm_btn);

        cancel_btn = new QPushButton(verticalLayoutWidget);
        cancel_btn->setObjectName("cancel_btn");
        cancel_btn->setMinimumSize(QSize(100, 30));
        cancel_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout_6->addWidget(cancel_btn);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        user_label->setText(QCoreApplication::translate("RegisterDialog", "\347\224\250    \346\210\267\357\274\232", nullptr));
        email_label->setText(QCoreApplication::translate("RegisterDialog", "\351\202\256    \347\256\261\357\274\232", nullptr));
        pass_label->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206    \347\240\201\357\274\232", nullptr));
        pass_label_2->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        verify_label->setText(QCoreApplication::translate("RegisterDialog", "\351\252\214 \350\257\201 \347\240\201\357\274\232", nullptr));
        get_btn->setText(QCoreApplication::translate("RegisterDialog", "\350\216\267\345\217\226", nullptr));
        cofirm_btn->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244", nullptr));
        cancel_btn->setText(QCoreApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
