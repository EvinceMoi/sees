#include <iostream>
#include <clocale>

#include <QApplication>
#include "ui/player_widget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::setlocale(LC_NUMERIC, "C"); // required by libmpv

    PlayerWidget w;
    w.show();

    return app.exec();
}
