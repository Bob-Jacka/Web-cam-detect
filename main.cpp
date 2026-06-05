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

class CameraThread final : public QThread {
Q_OBJECT
public:
    void run() override {
#ifdef Q_OS_LINUX
        cv::VideoCapture cap("/dev/video0", cv::CAP_V4L2);
#else
#error "Install normal system (Linux) or go away"
#endif
        auto load_res = face_cascade.load(get_current_dir_name("haarcascade_frontalface_default.xml"));
        if (!load_res) {
            emit error("Haar's cascades are not loaded");
            return;
        }
        if (!cap.isOpened()) {
            emit error("Cannot open camera object");
            return;
        }

        cv::Mat frame;
        while (!m_stop) {
            cap >> frame;
            if (frame.empty()) {
                continue;
            }

            cv::Mat rgb;
            std::vector<cv::Rect> faces;
            cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
            face_cascade.detectMultiScale(rgb, faces, 1.1, 4);
            if (!faces.empty()) {
                for (const auto &face: faces) {
                    cv::rectangle(rgb, face, cv::Scalar(0, 255, 0), 2);
                }
            }

            QImage image(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);

            emit frame_ready(image.copy());
        }
    }

    void stop() { m_stop = true; }

signals:

    void frame_ready(const QImage &frame);

    void error(const QString &msg);

private:
    cv::CascadeClassifier face_cascade;
    std::atomic<bool> m_stop{false};
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    auto layout = new QVBoxLayout(&window);
    QLabel label;
    label.setAlignment(Qt::AlignCenter);
    layout->addWidget(&label);

    //Set up callbacks:
    CameraThread camera;
    QObject::connect(&camera, &CameraThread::frame_ready, [&](const QImage &img) -> void {
        label.setPixmap(QPixmap::fromImage(img).scaled(
                label.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    QObject::connect(&camera, &CameraThread::error, [&](const QString &msg) -> void {
        label.setText("Error: " + msg);
    });

    camera.start();

    window.resize(640, 480);
    window.show();

    int result = app.exec();

    camera.stop();
    camera.wait();

    return result;
}

#include "main.moc"