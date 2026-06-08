#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QThread>
#include <QImage>
#include <QTimer>
#include <atomic>
#include <filesystem>
#include <QPushButton>
#include <QMutexLocker>

/**
* Platform independent filepath getter.
* @param optional_file_name if you need to include filename into path
* @return string value of current path
*/
inline std::string get_current_dir_name(const std::string &optional_file_name = "") {
    auto res = std::filesystem::current_path().string();
    if (!optional_file_name.empty()) {
        res += "/" + optional_file_name;
    }
    return res;
}

void add_overlay_img(const cv::Mat &src, const cv::Mat &overlay, cv::Point location) {
    int roi_x = std::max(0, location.x);
    int roi_y = std::max(0, location.y);
    int roi_w = std::min(overlay.cols, src.cols - roi_x);
    int roi_h = std::min(overlay.rows, src.rows - roi_y);

    if (roi_w <= 0 || roi_h <= 0) {
        return; //return in case of error
    }

    cv::Mat roi = src(cv::Rect(roi_x, roi_y, roi_w, roi_h));
    cv::Mat overlay_roi = overlay(cv::Rect(0, 0, roi_w, roi_h));

    if (overlay.channels() == 4) {
        std::vector<cv::Mat> ch(4);
        cv::split(overlay_roi, ch);
        cv::Mat alpha;
        ch[3].convertTo(alpha, CV_32FC1, 1.0 / 255.0);

        cv::Mat alpha3;
        cv::merge(std::vector<cv::Mat>{alpha, alpha, alpha}, alpha3);

        cv::Mat roi_f;
        roi.convertTo(roi_f, CV_32FC3, 1.0 / 255.0);

        cv::Mat overlay_f;
        cv::Mat overlay_rgb;
        cv::cvtColor(overlay_roi, overlay_rgb, cv::COLOR_BGRA2RGB);
        overlay_rgb.convertTo(overlay_f, CV_32FC3, 1.0 / 255.0);

        cv::Mat blended = roi_f.mul(1.0 - alpha3) + overlay_f.mul(alpha3);

        blended.convertTo(roi, CV_8UC3, 255.0);
    } else {
        overlay_roi.copyTo(roi);
    }
}

class Camera_thread final : public QThread {
Q_OBJECT
public:
    void run() override {
        if (!m_filename_to_show.empty()) {
#ifdef Q_OS_LINUX
            cv::VideoCapture cap("/dev/video0", cv::CAP_V4L2);
#else
#error "Install normal system (Linux) or go away"
#endif
            auto load_res = m_face_cascade.load(get_current_dir_name("haarcascade_frontalface_default.xml"));
            if (!load_res) {
                emit error("Haar's cascades are not loaded");
                return;
            }
            if (!cap.isOpened()) {
                emit error("Cannot open camera object");
                return;
            }

            cv::Mat frame;
            if (overlay_img.empty()) {
                emit error("Overlay image is not read");
                return;
            }
            while (!m_stop) {
                cap >> frame;
                if (frame.empty()) {
                    continue;
                }

                cv::Mat camera_frame;
                std::vector<cv::Rect> faces;
                cv::cvtColor(frame, camera_frame, cv::COLOR_BGR2RGB);
                m_face_cascade.detectMultiScale(camera_frame, faces, 1.1, 4);
                if (!faces.empty()) {
                    for (const auto &face: faces) {
                        cv::Mat resized_overlay;
                        cv::resize(overlay_img, resized_overlay, face.size(), 0, 0, cv::INTER_LINEAR);
                        add_overlay_img(camera_frame, resized_overlay, cv::Point(face.x, face.y));
//                      cv::rectangle(camera_frame, face, cv::Scalar(0, 255, 0), 2);
                    }
                }

                QImage image(camera_frame.data, camera_frame.cols, camera_frame.rows, (long long) camera_frame.step,
                             QImage::Format_RGB888);
                emit frame_ready(image.copy());
            }
        }
    }

    void set_overlay(const std::string &filename) {
        QMutexLocker locker(&m_mutex);
        m_filename_to_show = filename;
        overlay_img = cv::imread(get_current_dir_name(filename), cv::IMREAD_UNCHANGED);
    }

    void stop() {
        //TODO think about how to stop thread
        m_stop = true;
    }

    Camera_thread() = default;

    Camera_thread(Camera_thread &&other) = delete;

    Camera_thread(Camera_thread &other) = delete;

    Camera_thread &operator=(Camera_thread &other) = delete;

    Camera_thread &operator=(Camera_thread &&other) = delete;

    ~Camera_thread() override = default;

signals:

    void frame_ready(const QImage &frame);

    void error(const QString &msg);

private:
    cv::CascadeClassifier m_face_cascade;
    std::atomic<bool> m_stop{false};
    std::string m_filename_to_show;
    cv::Mat overlay_img;
    QMutex m_mutex;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    QLabel label;
    QPushButton clown_btn; //button to show clown nose
    QPushButton cat_ears_btn; //button to cat ears
    Camera_thread camera;
    clown_btn.setText("Look like a clown");
    cat_ears_btn.setText("Look like a quadrober");

    window.resize(970, 580);
    window.setFixedSize(970, 580);

    QVBoxLayout layout = QVBoxLayout(&window);
    label.setAlignment(Qt::AlignCenter);
    layout.addWidget(&label);
    layout.addWidget(&clown_btn);
    layout.addWidget(&cat_ears_btn);

    //Set up callbacks:
    QObject::connect(&camera, &Camera_thread::frame_ready, [&](const QImage &img) -> void {
        label.setPixmap(QPixmap::fromImage(img).scaled(
                label.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    QObject::connect(&camera, &Camera_thread::error, [&](const QString &msg) -> void {
        label.setText("Error: " + msg);
    });

    QObject::connect(&clown_btn, &QPushButton::clicked, [&camera]() -> void {
        camera.set_overlay("clown-suit.png");
        if (!camera.isRunning()) {
            camera.start();
        }
        return;
    });

    QObject::connect(&cat_ears_btn, &QPushButton::clicked, [&camera]() -> void {
        camera.set_overlay("cat-ears3.png");
        if (!camera.isRunning()) {
            camera.start();
        }
        return;
    });

    camera.start();
    window.show();

    int result = QApplication::exec();

    camera.stop();
    camera.wait();

    return result;
}

#include "main.moc"