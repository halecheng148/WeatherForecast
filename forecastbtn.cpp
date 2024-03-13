#include "forecastbtn.h"
#include "ui_forecastbtn.h"
#include <QMouseEvent>
#include <QDateTime>
ForecastBtn::ForecastBtn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForecastBtn)
{
    ui->setupUi(this);
}

ForecastBtn::ForecastBtn(QString week, QString weather, QString temp, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ForecastBtn)
{
    ui->setupUi(this);
    this->week = week;
    ui->weekLab->setText(weekHandle(week));
    ui->weatherLab->setText(weather);
    ui->tempLab->setText(temp);



    QString delimiter = "转";
    QStringList weathers = weather.split(delimiter,QString::SkipEmptyParts);
    if(weathers.count()==1)
    {
        ui->weatherLab->setProperty("weather",weather);
    }
    else
    {
        if(isDayNow())
        {
            ui->weatherLab->setProperty("weather",weathers.at(0));
        }
        else
        {
             ui->weatherLab->setProperty("weather",weathers.at(0));
        }
    }


}

ForecastBtn::~ForecastBtn()
{
    delete ui;
}

void ForecastBtn::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        emit checked(week);
    }
}

QString ForecastBtn::weekHandle(QString week)
{
    QString date = "";
    switch (week.toInt()) {
    case 1:
        date += "星期一";
    case 2:
        date += "星期二";
        break;
    case 3:
        date += "星期三";
        break;
    case 4:
        date += "星期四";
        break;
    case 5:
        date += "星期五";
        break;
    case 6:
        date += "星期六";
        break;
    case 7:
        date += "星期日";
        break;
    default:
        break;
    }
    return date;
}

bool ForecastBtn::isDayNow()
{
    QDateTime currentNow = QDateTime::currentDateTime();

    bool isDay = currentNow.time().hour() >= 6 && currentNow.time().hour() <= 18;

    return isDay;
}

