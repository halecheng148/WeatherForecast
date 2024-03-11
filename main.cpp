#include "qweatherforecast.h"
#include "ioxmlconfig.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWeatherForecast w;
    w.show();
    return a.exec();

}
