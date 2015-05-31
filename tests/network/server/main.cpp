#include <QCoreApplication>
#include <QTimer>

#include <cserver.h>
#include <cserverplayer.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CServer *server = new CServer(qApp);
    if (!server->listen(QHostAddress::Any, 5927)) {
        qDebug("The server failed to start, probably due to port 5927 occupied by another application.");
        return 0;
    }

    qDebug("The server is listening on 5927");

    QObject::connect(server, &CServer::playerAdded, [=](CServerPlayer *player){
        qDebug() << QString("New player: %1(%2)").arg(player->screenName()).arg(player->id());

        QObject::connect(player, &CServerPlayer::networkDelayChanged, [=](){
            qDebug() << QString("Player %1(%2) Network Delay: %3").arg(player->screenName()).arg(player->id()).arg(player->networkDelay());
        });
        player->updateNetworkDelay();
    });

    QTimer::singleShot(20000, [=](){
        qApp->quit();
    });

    return app.exec();
}
