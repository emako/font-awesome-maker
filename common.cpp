#include "common.h"

QString Common::getFileText(const QString &a_filename)
{
    QFile file(a_filename);
    QTextStream in(&file);
    QString text;

    if (!file.open(QIODevice::ReadOnly))
    {
        return text;
    }
    text = in.readAll();
    file.close();
    return text;
}

bool Common::setFileText(const QString &a_filename, const QString &a_text)
{
    QFile file(a_filename);
    QTextStream in(&file);

    in.setCodec("UTF-8");
    in.setGenerateByteOrderMark(false);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    in << a_text;
    file.close();
    return true;
}

QJsonObject Common::getJsonFromString(const QString a_string)
{
    QJsonDocument json_document = QJsonDocument::fromJson(a_string.toLocal8Bit().data());

    if(json_document.isNull())
    {
        qDebug()<< "Illegal json string: "<< a_string.toLocal8Bit().data();
    }

    QJsonObject json = json_document.object();
    return json;
}

QString Common::getStringFromJson(const QJsonObject& a_json)
{
    return QString(QJsonDocument(a_json).toJson());
}
