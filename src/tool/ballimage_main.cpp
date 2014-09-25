#include <QtGui/QApplication>
#include "BallImageTool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tool::BallImageTool theTool;
    theTool.show();
    return a.exec();
}
