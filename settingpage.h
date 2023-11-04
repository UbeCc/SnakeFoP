#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QDialog>
#include "widget.h"

namespace Ui {
class SettingPage;
}

class Widget;

class SettingPage : public QDialog
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);
    ~SettingPage();

private slots:
    void on_configButton_clicked();
    void on_mapButton_clicked();
    void on_confirmButton_clicked();

private:
    Ui::SettingPage *ui;
    Widget *widget;
};

#endif // SETTINGPAGE_H
