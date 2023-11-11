#include <QFileDialog>
#include <QMessageBox>
#include "configeditor.h"
#include "ui_configeditor.h"

using std::exception;

const float eps = 1e-5;

ConfigEditor::ConfigEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigEditor)
{
    ui->setupUi(this);
    connect(ui->CreateButton, &QPushButton::clicked, this, &ConfigEditor::OnNewConfigButtonClicked);
    connect(ui->LoadButton, &QPushButton::clicked, this, &ConfigEditor::OnLoadConfigButtonClicked);
    connect(ui->SaveButton, &QPushButton::clicked, this, &ConfigEditor::OnSaveConfigButtonClicked);
}

ConfigEditor::~ConfigEditor()
{
    delete ui;
}

void ConfigEditor::OnNewConfigButtonClicked()
{
    QString errorText = "";
    int level = ui->LevelText->toPlainText().toInt();
    if(level < 1 || level > 10 ) errorText += "难度错误，应为1~10的正整数\n";

    int seed = ui->SeedText->toPlainText().toInt();

    int foodCount = ui->FoodNumText->toPlainText().toInt();
    if(foodCount < 1 || foodCount > 5) errorText += "食物数量错误，应为1~5的正整数\n";

    float foodPossibility1 = ui->Food1Text->toPlainText().toFloat();
    if(foodPossibility1 < 0 || foodPossibility1 > 1) errorText += "食物概率1错误，应为0~1的浮点数\n";
    float foodPossibility2 = ui->Food2Text->toPlainText().toFloat();
    if(foodPossibility2 < 0 || foodPossibility2 > 1) errorText += "食物概率2错误，应为0~1的浮点数\n";
    float foodPossibility3 = ui->Food3Text->toPlainText().toFloat();
    if(foodPossibility3 < 0 || foodPossibility3 > 1) errorText += "食物概率3错误，应为0~1的浮点数\n";
    if(abs(foodPossibility1 + foodPossibility2 + foodPossibility3 - 1) > eps)
        errorText += "食物概率错误，三种食物概率和应为1\n";

    if(errorText != "")
    {
        QMessageBox::warning(this, "配置错误", errorText);
        return;
    }

    config = {
        level,
        seed,
        foodCount,
        {foodPossibility1, foodPossibility2, foodPossibility3}
    };
}

void ConfigEditor::OnLoadConfigButtonClicked() {
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"));
    QDir(QCoreApplication::applicationFilePath()).filePath("config/"), tr("配置文件 (*.cfg)");
    QFileInfo fileInfo = QFileInfo(configFilePath);
    try
    {
        config = ConfigManager::LoadConfig(fileInfo.filePath().toStdString());
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "打开配置错误", e.what());
        return;
    }

    ui->LevelText->setText(QString::number(config.level));
    ui->SeedText->setText(QString::number(config.randomSeed));
    ui->FoodNumText->setText(QString::number(config.foodCount));
    ui->Food1Text->setText(QString::number(config.foodProbabilities[0]));
    ui->Food2Text->setText(QString::number(config.foodProbabilities[1]));
    ui->Food3Text->setText(QString::number(config.foodProbabilities[2]));
}

void ConfigEditor::OnSaveConfigButtonClicked()
{
    QString configFilePath = QFileDialog::getSaveFileName(this, tr("选择文件"),
    QDir(QCoreApplication::applicationDirPath()).filePath("config/"), tr("配置文件 (*.cfg)"));
    QFileInfo fileInfo = QFileInfo(configFilePath);
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
