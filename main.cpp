#include "mediaplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MediaPlayer");
    a.setApplicationVersion("0.0.1");

    MediaPlayer w;
    w.setWindowOpacity(100);
    w.show();

    return a.exec();
}
