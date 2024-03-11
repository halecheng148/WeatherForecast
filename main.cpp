#include "qweatherforecast.h"
#include "ioxmlconfig.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    QWeatherForecast w;
//    w.show();

    IOXmlConfig config;
    QMap<QString,QString> map = config.getCityMapByProviceCode("370000");
    QMap<QString,QString>::const_iterator i;
    for(i=map.constBegin();i!=map.constEnd();i++)
    {
        qDebug() << i.key() << " : " << i.value();
    }

//    return a.exec();
    return 0;
}
