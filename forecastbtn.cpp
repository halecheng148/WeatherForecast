#include "forecastbtn.h"
#include "ui_forecastbtn.h"

ForecastBtn::ForecastBtn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForecastBtn)
{
    ui->setupUi(this);
}

ForecastBtn::~ForecastBtn()
{
    delete ui;
}
