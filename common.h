#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace Common
{
    QString getFileText(const QString &a_filename);
    bool setFileText(const QString &a_filename, const QString &a_text);

    QJsonObject getJsonFromString(const QString a_string);
    QString getStringFromJson(const QJsonObject& a_json);
}

#endif // COMMON_H
