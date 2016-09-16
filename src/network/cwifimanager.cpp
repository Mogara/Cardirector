/********************************************************************
    Copyright (c) 2013-2016 - Mogara

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

#include "cwifimanager.h"

#ifdef Q_OS_ANDROID

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

namespace {
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject manager = activity.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", QAndroidJniObject::fromString("wifi").object<jstring>());
}

#elif defined(Q_OS_WIN)  && !defined(Q_OS_WINRT)

#include <QProcess>
#include <QTextStream>
#include <windows.h>
#include <shellapi.h>
#ifdef _MSC_VER
#pragma comment(lib, "Shell32.lib")
#endif

void seek(QTextStream &stream, char target){
    char ch;
    do {
        stream >> ch;
    } while(ch != target);
    stream.skipWhiteSpace();
}

#endif

CWifiManager::CWifiManager(const QString &deviceName)
    : m_deviceName(deviceName)
    , m_ssidPrefix(QStringLiteral("Cardirector-"))
    , m_key(QStringLiteral("org.mogara.cardirector"))
{
}

bool CWifiManager::enableHotspot()
{
#ifdef Q_OS_ANDROID
    setWifiEnabled(false);

    QAndroidJniObject ssid = QAndroidJniObject::fromString(QString("%1%2").arg(m_ssidPrefix).arg(m_deviceName));
    QAndroidJniObject key = QAndroidJniObject::fromString(m_key);

    QAndroidJniObject config("android/net/wifi/WifiConfiguration");
    config.setField<jstring>("SSID", ssid.object<jstring>());
    config.setField<jstring>("preSharedKey", key.object<jstring>());

    //@todo: Replace integer constants with constant identifiers
    config.setField<jint>("status", 2);
    QAndroidJniObject allowedGroupCiphers = config.getObjectField("allowedGroupCiphers", "Ljava/util/BitSet;");
    allowedGroupCiphers.callMethod<void>("set", "(I)V", 2);
    allowedGroupCiphers.callMethod<void>("set", "(I)V", 3);
    QAndroidJniObject allowedKeyManagement = config.getObjectField("allowedKeyManagement", "Ljava/util/BitSet;");
    allowedKeyManagement.callMethod<void>("set", "(I)V", 1);
    QAndroidJniObject allowedPairwiseCiphers = config.getObjectField("allowedPairwiseCiphers", "Ljava/util/BitSet;");
    allowedPairwiseCiphers.callMethod<void>("set", "(I)V", 1);
    allowedPairwiseCiphers.callMethod<void>("set", "(I)V", 2);
    QAndroidJniObject allowedProtocols = config.getObjectField("allowedProtocols", "Ljava/util/BitSet;");
    allowedProtocols.callMethod<void>("set", "(I)V", 1);

    return manager.callMethod<jboolean>("setWifiApEnabled", "(Landroid/net/wifi/WifiConfiguration;Z)Z", config.object(), true);
#elif defined(Q_OS_WIN) && !defined(Q_OS_WINRT)
    QStringList commands;
    commands << QStringLiteral("netsh wlan stop hostednetwork");
    commands << QStringLiteral("netsh wlan set hostednetwork mode=allow ssid=%1%2 key=%3").arg(m_ssidPrefix).arg(m_deviceName).arg(m_key);
    commands << QStringLiteral("netsh wlan start hostednetwork");

    QString argument = QStringLiteral("/c \"%1\"").arg(commands.join(QStringLiteral(" && ")));
    ShellExecuteW(0, L"runas", L"cmd", argument.toStdWString().data(), 0, SW_HIDE);

    return true;
#else
    return false;
#endif
}

bool CWifiManager::disableHotspot()
{
#ifdef Q_OS_ANDROID
    return manager.callMethod<jboolean>("setWifiApEnabled", "(Landroid/net/wifi/WifiConfiguration;Z)Z", NULL, false);
#elif defined(Q_OS_WIN) && !defined(Q_OS_WINRT)
    ShellExecuteW(0, L"runas", L"netsh", L"wlan stop hostednetwork", 0, SW_HIDE);
    return true;
#else
    return false;
#endif
}

bool CWifiManager::isWifiEnabled() const
{
#ifdef Q_OS_ANDROID
    return manager.callMethod<jboolean>("isWifiEnabled");
#else
    return false;
#endif
}

bool CWifiManager::setWifiEnabled(bool enabled)
{
#ifdef Q_OS_ANDROID
    return manager.callMethod<jboolean>("setWifiEnabled", "(Z)Z", enabled);
#else
    (void) enabled;
    return false;
#endif
}

QStringList CWifiManager::detectServer()
{
    if (!isWifiEnabled())
        setWifiEnabled(true);

    QStringList hotspots;
#ifdef Q_OS_ANDROID
    QAndroidJniObject results = manager.callObjectMethod("getScanResults", "()Ljava/util/List;");
    int length = results.callMethod<jint>("size");
    for (int i = 0; i < length; i++) {
        QAndroidJniObject result = results.callObjectMethod("get", "(I)Ljava/lang/Object;", i);
        QString ssid = result.getObjectField("SSID", "Ljava/lang/String;").toString();
        if (ssid.startsWith(m_ssidPrefix)) {
            ssid.remove(0, m_ssidPrefix.length());
            hotspots << ssid;
        }
    }
#elif defined(Q_OS_WIN) && !defined(Q_OS_WINRT)
    QProcess netsh;
    netsh.start(QStringLiteral("netsh wlan show networks"));
    if (netsh.waitForFinished()) {
        QTextStream stream(&netsh);
        QString ssid;
        while (!stream.atEnd()) {
            stream >> ssid;
            if (ssid == QStringLiteral("SSID")) {
                seek(stream, ':');
                stream >> ssid;

                if (!m_ssidPrefix.isEmpty() && !ssid.startsWith(m_ssidPrefix)) {
                    seek(stream, '\n');
                    seek(stream, '\n');
                    seek(stream, '\n');
                    continue;
                }

                ssid.remove(0, m_ssidPrefix.length());
                hotspots << ssid;
            }
        }
    }
#endif
    return hotspots;
}

QString CWifiManager::currentServer() const
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject currentConnection = manager.callObjectMethod("getConnectionInfo", "()Landroid/net/wifi/WifiInfo;");
    QString currentSsid = currentConnection.callObjectMethod("getSSID", "()Ljava/lang/String;").toString();
    if (currentSsid.startsWith(m_ssidPrefix))
        return currentSsid.mid(m_ssidPrefix.length());
#endif

    return QString();
}

bool CWifiManager::connectToServer(const QString &server)
{
    QString currentSsid = currentServer();
    if (currentSsid == server)
        return true;

#ifdef Q_OS_ANDROID
    QAndroidJniObject ssid = QAndroidJniObject::fromString(QString("\"%1%2\"").arg(m_ssidPrefix).arg(server));
    QAndroidJniObject key = QAndroidJniObject::fromString(QString("\"%1\"").arg(m_key));

    QAndroidJniObject configs = manager.callObjectMethod("getConfiguredNetworks", "()Ljava/util/List;");
    int length = configs.callMethod<jint>("size");
    int networkId = -1;
    for (int i = 0; i < length; i++) {
        QAndroidJniObject config = configs.callObjectMethod("get", "(I)Ljava/lang/Object;", i);
        if (config.getObjectField("SSID", "Ljava/lang/String;").toString() == "TETSU") {
            networkId = config.getField<jint>("networkId");
            break;
        }
    }

    if (networkId == -1) {
        QAndroidJniObject config("android/net/wifi/WifiConfiguration");
        config.setField<jstring>("SSID", ssid.object<jstring>());
        config.setField<jstring>("preSharedKey", key.object<jstring>());

        //@todo: Replace integer constants with constant identifiers
        config.setField<jint>("status", 2);
        QAndroidJniObject allowedGroupCiphers = config.getObjectField("allowedGroupCiphers", "Ljava/util/BitSet;");
        allowedGroupCiphers.callMethod<void>("set", "(I)V", 2);
        allowedGroupCiphers.callMethod<void>("set", "(I)V", 3);
        QAndroidJniObject allowedKeyManagement = config.getObjectField("allowedKeyManagement", "Ljava/util/BitSet;");
        allowedKeyManagement.callMethod<void>("set", "(I)V", 1);
        QAndroidJniObject allowedPairwiseCiphers = config.getObjectField("allowedPairwiseCiphers", "Ljava/util/BitSet;");
        allowedPairwiseCiphers.callMethod<void>("set", "(I)V", 1);
        allowedPairwiseCiphers.callMethod<void>("set", "(I)V", 2);
        QAndroidJniObject allowedProtocols = config.getObjectField("allowedProtocols", "Ljava/util/BitSet;");
        allowedProtocols.callMethod<void>("set", "(I)V", 1);

        networkId = manager.callMethod<jint>("addNetwork", "(Landroid/net/wifi/WifiConfiguration;)I", config.object());
    }

    return manager.callMethod<jboolean>("enableNetwork", "(IZ)Z", networkId, true);
#else
    return false;
#endif
}
