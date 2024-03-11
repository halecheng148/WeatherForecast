#include "qweatherforecast.h"

#include <weathernetwork.h>
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QWeatherForecast w;
//    w.show();

    WeatherNetWork nw;
    nw.request();

    return a.exec();
//    return 0;
}
