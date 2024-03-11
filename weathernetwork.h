#ifndef WEATHERNETWORK_H
#define WEATHERNETWORK_H

#include "ioxmlconfig.h"
#include <QNetworkAccessManager>
class WeatherNetWork:public QObject
{
    Q_OBJECT
private:
    QString requestLink;
    QString key;
    QString extensions;
    QNetworkAccessManager *networkMgr;

public:
    WeatherNetWork();
    ~WeatherNetWork();

    /**
     * @brief request 通用请求
     * @param cityCode 默认值110000->北京
     * @param extensions 默认值base->实时天气类型，可选all->预报天气
     */
    void request(QString cityCode="110000",QString extensions="base");

    /**
     * @brief requestLive 内部调用request函数，相对与 extensions=base
     * @param cityCode 默认值110000 北京
     */
    void requestLive(QString cityCode="110000");

    /**
     * @brief requestForecast 内部调用request函数，相对与 extensions=all
     * @param cityCode 默认值110000 北京
     */
    void requestForecast(QString cityCode="110000");



private slots:
    QMap<QString,QMap<QString,QString>> response();



};

#endif // WEATHERNETWORK_H
