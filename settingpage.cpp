#include "settingpage.h"
#include "ui_settingpage.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QCoreApplication>

SettingPage::SettingPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingPage),
    widget(dynamic_cast<Widget *>(parent))
{
    ui->setupUi(this);
    connect(ui->configButton, &QPushButton::clicked, this, &SettingPage::OnConfigButtonClicked);
    connect(ui->mapButton, &QPushButton::clicked, this, &SettingPage::OnMapButtonClicked);
    connect(ui->confirmButton, &QPushButton::clicked, this, &SettingPage::OnConfirmButtonClicked);
}

SettingPage::~SettingPage()
{
    delete ui;
}

void SettingPage::OnConfigButtonClicked()
{
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("config/"), tr("配置文件 (*.cfg)"));

    if (configFilePath.isEmpty()) return;

    QFileInfo fileInfo = QFileInfo(configFilePath);
    ui->configLabel->setText(fileInfo.fileName());
    widget->SetGameConfigPath(fileInfo);
}

void SettingPage::OnMapButtonClicked()
{
    QString mapFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("maps/"), tr("地图文件 (*.map)"));

    if (mapFilePath.isEmpty()) return;

    QFileInfo fileInfo = QFileInfo(mapFilePath);
    ui->mapLabel->setText(fileInfo.fileName());
    widget->SetGameMapPath(fileInfo);
}

void SettingPage::OnConfirmButtonClicked()
{
    this->accept();
}
