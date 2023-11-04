#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QDialog>
#include "playpage.h"

namespace Ui {
class SettingPage;
}

class SettingPage : public QDialog
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);
    virtual void showEvent(QShowEvent*);
    ~SettingPage();

private slots:
    void on_configButton_clicked();
    void on_mapButton_clicked();
    void on_confirmButton_clicked();

private:
    Ui::SettingPage *ui;
    PlayPage *playPage = new PlayPage();
};

#endif // SETTINGPAGE_H
