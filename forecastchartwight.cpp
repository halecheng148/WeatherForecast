#include "forecastchartwight.h"
#include "ui_forecastchartwight.h"
#include "forecastbtn.h"
#include <QMap>
ForecastChartWight::ForecastChartWight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForecastChartWight)
{
    ui->setupUi(this);
    chart = new QChart;
    chart->setTitle("温度变化");
    daySeries = new QLineSeries;
    nightSeries = new QLineSeries;
    valueAxis = new QValueAxis;
    dateTimeAxis = new QDateTimeAxis;
    ui->graphicsView->setChart(chart);
    chart->addAxis(dateTimeAxis,Qt::AlignBottom);
    chart->addAxis(valueAxis,Qt::AlignRight);
    chart->addSeries(daySeries);
    chart->addSeries(nightSeries);

    setObjectName("ForecastChartWight");
}

ForecastChartWight::~ForecastChartWight()
{
    delete daySeries;
    delete nightSeries;
    delete valueAxis;
    delete dateTimeAxis;
    delete ui;
}

void ForecastChartWight::drawing(const QMap<QString, QMap<QString, QString>> &tempInfo)
{


    daySeries->setName("日间温度");
    nightSeries->setName("夜间温度");
    dateTimeAxis->setRange(QDateTime::fromString(tempInfo["0"]["date"],"yyyy-MM-d"),
            QDateTime::fromString(tempInfo["3"]["date"],"yyyy-MM-d"));
    int count = tempInfo.count()-1;

    int maxTemp = tempInfo["0"]["daytemp"].toInt() > tempInfo["0"]["nighttemp"].toInt()?tempInfo["0"]["daytemp"].toInt():tempInfo["0"]["nighttemp"].toInt();
    int minTemp = tempInfo["0"]["daytemp"].toInt() < tempInfo["0"]["nighttemp"].toInt()?tempInfo["0"]["daytemp"].toInt():tempInfo["0"]["nighttemp"].toInt();
    for(int i=0;i<count;i++)
    {
        QMap<QString,QString> map = tempInfo[QString::number(i)];
        qint64 date = QDateTime::fromString(map["date"],"yyyy-MM-d").toMSecsSinceEpoch();
        daySeries->append(date,map["daytemp"].toInt());
        nightSeries->append(date,map["nighttemp"].toInt());

        maxTemp = map["daytemp"].toInt() > maxTemp?map["daytemp"].toInt():maxTemp;
        minTemp =  minTemp < map["nighttemp"].toInt()?minTemp:map["nighttemp"].toInt();


        // 添加按钮
        // 数据准备：
        QString weather;
        if(map["dayweather"]!=map["nightweather"])
            weather = map["dayweather"]+"转"+map["nightweather"];
        else
            weather = map["dayweather"];
        QString week = map["week"];
        QString tempBtn = map["daytemp"]+"°C~"+map["nighttemp"]+"°C";
        ForecastBtn *btn = new ForecastBtn(week,weather,tempBtn,this);

        // 监听鼠标右键单击事件,传递信息
        connect(btn,&ForecastBtn::checked,this,&ForecastChartWight::forecastBtnCheckedSlot);

        ui->horizontalLayout->addWidget(btn);

    }
    valueAxis->setRange(minTemp-2,maxTemp+2);

    daySeries->attachAxis(valueAxis);
    daySeries->attachAxis(dateTimeAxis);
    nightSeries->attachAxis(valueAxis);
    nightSeries->attachAxis(dateTimeAxis);

}

void ForecastChartWight::forecastBtnCheckedSlot(QString week)
{
    emit forecastBtnCheckedSignal(week);
}



