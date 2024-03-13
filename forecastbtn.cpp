#include "forecastbtn.h"
#include "ui_forecastbtn.h"
#include <QMouseEvent>
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
