#include "status.h"
#include "settingpage.h"
#include "ui_settingpage.h"
#include <QFileDialog>
#include <QFileInfo>
#include "status.h"
#include "configmanager.h"
#include "mapmanager.h"
#include "game.h"
#include <cstring>
#include <QCoreApplication>

using namespace std;

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

bool configFlag = false, mapFlag = false;
QString configFilePath, mapFilePath;
void SettingPage::on_configButton_clicked() {
    if(!configFlag) {
        configFilePath = "/Users/ubec/workspace/SnakeFoP/SnakeFoP/config/default.txt";
        configFlag = true;
        QFileInfo fileInfo = QFileInfo(configFilePath);
        ui->configLabel->setText(QString("当前选中") + fileInfo.fileName());
        gameConfigPath = configFilePath.toStdString();
        return;
    }
    configFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::rootPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(configFilePath);
    ui->configLabel->setText(QString("当前选中") + fileInfo.fileName());
    // TODO: add filename check
    gameConfigPath = configFilePath.toStdString();
}

void SettingPage::on_mapButton_clicked() {
    if(!mapFlag) {
        mapFilePath = "/Users/ubec/workspace/SnakeFoP/SnakeFoP/maps/test_map.txt";
        mapFlag = true;
        QFileInfo fileInfo = QFileInfo(mapFilePath);
        ui->mapLabel->setText(QString("当前选中") + fileInfo.fileName());
        gameMapPath = mapFilePath.toStdString();
        return;
    }
    mapFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::rootPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(mapFilePath);
    ui->mapLabel->setText(QString("当前选中") + fileInfo.fileName());
    // TODO: add filename check
    gameMapPath = mapFilePath.toStdString();
}

void SettingPage::on_confirmButton_clicked() {
    this->done(0);
    playPage->initPlay();
    playPage->show();
}
