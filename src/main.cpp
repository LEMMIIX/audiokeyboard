#include "mainwindow.h"
#include "soundwave.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /////////////

    //RtAudio adc;


    /////////////

    return a.exec();
}
