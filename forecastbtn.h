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
    ~ForecastBtn();

private:
    Ui::ForecastBtn *ui;
};

#endif // FORECASTBTN_H
