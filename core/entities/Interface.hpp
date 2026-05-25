#ifndef EDGE_DETECTOR_INTERFACE_HPP
#define EDGE_DETECTOR_INTERFACE_HPP

#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QMessageBox>
#include <QWindow>
#include <memory>
#include <QMediaDevices>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QVideoFrame>
#include <QAbstractVideoBuffer>
#include <opencv2/opencv.hpp>

#define UI_MSG(Window_name, Window_txt) \
QMessageBox(QMessageBox::Icon::Warning, Window_name, Window_txt).exec(); \
return;                                 \

QT_BEGIN_NAMESPACE

auto adapt_qt_frame = [](const QVideoFrame &frame) -> cv::Mat {
    QVideoFrame copy(frame);
    if (frame.isValid()) {
        cv::Mat frameYUV = cv::Mat(copy.height() + copy.height() / 2, copy.width(), CV_8UC1, (void *) copy.bits(1));
        cv::Mat frameRGB;
        cvtColor(frameYUV, frameRGB, cv::COLOR_YUV2BGRA_I420);
        return frameRGB;
    }
    return {};
};

class Ui_MainWindow {
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QFrame *frame;

    QVBoxLayout *buttons; ///main layout for buttons
    QPushButton *detect_btn; ///detect edges object in video

    QPushButton *disconnect_camera_btn;
    QPushButton *available_cameras_btn;
    QVideoWidget *video_wid;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QCamera *camera;
    QMediaCaptureSession capture_session;
    QVideoSink *v_sink;
    QList<QCameraDevice> cameras;

    void setup_Ui(QMainWindow *MainWindow) {
        if (MainWindow->objectName().isEmpty()) {
            MainWindow->setObjectName("MainWindow");
        }
        cameras = QMediaDevices::videoInputs();
        camera = new QCamera(cameras[0]);

        MainWindow->resize(970, 580);
        MainWindow->setFixedSize(970, 580); //do not resize window
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(780, 20, 160, 230));
        buttons = new QVBoxLayout(verticalLayoutWidget);
        buttons->setSpacing(0);
        buttons->setObjectName("buttons");
        buttons->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        buttons->setContentsMargins(0, 0, 0, 0);

        detect_btn = new QPushButton(verticalLayoutWidget);
        detect_btn->setObjectName("detect_btn");
        buttons->addWidget(detect_btn);

        disconnect_camera_btn = new QPushButton(verticalLayoutWidget);
        disconnect_camera_btn->setObjectName("close_btn");
        buttons->addWidget(disconnect_camera_btn);

        available_cameras_btn = new QPushButton(verticalLayoutWidget);
        available_cameras_btn->setObjectName("available_cams");
        buttons->addWidget(available_cameras_btn);

        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 20, 740, 470));
        frame->setFrameShape(QFrame::Shape::Box);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        frame->setLineWidth(5);
        frame->setMidLineWidth(0);
        video_wid = new QVideoWidget(frame);
        video_wid->setObjectName("video_wid");
        video_wid->setGeometry(QRect(10, 10, 720, 450));
        v_sink = video_wid->videoSink();
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 960, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        translate_Ui(MainWindow);
        QMetaObject::connectSlotsByName(MainWindow);
    }

    void translate_Ui(QMainWindow *MainWindow) const {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Web cam detect", nullptr));
        detect_btn->setText(QCoreApplication::translate("MainWindow", "Turn on camera", nullptr));
        disconnect_camera_btn->setText(QCoreApplication::translate("MainWindow", "Disconnect camera", nullptr));
        available_cameras_btn->setText(QCoreApplication::translate("MainWindow", "Available cameras", nullptr));
    }

    void setup_callbacks() {
        //turn on camera func
        QPushButton::connect(detect_btn, &QPushButton::clicked, [this]() {
            if (cameras.count() > 0) {
                if (camera == nullptr) {
                    camera = new QCamera(cameras[0]);
                }
                capture_session.setCamera(camera);
                capture_session.setVideoOutput(video_wid);
                capture_session.setVideoOutput(v_sink);
                camera->start();
                video_wid->show();

                return;
            } else {
                UI_MSG("Critical Error", "There are no available camera devices")
            }
        });

        //Disconnect camera
        QPushButton::connect(disconnect_camera_btn, &QPushButton::clicked, [this]() {
            if (camera->isActive()) {
                video_wid->close();
                capture_session.setVideoOutput(nullptr);
                capture_session.setCamera(nullptr);
                camera->stop();
                camera->disconnect();
                return;
            } else {
                UI_MSG("Critical Error", "Camera is stopped")
            }
        });

        //Available cameras
        QPushButton::connect(available_cameras_btn, &QPushButton::clicked, [this]() {
            QString tmp_str;
            for (const auto &cam: cameras) {
                tmp_str += cam.description() + "\n";
            }
            QMessageBox(QMessageBox::Icon::NoIcon, "Available cameras", tmp_str).exec();
            return;
        });
    }
};

namespace Ui {
    class MainWindow final : public Ui_MainWindow {
    };
}

QT_END_NAMESPACE

#endif