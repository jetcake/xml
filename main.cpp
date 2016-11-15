#include "dialog.h"
#include "explorer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    w.setWindowState(Qt::WindowFullScreen);



    return a.exec();
}
