#include "qweatherforecast.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWeatherForecast w;
    w.show();
    return a.exec();

}
