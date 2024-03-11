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
        qDebug() << __func__ << "  : file open error";
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
