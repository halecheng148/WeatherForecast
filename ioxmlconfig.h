#ifndef IOXMLCONFIG_H
#define IOXMLCONFIG_H


#include <QDomDocument>
#include <QFile>

class IOXmlConfig
{
private:
    QDomDocument xmlConfigDocument;
public:
    IOXmlConfig();

    /**
     * @brief getWeatherApiConfig
     * @return 返回api配置信息
     */
    QMap<QString,QString> getWeatherApiConfig();
};

#endif // IOXMLCONFIG_H
