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

void RePlayPage::exitButton_clicked() {
    if(gameTimer != nullptr && gameTimer->isActive()) gameTimer->stop();
    this->hide();
    widget->show();
}

void RePlayPage::recordButton_clicked() {
    QString recordFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(recordFilePath);
    initPlay(fileInfo);
}

void RePlayPage::playButton_clicked() {
    if(playFlag) {
        playFlag = false;
        gameTimer->stop();
    } else {
        playFlag = true;
        gameTimer->start();
    }
}

RePlayPage::RePlayPage(QWidget *parent) :
    initFlag(false),
    playFlag(true),
    widget(dynamic_cast<Widget *>(parent)),
    curStep(0),
    game(nullptr),
    ui(new Ui::RePlayPage),
    gameTimer(nullptr) {
    ui->setupUi(this);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(1, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::recordButton_clicked);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(2, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::playButton_clicked);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(3, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::exitButton_clicked);
}

RePlayPage::~RePlayPage() {
    delete ui;
}

void RePlayPage::initPlay(const QFileInfo& fileInfo) {
    curStep = 0;
    Widget::mode = true;
    ui->RecordLabel->setText(fileInfo.fileName());
    Record tmp;
    try {
        tmp = RecordManager::LoadRecord(fileInfo.filePath().toStdString(), Widget::gameRecord);
    } catch (exception &e) {
        QMessageBox::warning(this, "打开地图错误", e.what());
        return;
    }
    Widget::ResetRecord(tmp);
    game = new Game(Widget::GetMap(), Widget::GetConfig(), 1);
    ui->Canvas->SetGame(game);
    auto &status = game->GetStatus();
    game->SetStatus(Game::Alive);
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [&]() {
        ++curStep;
        game->Step();
        while(Widget::GetCurrentStep() == curStep) Step();
        ui->ScoreLabel->setText(QString::number(status.score));
        ui->LengthLabel->setText(QString::number(status.length));
        ui->Canvas->update();
        if(Widget::IsEnd()) gameOver();
    });
    gameTimer->start((int)(TIME_INTERVAL * (1. / status.config.level)));
}

void RePlayPage::gameOver() {
    gameTimer->stop();
    this->hide();
    widget->show();
}

void RePlayPage::Step() {
    auto status = game->GetStatus();
    char curOp =Widget::NextAction();
    if(curOp == 'M') {
        Game::Direction direction = game->GetStatus().direction;
        char dire = Widget::GetNextMovement();
        //DSAW
        switch (dire) {
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
        default:
            break;
        }
    } else if(curOp == 'F') {
        auto food = Widget::GetNextFood();
        int x = food.first.x, y = food.first.y;
        status.map[x][y].y = food.second;
        status.foods.push_back({x, y});
    }
}

int RePlayPage::getScore() {
    return game->GetStatus().score;
}

int RePlayPage::getLength() {
    return game->GetStatus().length;
}
