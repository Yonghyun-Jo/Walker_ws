/********************************************************************************
** Form generated from reading UI file 'p73.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_P73_H
#define UI_P73_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *verticalLayoutWidget_45;
    QVBoxLayout *verticalLayout_39;
    QPushButton *safeRstBtn;
    QPushButton *initYawBtn;
    QPushButton *stateEstimateBtn;
    QPushButton *posCtrlBtn;
    QPushButton *gravCompBtn;
    QComboBox *taskMode;
    QPushButton *taskSendBtn;
    QLabel *label_2;
    QLabel *label_40;
    QFrame *frame_3;
    QWidget *formLayoutWidget_5;
    QFormLayout *formLayout_5;
    QLabel *label_29;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_3;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QFrame *frame_5;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout;
    QPushButton *initPoseBtn;
    QPushButton *zeroPoseBtn;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_42;
    QDoubleSpinBox *trajTime;
    QPushButton *jointSendBtn;
    QLabel *label_58;
    QFrame *frame_8;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_17;
    QLabel *label_68;
    QLabel *label_69;
    QLabel *label_70;
    QLabel *label_71;
    QLabel *label_72;
    QLabel *label_73;
    QVBoxLayout *position_layout_4;
    QVBoxLayout *velocity_layout_4;
    QVBoxLayout *torque_layout_4;
    QFrame *frame_6;
    QWidget *layoutWidget_6;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_91;
    QVBoxLayout *position_layout_7;
    QVBoxLayout *velocity_layout_7;
    QVBoxLayout *torque_layout_7;
    QFrame *frame_9;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_44;
    QLabel *label_45;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *label_48;
    QLabel *label_49;
    QVBoxLayout *position_layout;
    QVBoxLayout *velocity_layout;
    QVBoxLayout *torque_layout;
    QLabel *label_15;
    QLabel *label;
    QPushButton *torqueon_button;
    QPushButton *torqueoff_button;
    QPushButton *emergencyoff_button;
    QFrame *frame_4;
    QLabel *label_37;
    QLabel *label_38;
    QWidget *verticalLayoutWidget_11;
    QVBoxLayout *leftleg_layout;
    QWidget *verticalLayoutWidget_17;
    QVBoxLayout *rightleg_layout;
    QWidget *verticalLayoutWidget_19;
    QVBoxLayout *waist_layout;
    QWidget *verticalLayoutWidget_39;
    QVBoxLayout *waist_safety;
    QWidget *verticalLayoutWidget_40;
    QVBoxLayout *rightleg_safety;
    QWidget *verticalLayoutWidget_41;
    QVBoxLayout *leftleg_safety;
    QWidget *verticalLayoutWidget_55;
    QVBoxLayout *waist_elmo;
    QWidget *verticalLayoutWidget_57;
    QVBoxLayout *rightleg_elmo;
    QWidget *verticalLayoutWidget_58;
    QVBoxLayout *leftleg_elmo;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_136;
    QLabel *label_sestatus;
    QLabel *label_137;
    QLabel *label_tcstatus;
    QLabel *label_32;
    QLabel *label_imustatus;
    QLabel *label_34;
    QLabel *label_ecatstatus;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_41;
    QLabel *currenttime;
    QFrame *frame_2;
    QLabel *label_35;
    QLabel *label_43;
    QLabel *label_50;
    QLabel *label_51;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *imuQuaternion;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *imuAngVel;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *imuLinAcc;
    QFrame *frame_17;
    QLabel *label_52;
    QLabel *label_53;
    QLabel *label_54;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *comPos;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *linVel;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1533, 809);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/:/icons/p73_cute.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 801, 761));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayoutWidget_45 = new QWidget(tab);
        verticalLayoutWidget_45->setObjectName(QString::fromUtf8("verticalLayoutWidget_45"));
        verticalLayoutWidget_45->setGeometry(QRect(620, 30, 161, 171));
        verticalLayout_39 = new QVBoxLayout(verticalLayoutWidget_45);
        verticalLayout_39->setObjectName(QString::fromUtf8("verticalLayout_39"));
        verticalLayout_39->setContentsMargins(0, 0, 0, 0);
        safeRstBtn = new QPushButton(verticalLayoutWidget_45);
        safeRstBtn->setObjectName(QString::fromUtf8("safeRstBtn"));

        verticalLayout_39->addWidget(safeRstBtn);

        initYawBtn = new QPushButton(verticalLayoutWidget_45);
        initYawBtn->setObjectName(QString::fromUtf8("initYawBtn"));

        verticalLayout_39->addWidget(initYawBtn);

        stateEstimateBtn = new QPushButton(verticalLayoutWidget_45);
        stateEstimateBtn->setObjectName(QString::fromUtf8("stateEstimateBtn"));

        verticalLayout_39->addWidget(stateEstimateBtn);

        posCtrlBtn = new QPushButton(verticalLayoutWidget_45);
        posCtrlBtn->setObjectName(QString::fromUtf8("posCtrlBtn"));

        verticalLayout_39->addWidget(posCtrlBtn);

        gravCompBtn = new QPushButton(verticalLayoutWidget_45);
        gravCompBtn->setObjectName(QString::fromUtf8("gravCompBtn"));

        verticalLayout_39->addWidget(gravCompBtn);

        taskMode = new QComboBox(tab);
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->addItem(QString());
        taskMode->setObjectName(QString::fromUtf8("taskMode"));
        taskMode->setGeometry(QRect(620, 210, 161, 27));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(taskMode->sizePolicy().hasHeightForWidth());
        taskMode->setSizePolicy(sizePolicy);
        taskMode->setMaximumSize(QSize(187, 27));
        taskSendBtn = new QPushButton(tab);
        taskSendBtn->setObjectName(QString::fromUtf8("taskSendBtn"));
        taskSendBtn->setGeometry(QRect(620, 240, 161, 41));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(130, 0, 171, 31));
        label_40 = new QLabel(tab);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(460, 0, 111, 31));
        frame_3 = new QFrame(tab);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(0, 30, 411, 271));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        formLayoutWidget_5 = new QWidget(frame_3);
        formLayoutWidget_5->setObjectName(QString::fromUtf8("formLayoutWidget_5"));
        formLayoutWidget_5->setGeometry(QRect(10, 10, 191, 31));
        formLayout_5 = new QFormLayout(formLayoutWidget_5);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_5->setHorizontalSpacing(40);
        formLayout_5->setVerticalSpacing(10);
        formLayout_5->setContentsMargins(0, 0, 0, 0);
        label_29 = new QLabel(formLayoutWidget_5);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_29);

        formLayoutWidget = new QWidget(frame_3);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 50, 191, 211));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setHorizontalSpacing(27);
        formLayout->setVerticalSpacing(10);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_4);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_6);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_7);

        label_8 = new QLabel(formLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_8);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_3);

        formLayoutWidget_2 = new QWidget(frame_3);
        formLayoutWidget_2->setObjectName(QString::fromUtf8("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(210, 50, 191, 211));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_2->setHorizontalSpacing(22);
        formLayout_2->setVerticalSpacing(10);
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(formLayoutWidget_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_9);

        label_10 = new QLabel(formLayoutWidget_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_10);

        label_11 = new QLabel(formLayoutWidget_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_11);

        label_12 = new QLabel(formLayoutWidget_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_12);

        label_13 = new QLabel(formLayoutWidget_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_13);

        label_14 = new QLabel(formLayoutWidget_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_14);

        frame_5 = new QFrame(tab);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(410, 30, 201, 211));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayoutWidget_2 = new QWidget(frame_5);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 10, 181, 92));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        initPoseBtn = new QPushButton(verticalLayoutWidget_2);
        initPoseBtn->setObjectName(QString::fromUtf8("initPoseBtn"));
        initPoseBtn->setIconSize(QSize(16, 16));

        verticalLayout->addWidget(initPoseBtn);

        zeroPoseBtn = new QPushButton(verticalLayoutWidget_2);
        zeroPoseBtn->setObjectName(QString::fromUtf8("zeroPoseBtn"));

        verticalLayout->addWidget(zeroPoseBtn);

        horizontalLayoutWidget = new QWidget(frame_5);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 120, 181, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_42 = new QLabel(horizontalLayoutWidget);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        horizontalLayout->addWidget(label_42);

        trajTime = new QDoubleSpinBox(horizontalLayoutWidget);
        trajTime->setObjectName(QString::fromUtf8("trajTime"));
        trajTime->setDecimals(1);
        trajTime->setMaximum(10.000000000000000);
        trajTime->setSingleStep(0.100000000000000);
        trajTime->setValue(2.000000000000000);

        horizontalLayout->addWidget(trajTime);

        jointSendBtn = new QPushButton(frame_5);
        jointSendBtn->setObjectName(QString::fromUtf8("jointSendBtn"));
        jointSendBtn->setGeometry(QRect(10, 160, 181, 41));
        label_58 = new QLabel(tab);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setGeometry(QRect(660, 0, 81, 31));
        frame_8 = new QFrame(tab);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setGeometry(QRect(410, 400, 381, 301));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        layoutWidget_3 = new QWidget(frame_8);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(0, 0, 381, 301));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        verticalLayout_17->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_68 = new QLabel(layoutWidget_3);
        label_68->setObjectName(QString::fromUtf8("label_68"));

        verticalLayout_17->addWidget(label_68);

        label_69 = new QLabel(layoutWidget_3);
        label_69->setObjectName(QString::fromUtf8("label_69"));

        verticalLayout_17->addWidget(label_69);

        label_70 = new QLabel(layoutWidget_3);
        label_70->setObjectName(QString::fromUtf8("label_70"));

        verticalLayout_17->addWidget(label_70);

        label_71 = new QLabel(layoutWidget_3);
        label_71->setObjectName(QString::fromUtf8("label_71"));

        verticalLayout_17->addWidget(label_71);

        label_72 = new QLabel(layoutWidget_3);
        label_72->setObjectName(QString::fromUtf8("label_72"));

        verticalLayout_17->addWidget(label_72);

        label_73 = new QLabel(layoutWidget_3);
        label_73->setObjectName(QString::fromUtf8("label_73"));

        verticalLayout_17->addWidget(label_73);

        verticalLayout_17->setStretch(0, 1);
        verticalLayout_17->setStretch(1, 1);
        verticalLayout_17->setStretch(2, 1);
        verticalLayout_17->setStretch(3, 1);
        verticalLayout_17->setStretch(4, 1);
        verticalLayout_17->setStretch(5, 1);

        horizontalLayout_5->addLayout(verticalLayout_17);

        position_layout_4 = new QVBoxLayout();
        position_layout_4->setObjectName(QString::fromUtf8("position_layout_4"));
        position_layout_4->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_5->addLayout(position_layout_4);

        velocity_layout_4 = new QVBoxLayout();
        velocity_layout_4->setObjectName(QString::fromUtf8("velocity_layout_4"));
        velocity_layout_4->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_5->addLayout(velocity_layout_4);

        torque_layout_4 = new QVBoxLayout();
        torque_layout_4->setObjectName(QString::fromUtf8("torque_layout_4"));
        torque_layout_4->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_5->addLayout(torque_layout_4);

        frame_6 = new QFrame(tab);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(20, 350, 381, 41));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        layoutWidget_6 = new QWidget(frame_6);
        layoutWidget_6->setObjectName(QString::fromUtf8("layoutWidget_6"));
        layoutWidget_6->setGeometry(QRect(0, 0, 381, 41));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget_6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        verticalLayout_20->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_91 = new QLabel(layoutWidget_6);
        label_91->setObjectName(QString::fromUtf8("label_91"));

        verticalLayout_20->addWidget(label_91);

        verticalLayout_20->setStretch(0, 1);

        horizontalLayout_8->addLayout(verticalLayout_20);

        position_layout_7 = new QVBoxLayout();
        position_layout_7->setObjectName(QString::fromUtf8("position_layout_7"));
        position_layout_7->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_8->addLayout(position_layout_7);

        velocity_layout_7 = new QVBoxLayout();
        velocity_layout_7->setObjectName(QString::fromUtf8("velocity_layout_7"));
        velocity_layout_7->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_8->addLayout(velocity_layout_7);

        torque_layout_7 = new QVBoxLayout();
        torque_layout_7->setObjectName(QString::fromUtf8("torque_layout_7"));
        torque_layout_7->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_8->addLayout(torque_layout_7);

        frame_9 = new QFrame(tab);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setGeometry(QRect(20, 400, 381, 301));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        layoutWidget_2 = new QWidget(frame_9);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(0, 0, 381, 301));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        verticalLayout_14->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_44 = new QLabel(layoutWidget_2);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        verticalLayout_14->addWidget(label_44);

        label_45 = new QLabel(layoutWidget_2);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        verticalLayout_14->addWidget(label_45);

        label_46 = new QLabel(layoutWidget_2);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        verticalLayout_14->addWidget(label_46);

        label_47 = new QLabel(layoutWidget_2);
        label_47->setObjectName(QString::fromUtf8("label_47"));

        verticalLayout_14->addWidget(label_47);

        label_48 = new QLabel(layoutWidget_2);
        label_48->setObjectName(QString::fromUtf8("label_48"));

        verticalLayout_14->addWidget(label_48);

        label_49 = new QLabel(layoutWidget_2);
        label_49->setObjectName(QString::fromUtf8("label_49"));

        verticalLayout_14->addWidget(label_49);

        verticalLayout_14->setStretch(0, 1);
        verticalLayout_14->setStretch(1, 1);
        verticalLayout_14->setStretch(2, 1);
        verticalLayout_14->setStretch(3, 1);
        verticalLayout_14->setStretch(4, 1);
        verticalLayout_14->setStretch(5, 1);

        horizontalLayout_2->addLayout(verticalLayout_14);

        position_layout = new QVBoxLayout();
        position_layout->setObjectName(QString::fromUtf8("position_layout"));
        position_layout->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_2->addLayout(position_layout);

        velocity_layout = new QVBoxLayout();
        velocity_layout->setObjectName(QString::fromUtf8("velocity_layout"));
        velocity_layout->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_2->addLayout(velocity_layout);

        torque_layout = new QVBoxLayout();
        torque_layout->setObjectName(QString::fromUtf8("torque_layout"));
        torque_layout->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_2->addLayout(torque_layout);

        label_15 = new QLabel(tab);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(340, 310, 171, 31));
        tabWidget->addTab(tab, QString());
        frame_3->raise();
        frame_5->raise();
        verticalLayoutWidget_45->raise();
        taskMode->raise();
        taskSendBtn->raise();
        label_2->raise();
        label_40->raise();
        label_58->raise();
        frame_8->raise();
        frame_6->raise();
        frame_9->raise();
        label_15->raise();
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(1350, 250, 181, 511));
        label->setAutoFillBackground(false);
        label->setPixmap(QPixmap(QString::fromUtf8(":/:/icons/p73_.png")));
        label->setScaledContents(true);
        torqueon_button = new QPushButton(centralwidget);
        torqueon_button->setObjectName(QString::fromUtf8("torqueon_button"));
        torqueon_button->setEnabled(true);
        torqueon_button->setGeometry(QRect(1030, 10, 151, 61));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        torqueon_button->setFont(font);
        torqueon_button->setCheckable(false);
        torqueon_button->setChecked(false);
        torqueoff_button = new QPushButton(centralwidget);
        torqueoff_button->setObjectName(QString::fromUtf8("torqueoff_button"));
        torqueoff_button->setGeometry(QRect(1180, 10, 151, 61));
        torqueoff_button->setFont(font);
        torqueoff_button->setCheckable(false);
        emergencyoff_button = new QPushButton(centralwidget);
        emergencyoff_button->setObjectName(QString::fromUtf8("emergencyoff_button"));
        emergencyoff_button->setEnabled(true);
        emergencyoff_button->setGeometry(QRect(1340, 10, 81, 101));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        emergencyoff_button->setFont(font1);
        emergencyoff_button->setStyleSheet(QString::fromUtf8("background-color: rgb(204, 0, 0);\n"
"color: rgb(238, 238, 236);"));
        emergencyoff_button->setCheckable(false);
        emergencyoff_button->setChecked(false);
        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(1020, 120, 321, 291));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_37 = new QLabel(frame_4);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(210, 10, 67, 17));
        label_37->setAlignment(Qt::AlignCenter);
        label_38 = new QLabel(frame_4);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(30, 10, 67, 17));
        label_38->setAlignment(Qt::AlignCenter);
        verticalLayoutWidget_11 = new QWidget(frame_4);
        verticalLayoutWidget_11->setObjectName(QString::fromUtf8("verticalLayoutWidget_11"));
        verticalLayoutWidget_11->setGeometry(QRect(270, 120, 21, 131));
        leftleg_layout = new QVBoxLayout(verticalLayoutWidget_11);
        leftleg_layout->setObjectName(QString::fromUtf8("leftleg_layout"));
        leftleg_layout->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_17 = new QWidget(frame_4);
        verticalLayoutWidget_17->setObjectName(QString::fromUtf8("verticalLayoutWidget_17"));
        verticalLayoutWidget_17->setGeometry(QRect(90, 120, 21, 131));
        rightleg_layout = new QVBoxLayout(verticalLayoutWidget_17);
        rightleg_layout->setObjectName(QString::fromUtf8("rightleg_layout"));
        rightleg_layout->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_19 = new QWidget(frame_4);
        verticalLayoutWidget_19->setObjectName(QString::fromUtf8("verticalLayoutWidget_19"));
        verticalLayoutWidget_19->setGeometry(QRect(180, 40, 21, 61));
        waist_layout = new QVBoxLayout(verticalLayoutWidget_19);
        waist_layout->setObjectName(QString::fromUtf8("waist_layout"));
        waist_layout->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_39 = new QWidget(frame_4);
        verticalLayoutWidget_39->setObjectName(QString::fromUtf8("verticalLayoutWidget_39"));
        verticalLayoutWidget_39->setGeometry(QRect(110, 40, 31, 61));
        waist_safety = new QVBoxLayout(verticalLayoutWidget_39);
        waist_safety->setObjectName(QString::fromUtf8("waist_safety"));
        waist_safety->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_40 = new QWidget(frame_4);
        verticalLayoutWidget_40->setObjectName(QString::fromUtf8("verticalLayoutWidget_40"));
        verticalLayoutWidget_40->setGeometry(QRect(20, 120, 31, 131));
        rightleg_safety = new QVBoxLayout(verticalLayoutWidget_40);
        rightleg_safety->setObjectName(QString::fromUtf8("rightleg_safety"));
        rightleg_safety->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_41 = new QWidget(frame_4);
        verticalLayoutWidget_41->setObjectName(QString::fromUtf8("verticalLayoutWidget_41"));
        verticalLayoutWidget_41->setGeometry(QRect(200, 120, 31, 131));
        leftleg_safety = new QVBoxLayout(verticalLayoutWidget_41);
        leftleg_safety->setObjectName(QString::fromUtf8("leftleg_safety"));
        leftleg_safety->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_55 = new QWidget(frame_4);
        verticalLayoutWidget_55->setObjectName(QString::fromUtf8("verticalLayoutWidget_55"));
        verticalLayoutWidget_55->setGeometry(QRect(140, 40, 41, 61));
        waist_elmo = new QVBoxLayout(verticalLayoutWidget_55);
        waist_elmo->setObjectName(QString::fromUtf8("waist_elmo"));
        waist_elmo->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_57 = new QWidget(frame_4);
        verticalLayoutWidget_57->setObjectName(QString::fromUtf8("verticalLayoutWidget_57"));
        verticalLayoutWidget_57->setGeometry(QRect(50, 120, 41, 131));
        rightleg_elmo = new QVBoxLayout(verticalLayoutWidget_57);
        rightleg_elmo->setObjectName(QString::fromUtf8("rightleg_elmo"));
        rightleg_elmo->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_58 = new QWidget(frame_4);
        verticalLayoutWidget_58->setObjectName(QString::fromUtf8("verticalLayoutWidget_58"));
        verticalLayoutWidget_58->setGeometry(QRect(230, 120, 41, 131));
        leftleg_elmo = new QVBoxLayout(verticalLayoutWidget_58);
        leftleg_elmo->setObjectName(QString::fromUtf8("leftleg_elmo"));
        leftleg_elmo->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(820, 430, 521, 331));
        plainTextEdit->setReadOnly(true);
        plainTextEdit->setOverwriteMode(false);
        label_136 = new QLabel(centralwidget);
        label_136->setObjectName(QString::fromUtf8("label_136"));
        label_136->setGeometry(QRect(1350, 120, 51, 21));
        label_136->setFrameShape(QFrame::Box);
        label_136->setAlignment(Qt::AlignCenter);
        label_sestatus = new QLabel(centralwidget);
        label_sestatus->setObjectName(QString::fromUtf8("label_sestatus"));
        label_sestatus->setGeometry(QRect(1410, 120, 101, 21));
        label_sestatus->setAutoFillBackground(false);
        label_sestatus->setFrameShape(QFrame::Box);
        label_sestatus->setAlignment(Qt::AlignCenter);
        label_137 = new QLabel(centralwidget);
        label_137->setObjectName(QString::fromUtf8("label_137"));
        label_137->setGeometry(QRect(1350, 150, 51, 21));
        label_137->setFrameShape(QFrame::Box);
        label_137->setAlignment(Qt::AlignCenter);
        label_tcstatus = new QLabel(centralwidget);
        label_tcstatus->setObjectName(QString::fromUtf8("label_tcstatus"));
        label_tcstatus->setGeometry(QRect(1410, 150, 101, 21));
        label_tcstatus->setAutoFillBackground(false);
        label_tcstatus->setFrameShape(QFrame::Box);
        label_tcstatus->setAlignment(Qt::AlignCenter);
        label_32 = new QLabel(centralwidget);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(1350, 180, 51, 21));
        label_32->setFrameShape(QFrame::Box);
        label_32->setAlignment(Qt::AlignCenter);
        label_imustatus = new QLabel(centralwidget);
        label_imustatus->setObjectName(QString::fromUtf8("label_imustatus"));
        label_imustatus->setGeometry(QRect(1410, 180, 101, 21));
        label_imustatus->setAutoFillBackground(false);
        label_imustatus->setFrameShape(QFrame::Box);
        label_imustatus->setAlignment(Qt::AlignCenter);
        label_34 = new QLabel(centralwidget);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(1350, 210, 51, 21));
        label_34->setFrameShape(QFrame::Box);
        label_34->setAlignment(Qt::AlignCenter);
        label_ecatstatus = new QLabel(centralwidget);
        label_ecatstatus->setObjectName(QString::fromUtf8("label_ecatstatus"));
        label_ecatstatus->setGeometry(QRect(1410, 210, 101, 21));
        label_ecatstatus->setAutoFillBackground(false);
        label_ecatstatus->setFrameShape(QFrame::Box);
        label_ecatstatus->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(820, 30, 198, 41));
        horizontalLayout_10 = new QHBoxLayout(layoutWidget);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_41 = new QLabel(layoutWidget);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        horizontalLayout_10->addWidget(label_41);

        currenttime = new QLabel(layoutWidget);
        currenttime->setObjectName(QString::fromUtf8("currenttime"));
        currenttime->setFrameShape(QFrame::StyledPanel);

        horizontalLayout_10->addWidget(currenttime);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(820, 250, 191, 161));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_35 = new QLabel(frame_2);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(10, 10, 67, 17));
        label_43 = new QLabel(frame_2);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(10, 30, 81, 17));
        label_50 = new QLabel(frame_2);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(10, 70, 81, 17));
        label_51 = new QLabel(frame_2);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(10, 110, 81, 17));
        horizontalLayoutWidget_2 = new QWidget(frame_2);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 50, 171, 21));
        imuQuaternion = new QHBoxLayout(horizontalLayoutWidget_2);
        imuQuaternion->setObjectName(QString::fromUtf8("imuQuaternion"));
        imuQuaternion->setContentsMargins(0, 0, 0, 0);
        horizontalLayoutWidget_3 = new QWidget(frame_2);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 90, 171, 21));
        imuAngVel = new QHBoxLayout(horizontalLayoutWidget_3);
        imuAngVel->setObjectName(QString::fromUtf8("imuAngVel"));
        imuAngVel->setContentsMargins(0, 0, 0, 0);
        horizontalLayoutWidget_4 = new QWidget(frame_2);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(10, 130, 171, 21));
        imuLinAcc = new QHBoxLayout(horizontalLayoutWidget_4);
        imuLinAcc->setObjectName(QString::fromUtf8("imuLinAcc"));
        imuLinAcc->setContentsMargins(0, 0, 0, 0);
        frame_17 = new QFrame(centralwidget);
        frame_17->setObjectName(QString::fromUtf8("frame_17"));
        frame_17->setGeometry(QRect(820, 120, 191, 121));
        frame_17->setFrameShape(QFrame::StyledPanel);
        frame_17->setFrameShadow(QFrame::Raised);
        label_52 = new QLabel(frame_17);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(10, 10, 67, 17));
        label_53 = new QLabel(frame_17);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(10, 30, 81, 17));
        label_54 = new QLabel(frame_17);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(10, 70, 81, 17));
        horizontalLayoutWidget_5 = new QWidget(frame_17);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(10, 50, 171, 21));
        comPos = new QHBoxLayout(horizontalLayoutWidget_5);
        comPos->setObjectName(QString::fromUtf8("comPos"));
        comPos->setContentsMargins(0, 0, 0, 0);
        horizontalLayoutWidget_6 = new QWidget(frame_17);
        horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(10, 90, 171, 21));
        linVel = new QHBoxLayout(horizontalLayoutWidget_6);
        linVel->setObjectName(QString::fromUtf8("linVel"));
        linVel->setContentsMargins(0, 0, 0, 0);
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(1030, 80, 301, 31));
        progressBar->setValue(0);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1533, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        taskMode->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "P73 GUI", nullptr));
#if QT_CONFIG(tooltip)
        safeRstBtn->setToolTip(QCoreApplication::translate("MainWindow", "Initialize Current Yaw to Zero", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        safeRstBtn->setStatusTip(QCoreApplication::translate("MainWindow", "statustip", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        safeRstBtn->setWhatsThis(QCoreApplication::translate("MainWindow", "whatsThis", nullptr));
#endif // QT_CONFIG(whatsthis)
        safeRstBtn->setText(QCoreApplication::translate("MainWindow", "safety reset", nullptr));
#if QT_CONFIG(tooltip)
        initYawBtn->setToolTip(QCoreApplication::translate("MainWindow", "Initialize Current Yaw to Zero", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        initYawBtn->setStatusTip(QCoreApplication::translate("MainWindow", "statustip", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        initYawBtn->setWhatsThis(QCoreApplication::translate("MainWindow", "whatsThis", nullptr));
#endif // QT_CONFIG(whatsthis)
        initYawBtn->setText(QCoreApplication::translate("MainWindow", "Init Yaw", nullptr));
#if QT_CONFIG(tooltip)
        stateEstimateBtn->setToolTip(QCoreApplication::translate("MainWindow", "state estimation on/off", nullptr));
#endif // QT_CONFIG(tooltip)
        stateEstimateBtn->setText(QCoreApplication::translate("MainWindow", "State Estimation", nullptr));
        posCtrlBtn->setText(QCoreApplication::translate("MainWindow", "Joint Position Control", nullptr));
        gravCompBtn->setText(QCoreApplication::translate("MainWindow", "Grav Comp", nullptr));
        taskMode->setItemText(0, QCoreApplication::translate("MainWindow", "Task Mode 0", nullptr));
        taskMode->setItemText(1, QCoreApplication::translate("MainWindow", "Task Mode 1", nullptr));
        taskMode->setItemText(2, QCoreApplication::translate("MainWindow", "Task Mode 2", nullptr));
        taskMode->setItemText(3, QCoreApplication::translate("MainWindow", "Task Mode 3", nullptr));
        taskMode->setItemText(4, QCoreApplication::translate("MainWindow", "Task Mode 4", nullptr));
        taskMode->setItemText(5, QCoreApplication::translate("MainWindow", "Task Mode 5", nullptr));
        taskMode->setItemText(6, QCoreApplication::translate("MainWindow", "Task Mode 6", nullptr));
        taskMode->setItemText(7, QCoreApplication::translate("MainWindow", "Task Mode 7", nullptr));
        taskMode->setItemText(8, QCoreApplication::translate("MainWindow", "Task Mode 8", nullptr));
        taskMode->setItemText(9, QCoreApplication::translate("MainWindow", "Task Mode 9", nullptr));
        taskMode->setItemText(10, QCoreApplication::translate("MainWindow", "Task Mode 10", nullptr));
        taskMode->setItemText(11, QCoreApplication::translate("MainWindow", "Task Mode 11", nullptr));
        taskMode->setItemText(12, QCoreApplication::translate("MainWindow", "Task Mode 12", nullptr));
        taskMode->setItemText(13, QCoreApplication::translate("MainWindow", "Task Mode 13", nullptr));
        taskMode->setItemText(14, QCoreApplication::translate("MainWindow", "Task Mode 14", nullptr));

        taskSendBtn->setText(QCoreApplication::translate("MainWindow", "SEND", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Joint Position Command", nullptr));
        label_40->setText(QCoreApplication::translate("MainWindow", "Position Mode", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "Waist Y", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "L Hip P", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "L Hip Y", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "L Knee", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "L Ankle P", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "L Ankle R", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "L Hip R", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "R Hip P", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "R Hip Y", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "R Knee", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "R Ankle P", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "R Ankle R", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "R Hip R", nullptr));
        initPoseBtn->setText(QCoreApplication::translate("MainWindow", "Init Pose", nullptr));
        zeroPoseBtn->setText(QCoreApplication::translate("MainWindow", "Zero Pose", nullptr));
        label_42->setText(QCoreApplication::translate("MainWindow", "Traj Time", nullptr));
        jointSendBtn->setText(QCoreApplication::translate("MainWindow", "SEND", nullptr));
        label_58->setText(QCoreApplication::translate("MainWindow", "Task Mode", nullptr));
        label_68->setText(QCoreApplication::translate("MainWindow", "R hip Roll", nullptr));
        label_69->setText(QCoreApplication::translate("MainWindow", "R hip Pitch", nullptr));
        label_70->setText(QCoreApplication::translate("MainWindow", "R hip Yaw", nullptr));
        label_71->setText(QCoreApplication::translate("MainWindow", "R knee Pitch", nullptr));
        label_72->setText(QCoreApplication::translate("MainWindow", "R ankle Pitch", nullptr));
        label_73->setText(QCoreApplication::translate("MainWindow", "R ankle Roll", nullptr));
        label_91->setText(QCoreApplication::translate("MainWindow", "Waist Y", nullptr));
        label_44->setText(QCoreApplication::translate("MainWindow", "L hip Roll", nullptr));
        label_45->setText(QCoreApplication::translate("MainWindow", "L hip Pitch", nullptr));
        label_46->setText(QCoreApplication::translate("MainWindow", "L hip Yaw", nullptr));
        label_47->setText(QCoreApplication::translate("MainWindow", "L knee Pitch", nullptr));
        label_48->setText(QCoreApplication::translate("MainWindow", "L ankle Pitch", nullptr));
        label_49->setText(QCoreApplication::translate("MainWindow", "L ankle Roll", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Joint Status Log", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "CommandAndStat", nullptr));
        label->setText(QString());
        torqueon_button->setText(QCoreApplication::translate("MainWindow", "TORQUE ON", nullptr));
        torqueoff_button->setText(QCoreApplication::translate("MainWindow", "TORQUE OFF", nullptr));
        emergencyoff_button->setText(QCoreApplication::translate("MainWindow", "EM\n"
"OFF\n"
"(F1)", nullptr));
        label_37->setText(QCoreApplication::translate("MainWindow", "left", nullptr));
        label_38->setText(QCoreApplication::translate("MainWindow", "right", nullptr));
        plainTextEdit->setPlainText(QCoreApplication::translate("MainWindow", "-- Bluerobin P73 Log --\n"
"", nullptr));
        label_136->setText(QCoreApplication::translate("MainWindow", "SE", nullptr));
        label_sestatus->setText(QCoreApplication::translate("MainWindow", "OFF", nullptr));
        label_137->setText(QCoreApplication::translate("MainWindow", "TC", nullptr));
        label_tcstatus->setText(QCoreApplication::translate("MainWindow", "NOT OK", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "IMU", nullptr));
        label_imustatus->setText(QCoreApplication::translate("MainWindow", "NOT OK", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "ECAT", nullptr));
        label_ecatstatus->setText(QCoreApplication::translate("MainWindow", "NOT OK", nullptr));
        label_41->setText(QCoreApplication::translate("MainWindow", "Time :", nullptr));
        currenttime->setText(QString());
        label_35->setText(QCoreApplication::translate("MainWindow", "IMU Data", nullptr));
        label_43->setText(QCoreApplication::translate("MainWindow", "Euler Angle", nullptr));
        label_50->setText(QCoreApplication::translate("MainWindow", "Ang Vel", nullptr));
        label_51->setText(QCoreApplication::translate("MainWindow", "Linear Acc", nullptr));
        label_52->setText(QCoreApplication::translate("MainWindow", "CoM", nullptr));
        label_53->setText(QCoreApplication::translate("MainWindow", "Position", nullptr));
        label_54->setText(QCoreApplication::translate("MainWindow", "Linear Vel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_P73_H
