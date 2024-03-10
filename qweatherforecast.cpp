#include "qweatherforecast.h"
#include "ui_qweatherforecast.h"

QWeatherForecast::QWeatherForecast(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QWeatherForecast)
{
    ui->setupUi(this);
}

QWeatherForecast::~QWeatherForecast()
{
    delete ui;
}
