#include "qweatherforecast.h"
#include "ui_qweatherforecast.h"



#include "jsonhandle.h"

#include <QMessageBox>
QWeatherForecast::QWeatherForecast(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QWeatherForecast)
{
    ui->setupUi(this);

    initialization();
    this->setWindowTitle("天气预报");
}

QWeatherForecast::~QWeatherForecast()
{
    //网络管理delete
    delete networkMgr;
    networkMgr = Q_NULLPTR;

    delete cityEditCompleter;
    cityEditCompleter = Q_NULLPTR;
    delete fcWidget;
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
    {
        ui->switchModeBtn->setText(strText);
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        emit ui->refreshAction->triggered(true);
    }
    else
    {
        ui->switchModeBtn->setText(strText);
        ui->stackedWidget->setCurrentWidget(ui->page);
        emit ui->refreshAction->triggered(true);
    }


}

void QWeatherForecast::on_refreshAction_triggered(bool checked)
{
    Q_UNUSED(checked)
    // 数据准备
    QString code = "110000"; // 默认北京市
    QString extensions;
    if(ui->stackedWidget->currentWidget() == ui->page)
        extensions = "base";
    else
        extensions = "all";



    // 有两种输入模式，那个可见选哪个
    if(ui->cityEdit->isVisible())
    {
        // 输入文本框mode
        QString editName = ui->cityEdit->text();
        if(editName.isEmpty())
        {
             // 使用默认的数据刷新
            request(code,extensions);
        }
        else
        {
             IOXmlConfig config;
             code = config.getCityCodeByName(editName);
             if(code.isEmpty())
             {
                 QMessageBox::warning(this,"未响应","输入有误",QMessageBox::Ok);
                 ui->cityEdit->clear();
                 return;
             }

             request(code,extensions);
        }
    }
    else
    {

        // 下拉选框mode
        QString countyCode = ui->countyComboBox->currentData().toString();
        QString cityCode = ui->cityComboBox->currentData().toString();
        QString provinceCode = ui->provinceComboBox->currentData().toString();
        if(!countyCode.isEmpty())
        {
            request(countyCode,extensions);
        }
        else if(!cityCode.isEmpty())
        {
            request(cityCode,extensions);
        }
        else if(!provinceCode.isEmpty())
        {
            request(provinceCode,extensions);
        }
        else
        {
            // 默认
            request(code,extensions);
        }

    }
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

void QWeatherForecast::drawingChartWidget(const QMap<QString, QMap<QString, QString> > &tempInfo)
{

    delete fcWidget;
    fcWidget = nullptr;
    fcWidget = new ForecastChartWight(this);
    fcWidget->drawing(tempInfo);
    ui->gridLayout->addWidget(fcWidget,3,0,1,3);
    ui->gridLayout->setRowStretch(3,3);
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
            qDebug() << "weathers NULL" << __func__;
        }

        QMap<QString,QString> map = weathers["0"];
        if(map.isEmpty())
        {
            qDebug() << " map NULL" << __func__;
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
        // 预测天气，数据显示，后续的绘画都需要在这实现
        extensions = "all";
        weathers = JsonHandle::weatherForecastJson(json);
        if(weathers.isEmpty())
        {
            qDebug() << " weathers NULL" << __func__;
        }

        QMap<QString,QString> map = weathers["0"];
        if(map.isEmpty())
        {
            qDebug() << " map NULL" << __func__;
        }

        QMap<QString,QString> info = weathers["4"];

        QString cpAddr = info["province"] + "-" + info["city"];
        ui->forecastWeatherCityLab->setText(cpAddr);

        QMap<QString,QString> initMap = weathers["0"];
        QString date = initMap["date"] + " ";
        switch (initMap["week"].toInt()) {
        case 1:
            date += "星期一";
        case 2:
            date += "星期二";
            break;
        case 3:
            date += "星期三";
            break;
        case 4:
            date += "星期四";
            break;
        case 5:
            date += "星期五";
            break;
        case 6:
            date += "星期六";
            break;
        case 7:
            date += "星期日";
            break;
        default:
            break;
        }

        ui->forecastDateLab->setText(date);
        ui->forecastHighTempLab->setText(initMap["daytemp"]+"°C");
        ui->forecastLowTempLab->setText(initMap["nighttemp"]+"°C");

        QString dnweather;
        if(initMap["dayweather"]!=initMap["nightweather"])
            dnweather = initMap["dayweather"]+"转"+initMap["nightweather"];
        else
            dnweather = initMap["dayweather"];

        ui->forecastWeatherLab->setText(dnweather);


        ui->forecastWindDirectionLab->setText(initMap["daywind"]);
        ui->forecastWindPowerLab->setText(initMap["daypower"]+"级");

        // 绘图后续
        drawingChartWidget(weathers);
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
    // 判断是否未直辖市：通过code
    int codeTasking = code.toInt();
    codeTasking = codeTasking%100;
    if(codeTasking==0)
    {
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
}

void QWeatherForecast::on_cityEdit_editingFinished()
{
    // 获取城市名称
    QString name = ui->cityEdit->text().trimmed();
    // 使用自定义IOxmlconfig对象
    IOXmlConfig config;
    // 通过它获取其edcode
    QString code = config.getCityCodeByName(name);

    if(code.isEmpty())
    {
        QMessageBox::warning(this,"未响应","输入有误",QMessageBox::Ok);
        ui->cityEdit->clear();
        return;
    }

    // 天气请求
    QString extensions;
    if(ui->stackedWidget->currentWidget() == ui->page)
        extensions = "base";
    else
        extensions = "all";

    request(code,extensions);

}
