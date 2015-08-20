#include <QCoreApplication>
#include <QTimer>

#include <cclient.h>
#include <cclientuser.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CClient *client = new CClient(&app);

    QObject::connect(client, &CClient::connected, [=](){
        client->signup("", "", "Takashiro", "machao");
    });

    QObject::connect(client, &CClient::roomListUpdated, [=](const QVariant &data){
        QVariantList rooms(data.toList());
        qDebug() << "Rooms: " << rooms;
        if (rooms.isEmpty()) {
            client->createRoom();
        } else {
            QVariantList room(rooms.at(0).toList());
            uint roomId = room.at(0).toUInt();
            client->enterRoom(roomId);
        }
    });

    QObject::connect(client, &CClient::userAdded, [=](const CClientUser *user){
        qDebug() << QString("%1(%2) came").arg(user->screenName()).arg(user->id());
        QObject::connect(user, &CClientUser::speak, [=](const QString &message){
            qDebug() << QString("%1(%2) said: %3").arg(user->screenName()).arg(user->id()).arg(message);
        });

        //Say hello to him.
        CClientUser *self = client->self();
        if (self && self != user)
            client->speakToServer(QString("welcome, %2(%1)!").arg(user->id()).arg(user->screenName()));
    });

    QObject::connect(client, &CClient::userRemoved, [](const CClientUser *user){
        qDebug() << QString("%1(%2) left").arg(user->screenName()).arg(user->id());
    });

    client->connectToHost(QHostAddress::LocalHostIPv6, 5927);

    QTimer::singleShot(5000, [=](){
        qApp->quit();
    });

    return app.exec();
}
