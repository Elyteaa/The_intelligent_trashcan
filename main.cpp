#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setFixedSize(250,105);
    w.setWindowTitle("The intelligent trash can");
    w.show();

    return a.exec();
}
