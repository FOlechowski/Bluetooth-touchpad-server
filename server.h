#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtCore/qobject.h>
#include <QCoreApplication>
#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>

#include "mouseaction.h"

QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class mouseAction;
/**
 * @brief The Server class
 * Klasa odpowiedzialna za działanie serwera bluetooth
 */
class Server : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Server Konstruktor klasy Server
     *
     * @param parent
     */
    Server(QCoreApplication *parent = nullptr);
    /**
     * @brief startServer Metoda uruchamiająca serwer
     *
     * @param localAdapter adres bluetooth
     */
    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    /**
     * @brief stopServer Metoda zatrzymująca serwer
     *
     */
    void stopServer();
    /**
     * @brief Destruktor klasy Server
     */
    ~Server();

signals:
    /**
     * @brief sendMessage Sygnał przekazujący wiadomość do klasu MouseAction
     *
     * @param message wiadomość
     */
    void sendMessage(QString &message);


private slots:
    /**
     * @brief clientConnected Slot aktywowany po podłączeniu klienta z serwerem
     *
     */
    void clientConnected();
    /**
     * @brief clientDisconnected Slot aktywowany po rozłączeniu klienta z serwerem
     *
     */
    void clientDisconnected();
    /**
     * @brief readSocket Metoda odczytująca socket
     *
     */
    void readSocket();

private:
    /**
     * @brief rfcommServer Wskaźnik do serwera
     *
     */
    QBluetoothServer *rfcommServer = nullptr;
    /**
     * @brief serviceInfo Informacje o serwerze
     *
     */
    QBluetoothServiceInfo serviceInfo;
    /**
     * @brief clientSocket Wskażnik do socketu klienta
     *
     */
    QBluetoothSocket *clientSocket = nullptr;

};

#endif // SERVER_H
