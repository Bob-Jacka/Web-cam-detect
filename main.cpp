#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QThread>
#include <QImage>
#include <QTimer>
#include <atomic>

class CameraThread : public QThread {
Q_OBJECT
public:
    void run() override {
#ifdef Q_OS_LINUX
        cv::VideoCapture cap("/dev/video0", cv::CAP_V4L2);
#else
#error "Install normal system (linux) or go away"
#endif
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
            cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);

            QImage image(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);

            emit frameReady(image.copy());
        }
    }

    void stop() { m_stop = true; }

signals:

    void frameReady(const QImage &frame);

    void error(const QString &msg);

private:
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
    QObject::connect(&camera, &CameraThread::frameReady, [&](const QImage &img) -> void {
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