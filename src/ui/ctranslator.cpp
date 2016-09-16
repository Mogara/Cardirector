/********************************************************************
    Copyright (c) 2013-2015 - Mogara

    This file is part of Cardirector.

    This game engine is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 3.0
    of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the LICENSE file for more details.

    Mogara
*********************************************************************/

#include "ctranslator.h"

#include <QHash>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

class CTranslatorPrivate
{
public:
    QHash<QString, QString> translations;

    void load(QDir dir)
    {
        QStringList fileNames = dir.entryList(QStringList() << QStringLiteral("*.json"), QDir::Files);
        foreach (const QString &fileName, fileNames)
            load(dir.filePath(fileName));

        QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (const QString &dirName, subDirs) {
            dir.cd(dirName);
            load(dir);
            dir.cdUp();
        }
    }

    void load(const QString &filePath)
    {
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly))
            return;

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError)
            return;

        const QJsonObject translations = doc.object();
        for (QJsonObject::ConstIterator i = translations.constBegin(); i != translations.constEnd(); i++)
            this->translations.insert(i.key(), i.value().toString());
    }
};

CTranslator::CTranslator(QObject *parent)
    : QTranslator(parent)
    , p_ptr(new CTranslatorPrivate)
{
}

CTranslator::~CTranslator()
{
    delete p_ptr;
}

bool CTranslator::load(const QString &language, const QString &rootDirectory)
{
    QDir dir(rootDirectory);
    if (!dir.exists() || !dir.cd(language))
        return false;
    p_ptr->load(dir);
    return true;
}

QString CTranslator::translate(const char *, const char *sourceText, const char *, int) const
{
    return p_ptr->translations.value(QString::fromUtf8(sourceText));
}

