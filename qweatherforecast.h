#ifndef QWEATHERFORECAST_H
#define QWEATHERFORECAST_H

#include <QMainWindow>
#include <QMouseEvent>

#include "ioxmlconfig.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
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

    /**
     * @brief request 通用请求
     * @param cityCode 默认值110000->北京
     * @param extensions 默认值base->实时天气类型，可选all->预报天气
     */
    void request(QString cityCode="110000",QString extensions="base");

    /**
     * @brief requestLive 内部调用request函数，相对与 extensions=base
     * @param cityCode 默认值110000 北京
     */
    void requestLive(QString cityCode="110000");

    /**
     * @brief requestForecast 内部调用request函数，相对与 extensions=all
     * @param cityCode 默认值110000 北京
     */
    void requestForecast(QString cityCode="110000");


private slots:
    /**
     * @brief on_switchModeBtn_clicked
     * 切换页面，有两个页面：（page="实时天气"、page_2="预报天气"
     */
    void on_switchModeBtn_clicked();

    /**
     * @brief on_refreshAction_triggered
     * 刷新活动
     */
    void on_refreshAction_triggered(bool checked);

    /**
     * @brief on_cityChangedActionChecked
     * 输入模式的切换
     */
    void on_cityChangedActionChecked(bool checked);

    void response();

private:
    Ui::QWeatherForecast *ui;

    QMenu *m_menu;  //右键菜单

    QString requestLink;
    QString key;
    QString extensions;
    QNetworkAccessManager *networkMgr;
};

#endif // QWEATHERFORECAST_H
