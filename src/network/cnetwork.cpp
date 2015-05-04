#include "cnetwork.h"

#include <QSettings>
#include <QCoreApplication>

void cRegisterUrlScheme(const QString &introduction)
{
#ifdef Q_OS_WIN
    QString scheme = qApp->applicationName();
    scheme.remove(QRegExp("[^A-Za-z]"));

    QString appPath = QCoreApplication::applicationFilePath();
    appPath.replace('/', '\\');

    QSettings reg(QString("HKEY_CLASSES_ROOT"), QSettings::NativeFormat);
    reg.beginGroup(scheme);
    reg.setValue(".", introduction);
    reg.setValue("URL Protocol", "");
    reg.setValue("DefaultIcon/.", QString("%1, 0").arg(appPath));
    reg.setValue("Shell/Open/Command/.", QString("\"%1\"").arg(appPath) + " \"%1\"");
    reg.endGroup();
#else
    C_UNUSED(introduction)
#endif
}
