#ifndef JSONHANDLE_H
#define JSONHANDLE_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
class JsonHandle
{
public:
    JsonHandle();

    /**
     * @brief weatherLivesJson Lives["0"{...}]
     * @param data
     * @return 返回实时天气
     */
    static QMap<QString,QMap<QString,QString>> weatherLivesJson(QByteArray data);

    /**
     * @brief weatherForecastJson {forecasts["0"{casts["0"{},"1"{},...,"3"{}]}]}
     * @param data
     * @return 返回预报天气
     */
    static QMap<QString,QMap<QString,QString>> weatherForecastJson(QByteArray data);
};

#endif // JSONHANDLE_H
