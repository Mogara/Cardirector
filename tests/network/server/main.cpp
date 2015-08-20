#include <QCoreApplication>
#include <QTimer>

#include <cserver.h>
#include <cserveruser.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    CServer *server = new CServer(qApp);
    if (!server->listen(QHostAddress::Any, 5927)) {
        qDebug("The server failed to start, probably due to port 5927 occupied by another application.");
        return 0;
    }

    qDebug("The server is listening on 5927");

    QObject::connect(server, &CServer::userAdded, [=](CServerUser *user){
        qDebug() << QString("New user: %1(%2)").arg(user->screenName()).arg(user->id());

        QObject::connect(user, &CServerUser::networkDelayChanged, [=](){
            qDebug() << QString("User %1(%2) Network Delay: %3").arg(user->screenName()).arg(user->id()).arg(user->networkDelay());
        });
        user->updateNetworkDelay();
    });

    QTimer::singleShot(20000, [=](){
        qApp->quit();
    });

    return app.exec();
}
