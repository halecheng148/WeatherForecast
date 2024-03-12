#include "qweatherforecast.h"
#include "ui_qweatherforecast.h"

#include "jsonhandle.h"

QWeatherForecast::QWeatherForecast(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QWeatherForecast)
{
    ui->setupUi(this);

    initialization();

}

QWeatherForecast::~QWeatherForecast()
{
    //网络管理delete
    delete networkMgr;
    networkMgr = Q_NULLPTR;

    delete cityEditCompleter;
    cityEditCompleter = Q_NULLPTR;

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

void QWeatherForecast::initialization()
{
    // 指定起始页面
    ui->stackedWidget->setCurrentWidget(ui->page);

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

    networkMgr = new QNetworkAccessManager;


    // 请求天气初始化页面
    request();


    // 为输入文本框绑定QCompleter
    // 1.获取数据
    QMap<QString,QString> cityAll = config.getRoot();
    QStringList cityNameAll = cityAll.values();
    // 2.创建QCompleter
    cityEditCompleter = new QCompleter(cityNameAll);
    // 3.设置过滤模式 - 搜索词保存在项目中
    cityEditCompleter->setFilterMode(Qt::MatchContains);
    // 4.为文本框绑定
    ui->cityEdit->setCompleter(cityEditCompleter);


    // 为ProvinceComBox添加item
    // 数据准备
    QMap<QString,QString> provinceMap = config.getAllProvince();
    QMap<QString,QString>::const_iterator iter_Prov;
    ui->provinceComboBox->addItem("空","");

    for(iter_Prov=provinceMap.constBegin();iter_Prov != provinceMap.constEnd();iter_Prov++)
    {
        ui->provinceComboBox->addItem(iter_Prov.value(),iter_Prov.key());
    }
}

void QWeatherForecast::response()
{
    QMap<QString, QMap<QString, QString>> weathers;

    QByteArray json =static_cast<QNetworkReply*>(sender())->readAll();
    QString extensions;

    if(ui->stackedWidget->currentWidget() == ui->page)
    {
        extensions = "base";
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
        extensions = "all";
        weathers = JsonHandle::weatherForecastJson(json);
    }



}


void QWeatherForecast::on_provinceComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    // 获取code

    QString code = ui->provinceComboBox->currentData().toString();
    // 验证
    if(code.isEmpty())
    {
        ui->cityComboBox->clear();
        return;
    }


    // 天气请求
    QString extensions;
    if(ui->stackedWidget->currentWidget() == ui->page)
        extensions = "base";
    else
        extensions = "all";

    request(code,extensions);

    // cityCombox绑定
    // 前清空之前的
    ui->cityComboBox->clear();
    IOXmlConfig config;
    QMap<QString,QString> cityMap = config.getCityMapByProviceCode(code);
    QMap<QString,QString>::const_iterator iter_city;
    ui->cityComboBox->addItem("空","");
    for(iter_city=cityMap.constBegin();iter_city!=cityMap.constEnd();iter_city++)
    {
        ui->cityComboBox->addItem(iter_city.value(),iter_city.key());
    }
}

void QWeatherForecast::on_cityComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    QString code = ui->cityComboBox->currentData().toString();
    // 验证
    if(code.isEmpty())
    {
        ui->countyComboBox->clear();
        return;
    }

    // 天气请求
    QString extensions;
    if(ui->stackedWidget->currentWidget() == ui->page)
        extensions = "base";
    else
        extensions = "all";

    request(code,extensions);

    // countyComboBox绑定
    // 前清空之前的
    ui->countyComboBox->clear();
    IOXmlConfig config;
    QMap<QString,QString> cityMap = config.getCountyByCityCode(code);
    QMap<QString,QString>::const_iterator iter_county;
     ui->countyComboBox->addItem("空","");
    for(iter_county=cityMap.constBegin();iter_county!=cityMap.constEnd();iter_county++)
    {
        ui->countyComboBox->addItem(iter_county.value(),iter_county.key());
    }
}
