#ifndef QWEATHERFORECAST_H
#define QWEATHERFORECAST_H

#include <QMainWindow>
#include <QMouseEvent>
namespace Ui {
class QWeatherForecast;
}

class QWeatherForecast : public QMainWindow
{
    Q_OBJECT

public:
    explicit QWeatherForecast(QWidget *parent = 0);
    ~QWeatherForecast();

protected:
    /**
     * @brief mousePressEvent
     * @param event
     * 使用鼠标事件，创建右键菜单
     */
    void mousePressEvent(QMouseEvent *event);

private slots:
    /**
     * @brief on_switchModeBtn_clicked
     * 切换页面，有两个页面：（page="实时天气"、page_2="预报天气"
     */
    void on_switchModeBtn_clicked();



private:
    Ui::QWeatherForecast *ui;

    QMenu *m_menu;  //右键菜单
};

#endif // QWEATHERFORECAST_H
