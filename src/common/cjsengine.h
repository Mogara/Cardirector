#ifndef CJSENGINE_H
#define CJSENGINE_H

#include "cglobal.h"
#include <QJSEngine>
#include <QJSValue>

class CJSEnginePrivate;
class MCD_EXPORT CJSEngine : public QJSEngine
{
    Q_OBJECT

public:
    CJSEngine(QObject *parent = Q_NULLPTR);
    virtual ~CJSEngine();

private:
    C_DECLARE_PRIVATE(CJSEngine);
    CJSEnginePrivate *p_ptr;
};

class CJSEngineFunctions : public QObject
{
    Q_OBJECT

public:
    CJSEngineFunctions(CJSEngine *engine);

public slots:
    Q_INVOKABLE QJSValue evaluateAnotherJsScript(const QString &fileName);

private:
    CJSEngine *m_engine;
};

#endif // CJSENGINE_H
