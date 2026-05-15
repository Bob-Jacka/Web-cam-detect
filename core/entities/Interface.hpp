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
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QAudioOutput>
#include <QCheckBox>
#include <QWindow>

///Get ui message using qmessage box
#define UI_MSG(Window_name, Window_txt) \
QMessageBox(QMessageBox::Icon::Warning, Window_name, Window_txt).exec(); \
return;                                 \

QT_BEGIN_NAMESPACE

class Ui_MainWindow {
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *buttons; ///main layout for buttons
    QPushButton *open_vid_btn;
    QPushButton *play_vid_btn;

    QPushButton *go_back_btn;
    QPushButton *go_forward_btn;

    QPushButton *detect_btn; ///detect edges object in video
    QPushButton *track_btn;
    QPushButton *stop_btn;
    QPushButton *create_gistogram_btn;
    QPushButton *detect_face_on_frame_btn;

    QCheckBox *is_need_audio;
    QFrame *frame;
    QVideoWidget *video_wid;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    QMediaPlayer *media_player;  //video or dynamic media media_player
    QAudioOutput *audio_output;
    std::unique_ptr<cv::VideoCapture> video_capture;

    void setup_Ui(QMainWindow *MainWindow) {
        if (MainWindow->objectName().isEmpty()) {
            MainWindow->setObjectName("MainWindow");
        }
        media_player = new QMediaPlayer();
        audio_output = new QAudioOutput();

        MainWindow->resize(970, 580);
        MainWindow->setFixedSize(970, 580); //do not resize window
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(780, 19, 160, 227));
        buttons = new QVBoxLayout(verticalLayoutWidget);
        buttons->setSpacing(0);
        buttons->setObjectName("buttons");
        buttons->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        buttons->setContentsMargins(0, 0, 0, 0);

        open_vid_btn = new QPushButton(verticalLayoutWidget);
        open_vid_btn->setObjectName("open_vid_btn");
        buttons->addWidget(open_vid_btn);

        play_vid_btn = new QPushButton(verticalLayoutWidget);
        play_vid_btn->setObjectName("play_vid_btn");
        play_vid_btn->setStyleSheet("background-color: #bab86c;"); //looks like green
        buttons->addWidget(play_vid_btn);

        go_back_btn = new QPushButton(verticalLayoutWidget);
        go_back_btn->setObjectName("go_back_btn");
        go_back_btn->setStyleSheet("background-color: #ee82ee;"); //looks like pink
        buttons->addWidget(go_back_btn);

        go_forward_btn = new QPushButton(verticalLayoutWidget);
        go_forward_btn->setObjectName("go_forward_btn");
        go_forward_btn->setStyleSheet("background-color: #ee82ee;");
        buttons->addWidget(go_forward_btn);

        stop_btn = new QPushButton(verticalLayoutWidget);
        stop_btn->setObjectName("stop_btn");
        stop_btn->setStyleSheet("background-color: #c6a664;"); //looks like red
        buttons->addWidget(stop_btn);

        create_gistogram_btn = new QPushButton(verticalLayoutWidget);
        create_gistogram_btn->setObjectName("stop_btn");
        create_gistogram_btn->setStyleSheet("background-color: #c6a664;");
        buttons->addWidget(create_gistogram_btn);

        detect_face_on_frame_btn = new QPushButton(verticalLayoutWidget);
        detect_face_on_frame_btn->setObjectName("detect_face_on_frame_btn");
        detect_face_on_frame_btn->setStyleSheet("background-color: #c6a664;");
        buttons->addWidget(detect_face_on_frame_btn);

        detect_btn = new QPushButton(verticalLayoutWidget);
        detect_btn->setObjectName("detect_btn");
        buttons->addWidget(detect_btn);

        track_btn = new QPushButton(verticalLayoutWidget);
        track_btn->setObjectName("track_btn");
        buttons->addWidget(track_btn);

        is_need_audio = new QCheckBox(verticalLayoutWidget);
        is_need_audio->setObjectName("is_need_audio");
        is_need_audio->setCheckState(Qt::CheckState::Checked);
        buttons->addWidget(is_need_audio);

        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 20, 741, 471));
        frame->setFrameShape(QFrame::Shape::Box);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        frame->setLineWidth(5);
        frame->setMidLineWidth(0);
        video_wid = new QVideoWidget(frame);
        video_wid->setObjectName("video_wid");
        video_wid->setGeometry(QRect(10, 10, 721, 451));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 964, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        translate_Ui(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setup_Ui

    /**
     * Translate text on buttons
     * @param MainWindow
     */
    void translate_Ui(QMainWindow *MainWindow) const {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Edge detector", nullptr));
        open_vid_btn->setText(QCoreApplication::translate("MainWindow", "Open video", nullptr));
        play_vid_btn->setText(QCoreApplication::translate("MainWindow", "Play video", nullptr));
        go_back_btn->setText(QCoreApplication::translate("MainWindow", "Go back", nullptr));
        go_forward_btn->setText(QCoreApplication::translate("MainWindow", "Go forward", nullptr));
        detect_btn->setText(QCoreApplication::translate("MainWindow", "Detect edges", nullptr));
        track_btn->setText(QCoreApplication::translate("MainWindow", "Track object", nullptr));
        stop_btn->setText(QCoreApplication::translate("MainWindow", "Stop video", nullptr));
        create_gistogram_btn->setText(QCoreApplication::translate("MainWindow", "Gistogram", nullptr));
        detect_face_on_frame_btn->setText(QCoreApplication::translate("MainWindow", "Detect face on frame", nullptr));
        is_need_audio->setText(QCoreApplication::translate("MainWindow", "Turn on/off audio?", nullptr));
    } // translate_Ui

    /**
     * Setup ui callback
     */
    void setup_callbacks() {
        //Open video
        QPushButton::connect(open_vid_btn, &QPushButton::clicked,
                             [this] {

                                 QString filename = QFileDialog::getOpenFileName(nullptr, "Choose File");

                                 if (filename.endsWith("jpg") or filename.endsWith("png")) {
                                     UI_MSG("Error", "Photos are not supported")
                                 }
                                 if (filename.isEmpty()) {
                                     UI_MSG("Warning", "Filename is empty")
                                 }
                                 media_player->setSource(QUrl(filename));
                                 video_capture = std::make_unique<cv::VideoCapture>(filename.toStdString());
                                 if (media_player->source().isEmpty()) {
                                     UI_MSG("Warning", "An error occurred during video insert")
                                 }

                                 media_player->setVideoOutput(video_wid);
                                 media_player->setAudioOutput(audio_output);
                                 video_wid->show();
                             });

        //Play video
        QPushButton::connect(play_vid_btn, &QPushButton::clicked, [this] {
            if (media_player->isPlaying()) {
                media_player->pause(); //unexpected useful feature
                return;
            }

            if (not media_player->hasVideo()) {
                UI_MSG("Warning", "Open video first")
            }

            media_player->play();

            if (not media_player->isPlaying()) {
                UI_MSG("Warning", "An error occurred during playing video")
            }
        });

        //Go back in video
        QPushButton::connect(go_back_btn, &QPushButton::clicked, [this] {
            if (media_player->hasVideo()) {
                auto pos = GET_VID_POS;
                pos -= BACKWARD_STEP;

                media_player->setPosition(pos);
                video_capture->set(cv::CAP_PROP_POS_MSEC, pos);
                return;
            } else {
                UI_MSG("Error", "Video is not turned on")
            }
        });

        //Go forward in video
        QPushButton::connect(go_forward_btn, &QPushButton::clicked, [this] {
            if (media_player->hasVideo()) {
                auto pos = GET_VID_POS;
                pos += FORWARD_STEP;

                media_player->setPosition(pos);
                video_capture->set(cv::CAP_PROP_POS_MSEC, pos);
                return;
            } else {
                UI_MSG("Error", "Video is not turned on")
            }
        });

        //Stop video
        QPushButton::connect(stop_btn, &QPushButton::clicked, [this] {
            if (media_player->hasVideo()) {
                media_player->pause();
                if (media_player->isPlaying()) {
                    UI_MSG("Warning", "Video is not stopped, error occurred")
                }
            } else {
                UI_MSG("Warning", "Video is not turned on")
            }
        });

        //For gistograms (histograms) creation
        QPushButton::connect(create_gistogram_btn, &QPushButton::clicked, [this] {
            if (media_player->hasVideo()) {
                media_player->pause();
                if (media_player->isPlaying()) {
                    UI_MSG("Warning", "Video is not stopped, error occurred")
                } else {
                    cv::Mat hist;
                    cv::Mat tmp_frame;
                    cv::Mat gray_frame;
                    bool success = video_capture->read(tmp_frame);
                    cv::cvtColor(tmp_frame, gray_frame, cv::COLOR_BGR2GRAY);
                    if (not success) {
                        qDebug() << "Video has ended";
                        return;
                    }

                    const int hist_size = 256;
                    float range[] = {0, 256};
                    const float *histRange = {range};
                    cv::calcHist(&gray_frame, 1, nullptr, cv::Mat(), hist, 1, &hist_size, &histRange);
                    cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

                    cv::Mat hist_img(256, 256, CV_8UC3, cv::Scalar(0, 0, 0));
                    for (int i = 0; i < hist_size; ++i) {
                        cv::line(hist_img,
                                 cv::Point(i, 256),
                                 cv::Point(i, 256 - cvRound(hist.at<float>(i))),
                                 cv::Scalar(0, 255, 0), 1);
                    }
                    cv::imshow("Gistograma", hist_img);
                    return;
                }
            } else {
                UI_MSG("Error", "Video is not turned on")
            }
        });

        //Detect face on video frame
        QPushButton::connect(detect_face_on_frame_btn, &QPushButton::clicked, [this] {
            if (not media_player->hasVideo()) {
                UI_MSG("Error", "Video is not turned on")
            }
            media_player->pause(); //stop video when detect
            cv::Mat tmp_frame;
            cv::HOGDescriptor hog;
            hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

            std::vector<cv::Rect> found_locations;
            hog.detectMultiScale(tmp_frame, found_locations, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2);
            return;
        });

        //Detecting edges in video
        QPushButton::connect(detect_btn, &QPushButton::clicked, [this] {
            if (not media_player->hasVideo()) {
                UI_MSG("Error", "Video is not turned on")
            }

            media_player->pause(); //stop video when detect
            cv::Mat tmp_frame;
            video_capture->set(cv::CAP_PROP_POS_MSEC, GET_VID_POS);
            bool success = video_capture->read(tmp_frame);
            if (not success) {
                qDebug() << "Video has ended";
                return;
            }
            Edger::detect_edges(tmp_frame);
            return;
        });

        //Track video
        QPushButton::connect(track_btn, &QPushButton::clicked, [this] {
            if (not media_player->hasVideo()) {
                UI_MSG("Error", "Video is not turned on")
            }

            media_player->pause(); //stop main video when detect
            cv::Mat tmp_frame;
            video_capture->set(cv::CAP_PROP_POS_MSEC, GET_VID_POS);
            bool success = video_capture->read(tmp_frame);
            if (!success) {
                qDebug() << "Video has ended";
                return;
            }
            Edger::track_object<cv::TrackerCSRT>(video_capture.get(), tmp_frame);
            return;
        });

        //Audio settings
        QCheckBox::connect(is_need_audio, &QCheckBox::clicked, [this] {
            if (media_player->hasVideo() or media_player->hasAudio()) {
                bool current_mute = is_need_audio->isChecked();
                audio_output->setMuted(!current_mute);
                return;
            } else {
                is_need_audio->setChecked(false);
                UI_MSG("Error", "Video or audio is not turned on")
            }
        });
    }
};

namespace Ui {
    class MainWindow final : public Ui_MainWindow {
    };
}

QT_END_NAMESPACE

#endif