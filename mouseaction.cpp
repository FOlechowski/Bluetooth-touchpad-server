#include "mouseaction.h"

MouseAction::MouseAction(Server *server)
{
    this->server = server;
    connectToSlot();
}

MouseAction::~MouseAction()
{
   server = nullptr;
}

void MouseAction::getMessage(QString &message)
{
    switch (message.size())
    {
    case 3:
        if(message == "LMB")
        {
            leftClick();
        }
        else if(message == "RMB")
        {
            rightClick();
        }
        else if(message == "CSC")
        {
            middleClick();
        }
        else if(message == "EOC")
        {
            touchPoint[0] = 0;
            touchPoint[1] = 0;
            leftRelease();
        }
        else if(message == "LLC")
        {
            touchPoint[0] = 0;
            touchPoint[1] = 0;
            leftPress();

        }
        break;

    default:
        message.remove("MOV:",Qt::CaseInsensitive);
        QStringList list1 = message.split(QLatin1Char(','));
        bool ok1,ok2;
        short x = list1[0].toUShort(&ok1, 10);
        short y = list1[1].toUShort(&ok2, 10);
        if(ok1&&ok2)
        {
            moveMouse(x,y);
        }
        break;

    }
}

void MouseAction::gotoxy(const short x, const short y)
{
    SetCursorPos(x,y);
}

POINT MouseAction::currentPosition()
{
    POINT point;
    GetCursorPos( &point);
    return point;
}

void MouseAction::leftClick()
{
    leftPress();
    leftRelease();
}

void MouseAction::leftPress()
{
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1,&Input,sizeof(INPUT));
}

void MouseAction::leftRelease()
{
    INPUT Input={0};
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1,&Input,sizeof(INPUT));
}

void MouseAction::rightClick()
{
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    ::SendInput(1,&Input,sizeof(INPUT));

    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    ::SendInput(1,&Input,sizeof(INPUT));
}

void MouseAction::middleClick()
{
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    ::SendInput(1,&Input,sizeof(INPUT));

    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    ::SendInput(1,&Input,sizeof(INPUT));
}

void MouseAction::moveMouse(const short x, const short y)
{
    if(!(touchPoint[0]||touchPoint[1]))
    {
        touchPoint[0] = x;
        touchPoint[1] = y;
    }
    gotoxy(currentPosition().x + (x-touchPoint[0])*1.2,currentPosition().y + (y-touchPoint[1]));
    touchPoint[0] = x;
    touchPoint[1] = y;
}

void MouseAction::connectToSlot()
{
    if(server)
    {
        connect(server, &Server::sendMessage,
                this, &MouseAction::getMessage);
    }
}
