#include "jsonhandle.h"
#include <QDebug>

JsonHandle::JsonHandle()
{

}

QMap<QString,QMap<QString,QString>> JsonHandle::weatherForecastJson(QByteArray data)
{

    QMap<QString,QMap<QString,QString>> ret;

    QJsonParseError jError;
    QJsonDocument jDoc = QJsonDocument::fromJson(data,&jError);

    if(jError.error != QJsonParseError::NoError)
    {
        qDebug() << __func__ << ": json err";
        return ret;
    }
    // {forecasts["0"{casts["0"{},"1"{},...,"3"{}]}]}
    QJsonObject jsonRoot = jDoc.object();
    // forecasts["0"{casts["0"{},"1"{},...,"3"{}]}]
    QJsonArray forecasts = jsonRoot["forecasts"].toArray();
    // "0"{casts["0"{},"1"{},...,"3"{}]}
    QJsonObject num0 = forecasts.at(0).toObject();
    // casts["0"{},"1"{},...,"3"{}]
    QJsonArray casts = num0["casts"].toArray();
    QJsonObject retNode;
    QStringList keys;
    for(int i=0;i<casts.count();i++)
    {
        //"0"{=,=,=,..},"1"{},...,"3"{}
        retNode = casts.at(i).toObject();
        keys = retNode.keys();
        QMap<QString,QString> map;
        for(int j=0;j<keys.count();j++)
        {
            map[keys.at(j)]=retNode[keys.at(j)].toString();
        }

        ret[QString::number(i)] = map;
    }

    // 还有一些基础信息 city-province-county-reporttime
    QMap<QString,QString> infoMap;
    keys = num0.keys();
    for(int i=0;i<keys.count();i++)
    {
        if(keys.at(i) == "casts")
            continue;
        infoMap[keys.at(i)] = num0[keys.at(i)].toString();
    }
    ret[QString::number(casts.count())] = infoMap;

    return ret;
}

QMap<QString,QMap<QString,QString>> JsonHandle::weatherLivesJson(QByteArray data)
{


    QMap<QString,QMap<QString,QString>> ret;

    QJsonParseError jError;
    QJsonDocument jDoc = QJsonDocument::fromJson(data,&jError);
    if(jError.error != QJsonParseError::NoError)
    {
        qDebug() << __func__ << ": json err";
        return ret;
    }

    QJsonObject jsonRoot = jDoc.object();

    QJsonArray jsonLives = jsonRoot["lives"].toArray();
    QJsonObject jsonLive;
    QMap<QString,QString> map;
    QStringList keys;
    for(int i=0;i<jsonLives.count();i++)
    {
        jsonLive = jsonLives.at(0).toObject();
        keys = jsonLive.keys();

        for(int j=0;j<keys.count();j++)
        {
            map[keys.at(j)] = jsonLive[keys.at(j)].toString();
        }
        ret["0"]=map;
    }



    return ret;
}
