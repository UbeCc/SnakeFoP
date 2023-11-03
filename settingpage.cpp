#include "settingpage.h"
#include "ui_settingpage.h"
#include <QFileDialog>

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

void SettingPage::on_envButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::rootPath(), tr("所有文件 (*)"));
    qDebug() << filePath << Qt::endl;
}

void SettingPage::on_mapButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::rootPath(), tr("所有文件 (*)"));
    qDebug() << filePath << Qt::endl;
}

void SettingPage::on_confirmButton_clicked() {
    this->done(0);
    playpage->show();
}
