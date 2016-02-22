#include "cjsengine.h"

#include <QFile>

class CJSEnginePrivate
{
public:
    CJSEngineFunctions *funcs;
};

CJSEngine::CJSEngine(QObject *parent)
    : QJSEngine(parent), p_ptr(new CJSEnginePrivate)
{
    C_P(CJSEngine);
    p->funcs = new CJSEngineFunctions(this);

    globalObject().setProperty("Cardirector", newQObject(p->funcs));
}

CJSEngine::~CJSEngine()
{
    delete p_ptr;
}

CJSEngineFunctions::CJSEngineFunctions(CJSEngine *engine)
    : m_engine(engine), QObject(engine)
{

}

QJSValue CJSEngineFunctions::evaluateAnotherJsScript(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return QJSValue();

    QString script = file.readAll();
    file.close();

    return m_engine->evaluate(script, fileName);
}
