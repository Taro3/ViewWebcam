#include <QCameraInfo>
#include <QCamera>
#include <QVideoWidget>
#include <QMessageBox>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _cam(nullptr)
    , _timer()
    , _vWidget(nullptr)
{
    ui->setupUi(this);

    _vWidget = new QVideoWidget(ui->widget);
    _vWidget->show();

    _timer.setSingleShot(true);
    _timer.setInterval(10 * 1000);
    connect(&_timer, &QTimer::timeout, this, &MainWindow::timeout);
}

MainWindow::~MainWindow()
{
    _timer.stop();
    if (_cam)
        _cam->setViewfinder(static_cast<QVideoWidget*>(nullptr));
    delete ui;
}

void MainWindow::on_action_Start_triggered()
{
    if (_cam) {
        _cam->stop();
        delete _cam;
    }
    _cam = new QCamera(QCameraInfo::defaultCamera(), ui->widget);
    connect(_cam, &QCamera::errorOccurred, this, &MainWindow::errorOccurred);
    connect(_cam, &QCamera::stateChanged, this,&MainWindow::stateChanged);
    connect(_cam, &QCamera::statusChanged, this,&MainWindow::statusChanged);
    _cam->setViewfinder(_vWidget);
    _vWidget->setGeometry(ui->widget->rect());
    _cam->start();
    _timer.stop();
    _timer.start();
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
}

void MainWindow::on_action_Stop_triggered()
{
    _timer.stop();
    if (_cam)
        _cam->stop();
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
}

void MainWindow::errorOccurred(QCamera::Error value)
{
    QMessageBox::critical(this, tr("Error"), QString("%1: %2").arg(_cam->errorString()).arg(value));
}

void MainWindow::stateChanged(QCamera::State state)
{
    qDebug() << "state:" << state;
}

void MainWindow::statusChanged(QCamera::Status status)
{
    qDebug() << "status:" << status;
    ui->statusbar->showMessage(tr("Status: ") + QString::number(status));
    if (status == QCamera::ActiveStatus) {
        _timer.stop();
    }
}

void MainWindow::timeout()
{
    QMessageBox::critical(this, "Timeout", "Camera not activated.");
    on_action_Stop_triggered();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    QMainWindow::resizeEvent(event);
    _vWidget->setGeometry(ui->widget->rect());
}
