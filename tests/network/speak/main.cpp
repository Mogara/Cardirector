#include <QCoreApplication>

#include <cserver.h>
#include <cabstractclient.h>
#include <cclientplayer.h>
#include <cprotocol.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CServer server;
    if (!server.listen(QHostAddress::Any, 5927)) {
        CAbstractClient *client = new CAbstractClient(&server);

        QObject::connect(client, &CAbstractClient::connected, [=](){
            client->signup("", "", "Takashiro", "zhaoyun");
        });

        QObject::connect(client, &CAbstractClient::loggedIn, [=](){
            foreach (const CClientPlayer *player, client->players()) {
                QObject::connect(player, &CClientPlayer::speak, [=](const QString &message){
                    qDebug() << QString("%1(%2) said: %3").arg(player->screenName()).arg(player->id()).arg(message);
                });
            }
        });

        QObject::connect(client, &CAbstractClient::playerAdded, [=](const CClientPlayer *player){
            qDebug() << QString("%1(%2) came").arg(player->screenName()).arg(player->id());
            QObject::connect(player, &CClientPlayer::speak, [=](const QString &message){
                qDebug() << QString("%1(%2) said: %3").arg(player->screenName()).arg(player->id()).arg(message);
            });

            //Say hello to him.
            CClientPlayer *self = client->self();
            if (self && self != player)
                client->notifyServer(S_COMMAND_SPEAK, QString("welcome, %2(%1)!").arg(player->id()).arg(player->screenName()));
        });

        QObject::connect(client, &CAbstractClient::playerRemoved, [](const CClientPlayer *player){
            qDebug() << QString("%1(%2) left").arg(player->screenName()).arg(player->id());
        });

        client->connectToHost(QHostAddress::LocalHostIPv6, 5927);
    } else {
        qDebug("Server is listening...");
    }

    return app.exec();
}
