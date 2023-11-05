#include "settingpage.h"
#include "ui_settingpage.h"
#include <QFileDialog>
#include <QFileInfo>
#include "configmanager.h"
#include <QCoreApplication>

using namespace std;

SettingPage::SettingPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingPage),
    widget(dynamic_cast<Widget*>(parent))
{
    ui->setupUi(this);
}

SettingPage::~SettingPage()
{
    delete ui;
}

void SettingPage::on_configButton_clicked() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(configFilePath);
    ui->configLabel->setText(fileInfo.fileName());
    widget->SetGameConfigPath(fileInfo);
}

void SettingPage::on_mapButton_clicked() {
    QString mapFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(mapFilePath);
    ui->mapLabel->setText(fileInfo.fileName());
    widget->SetGameMapPath(fileInfo);
}

void SettingPage::on_confirmButton_clicked() {
    this->accept();
}
