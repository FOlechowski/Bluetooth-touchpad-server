#include "server.h"
#include <QtBluetooth>
#include <QDebug>

static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805f9b34fb");

Server::Server(QCoreApplication *parent)
:   QObject(parent)
{
  //empty constructor
}

void Server::startServer(const QBluetoothAddress &localAdapter)
{
    if (rfcommServer)
    {
        return;
    }

    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, &QBluetoothServer::newConnection,
            this, QOverload<>::of(&Server::clientConnected));
    bool result = rfcommServer->listen(localAdapter);

    if (!result)
    {
        //qWarning() << "Cannot bind chat server to" << localAdapter.toString();
        return;
    }

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bluetooth touchpad"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Touchpad for PC"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("Student"));

    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    serviceInfo.registerService(localAdapter);
}

Server::~Server()
{
    //qDebug() << "Robię destruktor";
    serviceInfo.unregisterService();
    if (clientSocket)
    {
        delete clientSocket;
        clientSocket = nullptr;
    }
    delete rfcommServer;
    rfcommServer = nullptr;
}

void Server::clientConnected()
{
    if (!clientSocket)
    {
        QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
        if (!socket)
        {
            return;
        }

        connect(socket, &QBluetoothSocket::readyRead, this, &Server::readSocket);
        connect(socket, &QBluetoothSocket::disconnected, this, QOverload<>::of(&Server::clientDisconnected));
        clientSocket = socket;
        qDebug() << "Connected with: " << socket->peerName();
    }
}

void Server::clientDisconnected()
{

    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    qDebug()<< socket->peerName() << " disconnected.";
    if (!socket)
    {
        return;
    }


    socket->deleteLater();
    clientSocket = nullptr;

}

void Server::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
    {
        //qDebug() << "socket not created!!!";
        return;
    }

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        //qDebug() << "Message" <<  QString::fromUtf8(line.constData(), line.length());

        QString message = QString::fromUtf8(line.constData(), line.length());

        emit sendMessage(message);


    }
}
