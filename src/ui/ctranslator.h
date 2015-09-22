#ifndef CTRANSLATOR_H
#define CTRANSLATOR_H

#include "cglobal.h"

#include <QTranslator>

class CTranslatorPrivate;
class MCD_EXPORT CTranslator : public QTranslator
{
public:
    CTranslator(QObject *parent = 0);
    ~CTranslator();

    bool load(const QString &language, const QString &rootDirectory);
    QString translate(const char *, const char *sourceText, const char *, int) const;

private:
    C_DECLARE_PRIVATE(CTranslator)
    C_DISABLE_COPY(CTranslator)
    CTranslatorPrivate *p_ptr;
};

#endif // CTRANSLATOR_H
