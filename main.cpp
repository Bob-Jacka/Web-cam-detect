#include "core/entities/Interface.hpp"

QT_BEGIN_NAMESPACE

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto window = std::make_unique<Ui::MainWindow>();
    auto main_win = std::make_unique<QMainWindow>();

    window->setup_Ui(main_win.get());
    window->setup_callbacks();

    if (main_win) {
        main_win->show();
    }

    return QApplication::exec();
}

QT_END_NAMESPACE