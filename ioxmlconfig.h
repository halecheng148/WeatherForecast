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

    /**
     * @brief getRoot
     * @return 返回所有地区的edcode编码映射
     *
     */
    QMap<QString, QString> getRoot();

    /**
     * @brief getCityCodeByName
     * @param name 城市名
     * @return 返回城市的编码
     */
    QString getCityCodeByName(QString name);

    /**
     * @brief getCityNameByCode
     * @param code 城市编码
     * @return 返回城市名
     */
    QString getCityNameByCode(QString code);

    /**
     * @brief getAllProvince 获取所有省份 <name.code>
     * @return
     */
    QMap<QString,QString> getAllProvince();

    /**
     * @brief getCountyByCityCode
     * @param code 市级的code,code 6位格式 xxxx00
     * @return 返回该市的所有县区或县级市
     */
    QMap<QString,QString> getCountyByCityCode(QString code);

    /**
     * @brief getCityMapByProviceCode 需要
     * @param code 省份级别的code ,6位数中的后四位固定 ??0000
     * @return 返回改省份下的所有市级城市<name,code>映射，市级的code前两位和后两位固定 xxxx00。
     */
    QMap<QString,QString> getCityMapByProviceCode(QString code);

};

#endif // IOXMLCONFIG_H
