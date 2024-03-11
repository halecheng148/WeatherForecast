#include "qweatherforecast.h"
#include "ui_qweatherforecast.h"

#include <QDebug>
QWeatherForecast::QWeatherForecast(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QWeatherForecast)
{
    ui->setupUi(this);

    m_menu = new QMenu;
    m_menu->addAction(ui->refreshAction);
    m_menu->addAction(ui->cityChangedAction);
    connect(ui->cityChangedAction,SIGNAL(triggered(bool)),this,SLOT(on_cityChangedActionChecked(bool)));

    ui->selectModeWgt->setVisible(0);

}

QWeatherForecast::~QWeatherForecast()
{
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

    int index = ui->stackedWidget->currentIndex();
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
