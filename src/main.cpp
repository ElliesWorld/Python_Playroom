#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Python Learning Quiz");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Learning Apps");

    MainWindow window;
    window.show();

    return app.exec();
}