#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include "cglobal.h"

#include <QStringList>

MCD_BEGIN_NAMESPACE

class MCD_EXPORT CWifiManager
{
public:
    CWifiManager(const QString &deviceName);

    bool enableHotspot();
    bool disableHotspot();

    bool isWifiEnabled() const;
    bool setWifiEnabled(bool enabled);

    QStringList detectServer();
    bool connectToServer(const QString &server);
    QString currentServer() const;

    void setSsidPrefix(const QString &prefix) { m_ssidPrefix = prefix; }
    QString ssidPrefix() const { return m_ssidPrefix; }

    void setKey(const QString &key) { m_key = key; }
    QString key() const { return m_key; }

private:
    QString m_deviceName;
    QString m_ssidPrefix;
    QString m_key;
};

MCD_END_NAMESPACE

#endif // WIFIMANAGER_H
