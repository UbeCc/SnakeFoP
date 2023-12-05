#include "playpage.h"
#include "ui_playpage.h"
#include <QPainter>
#include <QTimer>
#include <QString>
#include <QKeyEvent>
#include <QMessageBox>
#include "game.h"
#include "recordmanager.h"
#include <QDir>

using std::exception;

PlayPage::PlayPage(QWidget *parent) :
    steps(0),
    widget(dynamic_cast<Widget *>(parent)),
    game(nullptr),
    ui(new Ui::PlayPage),
    gameTimer(new QTimer(this)),
    resultPage(new ResultPage(this))
{
    ui->setupUi(this);
    connect(this, &QDialog::rejected, this, [&](){
        gameTimer->stop();
        widget->show();
        this->hide();
    });

    gameTimer->setTimerType(Qt::PreciseTimer);
    connect(gameTimer, &QTimer::timeout, this, [&]()
    {
        ++steps;
        game->SetPreDirection(game->GetStatus().direction);
        Step();
    });
}

PlayPage::~PlayPage()
{
    delete ui;
    delete gameTimer;
    delete game;
    delete resultPage;
}

void PlayPage::GameOver()
{
    gameTimer->stop();
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    QDateTime dateTime;
    dateTime.setMSecsSinceEpoch(timestamp);
    QString format = "yyyy-MM-dd-hh-mm-ss";
    QString formattedDateTime = dateTime.toString(format);
    try
    {
        RecordManager::SaveRecord(
            QDir(QCoreApplication::applicationDirPath()).filePath("records/" + formattedDateTime + ".rec")
                .toStdString(), widget->GetRecord());
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "保存回放错误", e.what());
        this->hide();
        widget->show();
        return;
    }

    if (resultPage->exec() == QDialog::Accepted)
    {
        widget->show();
        this->hide();
    }
    else {
        this->done(0);
        widget->close();
    }
}

bool PlayPage::InitPlay()
{
    steps = 0;
    widget->ResetRecord();
    Map map{};
    Config config{};
    widget->mode = false;
    try
    {
        map = MapManager::LoadMap(widget->GetGameMapPath().filePath().toStdString());
        config = ConfigManager::LoadConfig(widget->GetGameConfigPath().filePath().toStdString());
    }
    catch (exception &e)
    {
        QMessageBox::warning(this, "打开地图或配置文件错误", e.what());
        return false;
    }

    widget->ResetRecord(map, config);
    game = new Game(map, config, 0, widget);
    ui->ConfigLabel->setText(widget->GetGameConfigPath().fileName());
    ui->MapLabel->setText(widget->GetGameMapPath().fileName());
    ui->ScoreLabel->setText("0");
    ui->LengthLabel->setText("1");
    ui->Canvas->SetGame(game);
    auto &status = game->GetStatus();

    // Set timer and pause at entry
    gameTimer->setInterval((int) (TIME_INTERVAL * (1. / status.config.level)));
    ui->PausedLabel->show();

    return true;
}

void PlayPage::Step()
{
    int tot = game->Step(widget);
    for (int i = 1; i <= tot; ++i) widget->UpdateTime(steps);
    const auto &status = game->GetStatus();

    if (status.state == Game::Dead)
    {
        // use UpdateRecordMovement to memorize `endtime`
        widget->UpdateTime(steps);
        widget->UpdateRecordMovement('Q');
        GameOver();
    }

    ui->ScoreLabel->setText(QString::number(status.score));
    ui->LengthLabel->setText(QString::number(status.length));

    ui->Canvas->update();
}

void PlayPage::keyPressEvent(QKeyEvent *event)
{
    Game::Direction direction = game->GetStatus().preDirection;
    switch (event->key())
    {
        case Qt::Key_Up:
        case Qt::Key_W:
            if (direction != Game::Down && gameTimer->isActive())
            {
                game->ChangeDirection(Game::Up);
                widget->UpdateTime(steps);
                widget->UpdateRecordMovement('W');
            }
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            if (direction != Game::Up && gameTimer->isActive())
            {
                game->ChangeDirection(Game::Down);
                widget->UpdateTime(steps);
                widget->UpdateRecordMovement('S');
            }
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            if (direction != Game::Right && gameTimer->isActive())
            {
                game->ChangeDirection(Game::Left);
                widget->UpdateTime(steps);
                widget->UpdateRecordMovement('A');
            }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            if (direction != Game::Left && gameTimer->isActive())
            {
                game->ChangeDirection(Game::Right);
                widget->UpdateTime(steps);
                widget->UpdateRecordMovement('D');
            }
            break;
        case Qt::Key_Space:
            if(gameTimer->isActive()) {
                gameTimer->stop();
                ui->PausedLabel->show();
            }
            else {
                gameTimer->start();
                ui->PausedLabel->hide();
            }
            break;
        default:
            break;
    }
}

int PlayPage::GetScore()
{
    return game->GetStatus().score;
}

int PlayPage::GetLength()
{
    return game->GetStatus().length;
}
