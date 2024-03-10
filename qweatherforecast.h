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

private slots:
    /**
     * @brief on_switchModeBtn_clicked
     * 切换页面，有两个页面：（page="实时天气"、page_2="预报天气"
     */
    void on_switchModeBtn_clicked();

private:
    Ui::QWeatherForecast *ui;
};

#endif // QWEATHERFORECAST_H
