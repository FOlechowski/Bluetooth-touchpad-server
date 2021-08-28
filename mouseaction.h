#ifndef MOUSEACTION_H
#define MOUSEACTION_H

#include <windows.h>
#include <QString>
#include <QStringList>
#include <QObject>
#include "server.h"

class Server;
/**
 * @brief The MouseAction class
 * Klasa odpowiedzialna za wykonywanie akcji związanych z obsługą myszki
 */
class MouseAction : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy MouseAction
     * @param server wskaźnik do obiektu server
     */
    MouseAction(Server *server);
    /**
      * @brief Destruktor klasy MouseAction
      **/
    ~MouseAction();

public slots:
    /**
      * @brief getMessage Slot odbiera wiadomość i na podstawie jej zawartości decyduje jaką metodę wywołać
      *
      * @param message wiadomość uzyskiwana z klasy server
      */
     void getMessage(QString &message);

private:
    /// tablica zmiennej touchPoint przechowująca współrzędne pierwszego dotknięcia
    short touchPoint[2]={0,0};
    /**
     * @brief gotoxy Metoda przesuwająca kursor do wskazanego punktu
     *
     * @param x współrzędna X punktu
     * @param y współrzędna Y punktu
     */
    void gotoxy(const short x, const short y);
    /**
     * @brief currentPosition Metoda pobierająca aktualną współrzędną kursora
     *
     * @return Współrzędna aktualnej pozycji kursora
     */
    POINT currentPosition();
    /**
     * @brief leftClick Metoda wywołująca metodę leftPress i leftRelease
     *
     */
    void leftClick();
    /**
     * @brief leftPress Metoda wysyłająca informację o przyciśnięciu lewego klawisza myszy
     *
     */
    void leftPress();
    /**
     * @brief leftRelease Metoda wysyłająca informację o puszczeniu lewego klawisza myszy
     *
     */
    void leftRelease();
    /**
     * @brief rightClick Metoda wysyłająca informację o przyciśnięciu i puszczeniu prawego klawisza myszy
     *
     */
    void rightClick();
    /**
     * @brief middleClick Metoda wysyłająca informację o przyciśnięciu i puszczeniu środkowego klawisza myszy
     *
     */
    void middleClick();
    /**
     * @brief moveMouse Metoda określająca docelowe położenie kursora
     *
     * @param x otrzymana współrzędna X
     * @param y otrzymana współrzędna Y
     */
    void moveMouse(const short x, const short y);
    /**
     * @brief connectToSlot Metoda łącząca slot do przesyłania wiadmości między klasami
     *
     */
    void connectToSlot();
    /**
     * @brief server Wskaźnik do obiektu server
     *
     */
    Server *server = nullptr;
};

#endif // MOUSEACTION_H
