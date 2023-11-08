#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QDialog>
#include "widget.h"

namespace Ui
{
    class SettingPage;
}

class Widget;

class SettingPage : public QDialog
{
Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);

    ~SettingPage() override;

private slots:

    void OnConfigButtonClicked();

    void OnMapButtonClicked();

    void OnConfirmButtonClicked();

private:
    Ui::SettingPage *ui;
    Widget *widget;
};

#endif // SETTINGPAGE_H
