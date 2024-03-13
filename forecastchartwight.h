#ifndef FORECASTCHARTWIGHT_H
#define FORECASTCHARTWIGHT_H

#include <QWidget>
#include <QtCharts>

namespace Ui {
class ForecastChartWight;
}

class ForecastChartWight : public QWidget
{
    Q_OBJECT

public:
    explicit ForecastChartWight(QWidget *parent = 0);
    ~ForecastChartWight();

    void drawing(const QMap<QString,QMap<QString,QString>> &tempInfo);

signals:
    void forecastBtnCheckedSignal(QString week);

public slots:
    void forecastBtnCheckedSlot(QString week);

private:
    Ui::ForecastChartWight *ui;

    QChart *chart;
    QLineSeries *daySeries;
    QLineSeries *nightSeries;
    QValueAxis *valueAxis;
    QDateTimeAxis *dateTimeAxis;



};

#endif // FORECASTCHARTWIGHT_H
