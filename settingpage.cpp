#include "status.h"
#include "settingpage.h"
#include "ui_settingpage.h"
#include <QFileDialog>
#include <QFileInfo>

SettingPage::SettingPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingPage)
{
    ui->setupUi(this);
}

SettingPage::~SettingPage()
{
    delete ui;
}

void SettingPage::on_configButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::rootPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(filePath);
    ui->configLabel->setText(QString("当前选中") + fileInfo.fileName());
    // TODO: add filename check
    configManager->LoadConfig(filePath.toStdString());
}

void SettingPage::on_mapButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::rootPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(filePath);
    ui->mapLabel->setText(QString("当前选中") + fileInfo.fileName());
    // TODO: add filename check
    mapManager->LoadMap(filePath.toStdString());
}

void SettingPage::on_confirmButton_clicked() {
    this->done(0);
    playPage->show();
}
