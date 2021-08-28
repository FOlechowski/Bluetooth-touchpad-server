#include <QCoreApplication>
#include <server.h>
#include "mouseaction.h"
#include <windows.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server(&a);
    MouseAction m(&server);
    server.startServer();
    qDebug()<<"Server is running";
    HWND okno = GetConsoleWindow();
    //ShowWindow( okno, SW_HIDE );
    CloseWindow(okno);

    return a.exec();
}


