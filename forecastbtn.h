#ifndef FORECASTBTN_H
#define FORECASTBTN_H

#include <QWidget>

namespace Ui {
class ForecastBtn;
}

class ForecastBtn : public QWidget
{
    Q_OBJECT

public:
    explicit ForecastBtn(QWidget *parent = 0);
    ForecastBtn(QString week,QString weather,QString temp,QWidget *parent = 0);
    ~ForecastBtn();

    QString weekHandle(QString week);

    /**
     * @brief isDayOrNightNow 判断当前是否为白天
     * @return
     */
    bool isDayNow();


signals:
    void checked(QString week);
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    Ui::ForecastBtn *ui;

    QString week;
};

#endif // FORECASTBTN_H
