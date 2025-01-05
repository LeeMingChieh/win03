#include <QApplication>
#include "CalendarWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CalendarWidget window;
    window.show();

    return app.exec();
}
