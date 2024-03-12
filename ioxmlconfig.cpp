#include "ioxmlconfig.h"

#include <QDebug>
#include <QMap>
IOXmlConfig::IOXmlConfig()
{

}

QMap<QString, QString> IOXmlConfig::getWeatherApiConfig()
{
    QMap<QString, QString> map;
    QFile file(":/Config/WeatherApi.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return map;
    }


    QString *errmgr = new QString();
    if(!xmlConfigDocument.setContent(&file,errmgr))
    {
        qDebug() << errmgr;
        delete errmgr;
        return map;
    }
    delete errmgr;

    QDomElement root = xmlConfigDocument.documentElement();
    QDomNodeList rootChild= root.childNodes();
    for(int i=0;i<rootChild.length();i++)
    {
        QDomNode node = rootChild.at(i);
        QDomElement nodeElement = node.toElement();
        QString name = nodeElement.tagName();
        QString value = nodeElement.attribute(name);
        map[name]=value;
    }

    return map;
}

QMap<QString, QString> IOXmlConfig::getRoot()
{
    QMap<QString, QString> infoMap;
    QFile file(":/Config/CityCodeInfo.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << __func__ << "  : file open error";
        return infoMap;
    }

    QString *errmgr = new QString();
    if(!xmlConfigDocument.setContent(&file,errmgr))
    {
        qDebug() << errmgr;
        delete errmgr;
        return infoMap;
    }
    delete errmgr;

    QDomElement root = xmlConfigDocument.documentElement();
    QDomNodeList rootChild= root.childNodes();
    QDomNode node;
    QDomElement nodeElement;
    QString name;
    QString code;
    for(int i=0;i<rootChild.length();i++)
    {
        node = rootChild.at(i);
        nodeElement = node.toElement();
        name = nodeElement.attribute("Name");
        code = nodeElement.attribute("Code");
        infoMap[code]=name;
    }

    return infoMap;
}

QString IOXmlConfig::getCityCodeByName(QString name)
{
    QString ret = "";
    QMap<QString,QString> root = getRoot();
    QMap<QString,QString>::const_iterator i;

    for(i=root.constBegin();i!=root.constEnd();i++)
    {
        if(i.value().contains(name,Qt::CaseInsensitive))
        {
            ret = i.key();
            break;
        }
    }
    return ret;
}

QString IOXmlConfig::getCityNameByCode(QString code)
{
    QString ret = "";
    QMap<QString,QString> root = getRoot();
    QMap<QString,QString>::const_iterator i;

    for(i=root.constBegin();i!=root.constEnd();i++)
    {
        if(i.key() == code)
        {
            ret = i.value();
            break;
        }
    }
    return ret;
}

QMap<QString, QString> IOXmlConfig::getAllProvince()
{
    QMap<QString,QString> ret;
    QMap<QString,QString> root = getRoot();
    QMap<QString,QString>::const_iterator i;

    QStringList keyCode = root.keys();
    int code;
    int count = root.count();
    QString codeStr;
    for(int i=0 ; i < count ; i++ )
    {
        codeStr = keyCode.at(i);
        code = codeStr.toInt();
        code = code%10000;
        if(code == 0)
            ret[codeStr] = root[codeStr];
    }
    return ret;
}

QMap<QString, QString> IOXmlConfig::getCountyByCityCode(QString code)
{
    QMap<QString, QString> ret;
    QMap<QString,QString> root = getRoot();
    QMap<QString,QString>::const_iterator i;

    // xxxx??
    int cityCode = code.toInt();
    cityCode = cityCode/100;
    // 文件中城市的县区都在一块，从发现符合 到 copy完最后一个，不需要遍历到最后
    // 使用标识logo;0/1
    int logo=0;
    for(i=root.constBegin();i!=root.constEnd();i++)
    {
        int codeNum = i.key().toInt();
        codeNum = codeNum/100;
        if(codeNum==cityCode)
        {
            if(i.key()==code)
                continue;
            logo=1;
            ret[i.key()]=i.value();
        }
        if(logo==1&&codeNum!=cityCode)
            break;
    }
    return ret;
}

QMap<QString, QString> IOXmlConfig::getCityMapByProviceCode(QString code)
{
    QMap<QString, QString> ret;
    QMap<QString,QString> root = getRoot();
    QMap<QString,QString>::const_iterator i;

    int funcCode = code.toInt();
    int proviceCode = funcCode/10000; // 前两位代表省份
    int countyCode = 0; //后两位代表县级

    int logo = 0;

    for(i=root.constBegin();i!=root.constEnd();i++)
    {
        int codeNum = i.key().toInt();
        int codeNumProvice = codeNum/10000;
        int codeNumCounty = codeNum%100;
        if(codeNumProvice == proviceCode && countyCode == codeNumCounty)
        {
            logo=1;
            if(i.key()==code)
                continue;
            ret[i.key()]=i.value();
        }
        if(proviceCode!=codeNumProvice && logo==1)
            break;
    }
    return ret;
}
