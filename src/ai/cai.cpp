#include "cpch.h"


#include "cai.h"
#include "caiengine.h"

#include <QThread>
#include <QDebug>

class CAiPrivate
{
public:

    QThread *thread;
    CAiEngine *aiEngine;

    bool isRequesting;
    QVariant replyData;
    QSemaphore replyReadySemaphore;
    QSemaphore *extraReplyReadySemaphore;
};


CAi::CAi(QObject *parent)
    : QObject(parent), p_ptr(new CAiPrivate)
{
    C_P(CAi);

    p->thread = new QThread(this);
    p->thread->start();
    p->aiEngine = new CAiEngine;
    p->aiEngine->moveToThread(p->thread);

    p->isRequesting = false;
    p->extraReplyReadySemaphore = NULL;

    connect(this, &CAi::notifyToAiEngine, p->aiEngine, &CAiEngine::notify);
    connect(this, &CAi::replyToAiEngine, p->aiEngine, &CAiEngine::reply);
    connect(this, &CAi::requestToAiEngine, p->aiEngine, &CAiEngine::request);

    connect(this, &CAi::initAiToAiEngine, p->aiEngine, &CAiEngine::init, Qt::BlockingQueuedConnection);

    connect(p->aiEngine, &CAiEngine::replyReady, this, &CAi::engineReplyReady);

}

CAi::~CAi()
{
    C_P(CAi);

    // make sure the semaphores are released
    p->replyReadySemaphore.release();
    if (p->extraReplyReadySemaphore != NULL)
        p->extraReplyReadySemaphore->release();

    p->thread->exit();
    if (!p->thread->wait(2000))
        p->thread->terminate();

    if (!p->thread->wait(2000))
        qDebug() << QString("CAi::~CAi: QThread didn't finish termination in 2 seconds, force deleting");

    p->aiEngine->moveToThread(thread());
    p->aiEngine->collectGarbage();
    delete p->aiEngine;

    delete p;
}

void CAi::request(int command, const QVariant &data)
{
    C_P(CAi);
    if (p->replyReadySemaphore.available() > 0)
        p->replyReadySemaphore.acquire(p->replyReadySemaphore.available());

    p->replyData = QVariant();
    p->isRequesting = true;

    emit requestToAiEngine(command, data);
}

void CAi::reply(int command, const QVariant &data)
{
    emit replyToAiEngine(command, data);
}

void CAi::notify(int command, const QVariant &data)
{
    emit notifyToAiEngine(command, data);
}

bool CAi::initAi(const QString &aiStartScriptFile)
{
    emit initAiToAiEngine(aiStartScriptFile);
    C_P(CAi);
    return p->aiEngine->avaliable();
}

void CAi::setReplyReadySemaphore(QSemaphore *semaphore)
{
    C_P(CAi);
    if (!p->isRequesting)
        p->extraReplyReadySemaphore = semaphore;
}

QVariant CAi::waitForReply(int timeout)
{
    C_P(CAi);
    p->replyReadySemaphore.tryAcquire(1, timeout);

    return p->replyData;
}

void CAi::engineReplyReady(QVariant replyData)
{
    C_P(CAi);
    if (!p->isRequesting)
        return;

    p->replyData = replyData;

    p->replyReadySemaphore.release();
    if (p->extraReplyReadySemaphore != NULL) {
        p->extraReplyReadySemaphore->release();
        p->extraReplyReadySemaphore = NULL;
    }
}



