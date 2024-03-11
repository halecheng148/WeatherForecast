#include "qweatherforecast.h"
#include "ui_qweatherforecast.h"

#include "jsonhandle.h"
QWeatherForecast::QWeatherForecast(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QWeatherForecast)
{
    ui->setupUi(this);
    // 因两种输入方式同时存在，所以隐去一个，会影响布局的形式隐去，（不会影响的使用QSS）
    ui->selectModeWgt->setVisible(0);
    // 右键菜单+输入切换 + 刷新
    m_menu = new QMenu;
    m_menu->addAction(ui->refreshAction);
    m_menu->addAction(ui->cityChangedAction);
    // 输入切换 Action + triggered()
    connect(ui->cityChangedAction,SIGNAL(triggered(bool)),this,SLOT(on_cityChangedActionChecked(bool)));

    // weatherNetWork 转移的，用于网络配置的初始化
    IOXmlConfig config;
    QMap<QString,QString> info = config.getWeatherApiConfig();
    requestLink = info["Link"];
    key = info["ApiKey"];
    extensions = info["extensions"];
    networkMgr = new QNetworkAccessManager;


    // 请求天气初始化页面
    request();


}

QWeatherForecast::~QWeatherForecast()
{
    //网络管理delete
    delete networkMgr;
    networkMgr = Q_NULLPTR;

    delete ui;
}

void QWeatherForecast::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        m_menu->exec(QCursor::pos());
    }
    QWidget::mousePressEvent(event);
}

void QWeatherForecast::on_switchModeBtn_clicked()
{
    QString strText = ui->switchModeBtn->text();

    if(strText == "实时天气")
        strText = "预报天气";
    else
        strText = "实时天气";


    QWidget* page = ui->stackedWidget->currentWidget();
    if(ui->page == page)
        page = ui->page_2;
    else
        page = ui->page;

    ui->switchModeBtn->setText(strText);
    ui->stackedWidget->setCurrentWidget(page);
}

void QWeatherForecast::on_refreshAction_triggered(bool checked)
{

}

void QWeatherForecast::on_cityChangedActionChecked(bool checked)
{
    Q_UNUSED(checked)
    bool ComboHide = ui->selectModeWgt->isHidden();
    if(ComboHide)
    {
        ui->cityEdit->setVisible(0);
        ui->selectModeWgt->setVisible(1);
    }
    else
    {
        ui->cityEdit->setVisible(1);
        ui->selectModeWgt->setVisible(0);
    }
}

void QWeatherForecast::request(QString cityCode, QString extensions)
{
    // 拼接link
    QString reqLink = requestLink;
    reqLink = reqLink + "key=" + key + "&city=" + cityCode + "&extensions="+extensions;

    // 封装并验证URL是否有效
    QUrl url(reqLink);
    if(!url.isValid())
    {
        qDebug() << __func__  << url.errorString();
        return;
    }

    //发送请求，创建响应，等待响应
    QNetworkReply *reply = networkMgr->get(QNetworkRequest(url));
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << __func__ << " : " << "reply err";
        return;
    }
    this->extensions = extensions;
    // 根据响应的信号绑定处理槽函数
    connect(reply,SIGNAL(finished()),this,SLOT(response()));
}

void QWeatherForecast::requestLive(QString cityCode)
{
    request(cityCode,"base");
}

void QWeatherForecast::requestForecast(QString cityCode)
{
    request(cityCode,"all");
}

void QWeatherForecast::response()
{
    QMap<QString, QMap<QString, QString>> weathers;

    QByteArray json =static_cast<QNetworkReply*>(sender())->readAll();
    if(extensions=="base")
    {
        weathers = JsonHandle::weatherLivesJson(json);
        if(weathers.isEmpty())
        {
            qDebug() << "NULL" << __func__;
        }

        QMap<QString,QString> map = weathers["0"];
        if(map.isEmpty())
        {
            qDebug() << "NULL" << __func__;
        }
        QString temp = map["temperature_float"] + "°C";
        ui->liveTempLab->setText(temp);

        ui->liveHumidityLab->setText(map["humidity_float"]);
        int adcode = map["adcode"].toInt();
        int cityViewByNum = adcode%10000;
         QString provinceCity;
        if(cityViewByNum==0)
            provinceCity=map["city"];
        else
            provinceCity=map["province"]+"-"+map["city"];

        ui->liveWeatherCityLab->setText(provinceCity);

        ui->liveWeatherLab->setText(map["weather"]);
        ui->liveWindDirectionLab->setText(map["winddirection"]);
        ui->liveWindPowerLab->setText(map["windpower"]);
        ui->reportTimeLab->setText(map["reporttime"]);

    }else{
        weathers = JsonHandle::weatherForecastJson(json);
    }



}

