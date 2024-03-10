#ifndef QWEATHERFORECAST_H
#define QWEATHERFORECAST_H

#include <QMainWindow>

namespace Ui {
class QWeatherForecast;
}

class QWeatherForecast : public QMainWindow
{
    Q_OBJECT

public:
    explicit QWeatherForecast(QWidget *parent = 0);
    ~QWeatherForecast();

private:
    Ui::QWeatherForecast *ui;
};

#endif // QWEATHERFORECAST_H
