#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QTimer>

class QVideoWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_Start_triggered();
    void on_action_Stop_triggered();
    void errorOccurred(QCamera::Error value);
    void stateChanged(QCamera::State state);
    void statusChanged(QCamera::Status status);
    void timeout();

private:
    Ui::MainWindow *ui;
    QCamera* _cam;
    QTimer _timer;
    QVideoWidget* _vWidget;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};
#endif // MAINWINDOW_H
