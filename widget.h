#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "settingpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainPage; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_enterButton_clicked();

private:
    Ui::MainPage *ui;
    SettingPage *settingPage = new SettingPage();
};
#endif // WIDGET_H
