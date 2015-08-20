#include <QCoreApplication>
#include <QTime>
#include <QThread>
#include <QTimer>

#include <cserver.h>
#include <cserveruser.h>
#include <croom.h>
#include <cclient.h>
#include <cprotocol.h>

enum TestRequestCommand{
    S_COMMAND_RACING = CARDIRECTOR_SYSTEM_COMMAND_COUNT,
    S_COMMAND_SHUTDOWN
};

void RacingReplyCommand(QObject *receiver, const QVariant &data)
{
    CClient *client = qobject_cast<CClient *>(receiver);

    int number = data.toInt();
    qDebug("data: %d", number);

    number = qrand() % 1000;
    int waitTime = qrand() % 1000;
    QTimer::singleShot(waitTime, [=](){
        client->replyToServer(S_COMMAND_RACING, number);
    });
    qDebug("Tell the server it's %d after %d seconds", number, waitTime);
}

void ShutdownCommand(QObject *, const QVariant &)
{
    qApp->quit();
}

class RoomRequest : public QThread
{
public:
    RoomRequest(QObject *parent = 0)
        : QThread(parent)
    {
    }

    QList<CServerUser *> users;

protected:
    void run()
    {
        qDebug("4 users are ready.");

        int data = 0;
        foreach (CServerUser *user, users) {
            user->prepareRequest(S_COMMAND_RACING, data);
            data++;
        }

        CRoom *room = users.at(0)->room();
        qDebug("Start!");

        for (int i = 0; i < 2; i++) {
            CServerUser *winner = room->broadcastRacingRequest(users, 1000);
            if (winner) {
                int answer = winner->waitForReply().toInt();
                qDebug("The winner is %d (answer: %d)", winner->id(), answer);
            } else {
                qDebug("Timeout expired. No one answered.");
            }

            sleep(1);
        }

        room->broadcastNotification(S_COMMAND_SHUTDOWN);
    }
};

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    CClient::AddInteraction(S_COMMAND_RACING, &RacingReplyCommand);
    CClient::AddCallback(S_COMMAND_SHUTDOWN, &ShutdownCommand);

    QCoreApplication a(argc, argv);

    CServer *server = new CServer(qApp);
    if (server->listen(QHostAddress::Any, 5927)) {
        qDebug("work as a server.");

        RoomRequest *request = new RoomRequest(server);
        QObject::connect(request, &RoomRequest::finished, request, &RoomRequest::deleteLater);
        QObject::connect(request, &RoomRequest::finished, qApp, &QCoreApplication::quit);

        QObject::connect(server, &CServer::userAdded, [=](CServerUser *user){
            if (request->users.length() > 4) {
                user->kick();
            } else {
                request->users << user;
                if (request->users.length() == 4)
                    request->start();
            }
        });
    } else {
        qDebug("work as a client");

        CClient *client = new CClient(qApp);
        QObject::connect(client, &CClient::connected, [=](){
            qDebug("connected. Signing up...");
            client->signup("", "", "takashiro", "zhaoyun");
        });
        client->connectToHost(QHostAddress::LocalHostIPv6, 5927);
    }

    return a.exec();
}
