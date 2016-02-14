#ifndef CAIENGINE_H
#define CAIENGINE_H

#include "cglobal.h"
#include <QJSEngine>

MCD_BEGIN_NAMESPACE
class CAiEnginePrivate;
class MCD_EXPORT CAiEngine : public QJSEngine
{
    Q_OBJECT

public:
    CAiEngine();
    ~CAiEngine();

public slots:
    void request(int command, const QVariant &data);
    void reply(int command, const QVariant &data);
    void notify(int command, const QVariant &data);

    void init(QString startScriptFile);

    bool avaliable() const;

signals:
    void replyReady(QVariant replyData);

private:
    C_DECLARE_PRIVATE(CAiEngine);
    CAiEnginePrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CAIENGINE_H
