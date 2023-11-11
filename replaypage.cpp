#include "replaypage.h"
#include "ui_replaypage.h"
#include <QPainter>
#include <QTimer>
#include <QString>
#include <QKeyEvent>
#include <QMessageBox>
#include "game.h"
#include "recordmanager.h"
#include <QDir>
#include <QFileDialog>

using std::exception;

void ReplayPage::OnExitButtonClicked()
{
    if (gameTimer != nullptr && gameTimer->isActive()) gameTimer->stop();
    this->hide();
    widget->show();
}

void ReplayPage::OnRecordButtonClicked()
{
    QString recordFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("records/"), tr("回放文件 (*.rec)"));
    QFileInfo fileInfo = QFileInfo(recordFilePath);
    InitPlay(fileInfo);
}

void ReplayPage::OnPlayButtonClicked()
{
    if (playFlag)
    {
        playFlag = false;
        gameTimer->stop();
    }
    else
    {
        playFlag = true;
        gameTimer->start();
    }
}

ReplayPage::ReplayPage(QWidget *parent) :
    initFlag(false),
    playFlag(true),
    widget(dynamic_cast<Widget *>(parent)),
    curStep(0),
    game(nullptr),
    ui(new Ui::ReplayPage),
    gameTimer(nullptr),
    playRate(1)
{
    ui->setupUi(this);
    connect(ui->recordButton, &QPushButton::clicked, this, &ReplayPage::OnRecordButtonClicked);
    connect(ui->playButton, &QPushButton::clicked, this, &ReplayPage::OnPlayButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &ReplayPage::OnExitButtonClicked);
    connect(ui->playRateBox, &QDoubleSpinBox::valueChanged, this, &ReplayPage::OnPlayRateBoxChanged);
}

ReplayPage::~ReplayPage()
{
    delete ui;
    delete game;
    delete gameTimer;
}

bool ReplayPage::InitPlay(const QFileInfo &fileInfo)
{
    curStep = 0;
    widget->SetMode(true);
    ui->RecordLabel->setText(fileInfo.fileName());
    Record tmp;
    try
    {
        tmp = RecordManager::LoadRecord(fileInfo.filePath().toStdString(), widget->GetGameRecord());
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "打开地图错误", e.what());
        return false;
    }
    widget->ResetRecord();
    widget->ResetRecord(tmp);
    delete game;
    game = new Game(widget->GetMap(), widget->GetConfig(), 1, widget);
    ui->Canvas->SetGame(game);
    auto &status = game->GetStatus();
    game->SetStatus(Game::Alive);

    delete gameTimer;
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [&]()
    {
        if (widget->IsEnd()) {
            return;
        }

        while (!widget->IsEnd() && widget->GetCurrentStep() == curStep) Step();
        if (!widget->IsEnd())
        {
            ++curStep;
            game->Step(widget);
            ui->ScoreLabel->setText(QString::number(status.score));
            ui->LengthLabel->setText(QString::number(status.length));
            ui->Canvas->update();
        }
    });
    gameTimer->start((int) (TIME_INTERVAL / playRate * (1. / status.config.level)));
    return true;
}

void ReplayPage::GameOver()
{
    gameTimer->stop();
    QMessageBox::information(this, "回放", "本场回放已结束");
}

void ReplayPage::Step()
{
    auto status = game->GetStatus();
    char curOp = widget->NextAction();
    if (curOp == 'M')
    {
        Game::Direction direction = game->GetStatus().direction;
        char dire = widget->GetNextMovement();
        //DSAW
        switch (dire)
        {
            case 'W':
                if (direction != Game::Down) game->ChangeDirection(Game::Up);
                break;
            case 'S':
                if (direction != Game::Up) game->ChangeDirection(Game::Down);
                break;
            case 'A':
                if (direction != Game::Right) game->ChangeDirection(Game::Left);
                break;
            case 'D':
                if (direction != Game::Left) game->ChangeDirection(Game::Right);
                break;
            case 'Q':
                GameOver();
                break;
            default:
                break;
        }
    }
    else if (curOp == 'F')
    {
        game->UpdateFood(widget);
    }
}

int ReplayPage::GetScore()
{
    return game->GetStatus().score;
}

int ReplayPage::GetLength()
{
    return game->GetStatus().length;
}

void ReplayPage::OnPlayRateBoxChanged()
{
    playRate = ui->playRateBox->value();
    gameTimer->stop();
    gameTimer->start((int) (TIME_INTERVAL / playRate * (1. / game->GetStatus().config.level)));
}
