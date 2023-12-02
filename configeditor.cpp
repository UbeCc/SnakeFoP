#include <QFileDialog>
#include <QMessageBox>
#include "configeditor.h"
#include "ui_configeditor.h"

using std::exception;

ConfigEditor::ConfigEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigEditor)
{
    ui->setupUi(this);
    connect(ui->CreateButton, &QPushButton::clicked, this, &ConfigEditor::OnNewConfigButtonClicked);
    connect(ui->LoadButton, &QPushButton::clicked, this, &ConfigEditor::OnLoadConfigButtonClicked);
    connect(ui->SaveButton, &QPushButton::clicked, this, &ConfigEditor::OnSaveConfigButtonClicked);
    connect(ui->FoodProbability1, &QSlider::valueChanged, this, &ConfigEditor::OnFoodProbability1ValueChanged);
    connect(ui->FoodProbability2, &QSlider::valueChanged, this, &ConfigEditor::OnFoodProbability2ValueChanged);
}

ConfigEditor::~ConfigEditor()
{
    delete ui;
}

void ConfigEditor::OnNewConfigButtonClicked()
{
    ui->Level->setValue(1);
    ui->RandomSeed->setValue(-1);
    ui->FoodCount->setValue(5);
    ui->FoodProbability1->setValue(40);
    ui->FoodProbability2->setMaximum(60);
    ui->FoodProbability2->setValue(30);
}

void ConfigEditor::OnLoadConfigButtonClicked()
{
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationFilePath()).filePath("config/"), tr("配置文件 (*.cfg)"));
    QFileInfo fileInfo = QFileInfo(configFilePath);
    Config config{};

    try
    {
        config = ConfigManager::LoadConfig(fileInfo.filePath().toStdString());
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "打开配置错误", e.what());
        return;
    }

    ui->Level->setValue(config.level);
    ui->RandomSeed->setValue(config.randomSeed);
    ui->FoodCount->setValue(config.foodCount);
    ui->FoodProbability1->setValue((int) (config.foodProbabilities[0] * 100));
    ui->FoodProbability2->setMaximum(100 - ui->FoodProbability1->value());
    ui->FoodProbability2->setValue((int) (config.foodProbabilities[1] * 100));
}

void ConfigEditor::OnSaveConfigButtonClicked()
{
    QString configFilePath = QFileDialog::getSaveFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("config/"), tr("配置文件 (*.cfg)"));
    QFileInfo fileInfo = QFileInfo(configFilePath);

    Config config{
        ui->Level->value(),
        ui->RandomSeed->value(),
        ui->FoodCount->value(),
        {
            (float) ui->FoodProbability1->value() / 100.0f,
            (float) ui->FoodProbability2->value() / 100.0f,
            1.0f - (float) ui->FoodProbability1->value() / 100.0f - (float) ui->FoodProbability2->value() / 100.0f
        }
    };

    try
    {
        ConfigManager::SaveConfig(fileInfo.filePath().toStdString(), config);
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "保存配置错误", e.what());
        return;
    }
}

void ConfigEditor::OnFoodProbability1ValueChanged(int value)
{
    ui->FoodProbability1Label->setText(QString::number(value) + "%");
    ui->FoodProbability2->setMaximum(100 - value);
    if (ui->FoodProbability2->value() > ui->FoodProbability2->maximum())
    {
        ui->FoodProbability2->setValue(ui->FoodProbability2->maximum());
    }
}

void ConfigEditor::OnFoodProbability2ValueChanged(int value)
{
    ui->FoodProbability2Label->setText(QString::number(value) + "%");
}
