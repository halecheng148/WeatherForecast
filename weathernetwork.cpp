#include "weathernetwork.h"

#include "jsonhandle.h"
#include <QDebug>
#include <QNetworkReply>

WeatherNetWork::WeatherNetWork()
{
    IOXmlConfig config;
    QMap<QString,QString> info = config.getWeatherApiConfig();
    requestLink = info["Link"];
    key = info["ApiKey"];
    extensions = info["extensions"];
    networkMgr = new QNetworkAccessManager;

}

WeatherNetWork::~WeatherNetWork()
{
    delete networkMgr;
    networkMgr = Q_NULLPTR;

}

void WeatherNetWork::request(QString cityCode, QString extensions)
{
    // 拼接link
    QString reqLink = requestLink;
    reqLink = reqLink + "key=" + key + "&city=" + cityCode + "&extensions="+extensions;

    // 封装并验证URL是否有效
    QUrl url(reqLink);
    if(!url.isValid())
    {
        qDebug() << __func__  << url.errorString();
        return;
    }

    //发送请求，创建响应，等待响应
    QNetworkReply *reply = networkMgr->get(QNetworkRequest(url));
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << __func__ << " : " << "reply err";
        return;
    }
    this->extensions = extensions;
    // 根据响应的信号绑定处理槽函数
    connect(reply,SIGNAL(finished()),this,SLOT(response()));
}

void WeatherNetWork::requestLive(QString cityCode)
{
    request(cityCode,"base");
}

void WeatherNetWork::requestForecast(QString cityCode)
{
    request(cityCode,"all");
}

QMap<QString, QMap<QString, QString> > WeatherNetWork::response()
{
    QMap<QString, QMap<QString, QString>> weathers;

    QByteArray json =static_cast<QNetworkReply*>(sender())->readAll();
    if(extensions=="base")
    {
        weathers = JsonHandle::weatherLivesJson(json);

    }else{
        weathers = JsonHandle::weatherForecastJson(json);

    }

    return weathers;
}


