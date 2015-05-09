#include <QCoreApplication>
#include <QTime>
#include <QThread>
#include <QTimer>

#include <cserver.h>
#include <cserverplayer.h>
#include <cclient.h>
#include <cprotocol.h>

enum TestRequestCommand
{
    S_COMMAND_REQUEST_SUCCESSOR = CARDIRECTOR_SYSTEM_COMMAND_COUNT + 1,
    S_COMMAND_SHUTDOWN
};

void ClientRequestSuccessorCommand(QObject *receiver, const QVariant &data)
{
    CClient *client = qobject_cast<CClient *>(receiver);
    int number = data.toInt() + 1;
    int waitTime = qrand() % 10000;
    QTimer::singleShot(waitTime, [=](){
        client->replyToServer(S_COMMAND_REQUEST_SUCCESSOR, number);
    });
    qDebug("Tell the server it's %d after %d seconds", number, waitTime);
}

void ServerRequestSuccessorCommand(QObject *receiver, const QVariant &data)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    qDebug("Reply callback executed. %d replied %d", player->id(), data.toInt());
}

void ShutdownCommand(QObject *, const QVariant &)
{
    qApp->quit();
}

class RoomRequest : public QThread
{
public:
    RoomRequest(CServerPlayer *player, QObject *parent = 0)
        : QThread(parent)
        , player(player)
    {
    }

protected:
    void run()
    {
        int number = qrand();
        qDebug("Ask %d for the successor of %d", player->id(), number);
        player->request(S_COMMAND_REQUEST_SUCCESSOR, number);
        qDebug("Wait for reply....");
        QVariant result = player->waitForReply(5000);
        if (!result.isNull())
            qDebug("The result is: %d\n", result.toInt());
        else
            qDebug("timeout.");
        player->notify(S_COMMAND_SHUTDOWN);
    }

    CServerPlayer *player;
};

int main(int argc, char **argv)
{
    CClient::AddInteraction(S_COMMAND_REQUEST_SUCCESSOR, &ClientRequestSuccessorCommand);
    CClient::AddCallback(S_COMMAND_SHUTDOWN, &ShutdownCommand);
    CServerPlayer::AddCallback(S_COMMAND_REQUEST_SUCCESSOR, &ServerRequestSuccessorCommand);

    QCoreApplication app(argc, argv);
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    CServer *server = new CServer(qApp);
    if (!server->listen(QHostAddress::Any, 5927)) {
        qDebug("work as a client");

        CClient *client = new CClient(qApp);

        QObject::connect(client, &CClient::connected, [=](){
            qDebug("connected. Signing up...");
            client->signup("", "", "takashiro", "zhugeliang");
        });

        client->connectToHost(QHostAddress::LocalHostIPv6, 5927);
    } else {
        qDebug("work as a server.");

        QObject::connect(server, &CServer::playerAdded, [=](CServerPlayer *player){
            RoomRequest *request = new RoomRequest(player);
            request->start();
            QObject::connect(request, &RoomRequest::finished, request, &RoomRequest::deleteLater);
            QObject::connect(request, &RoomRequest::finished, qApp, &QCoreApplication::quit);
        });
    }

    return app.exec();
}
