#include "replaypage.h"
#include "ui_replaypage.h"
#include <QPainter>
#include <QTimer>
#include <QString>
#include <QKeyEvent>
#include <QMessageBox>
#include "game.h"
#include "playpage.h"
#include "recordmanager.h"
#include <QDir>
#include <QFileDialog>

using namespace std;

void RePlayPage::exitButton_clicked() {
    gameOver();
}

void RePlayPage::recordButton_clicked() {
    QString recordFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath(), tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(recordFilePath);
    ui->RecordLabel->setText(fileInfo.fileName());
    record = RecordManager::LoadRecord(fileInfo.filePath().toStdString());
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
    playFlag(false),
    widget(dynamic_cast<Widget *>(parent)),
    ui(new Ui::RePlayPage) {
    ui->setupUi(this);
    gameCanvas = new GameCanvas(this);
    ui->horizontalLayout_2->replaceWidget(ui->GameCanvas, gameCanvas);
//    for(int row = 0; row < ui->gridView->rowCount(); ++row) {
//        for (int col = 0; col < ui->gridView->columnCount(); ++col) {
//            QLayoutItem* item = ui->gridView->itemAtPosition(row, col);
//            if (item != nullptr) {
//                QWidget* widget = item->widget();
//                if (widget != nullptr) {
//                    QString widgetName = widget->objectName();
//                    qDebug() << "Widget name:" << widgetName;
//                }
//            }
//        }
//    }
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(1, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::recordButton_clicked);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(2, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::playButton_clicked);
    connect(dynamic_cast<QPushButton*>(ui->gridView->itemAtPosition(3, 1)->widget()), &QPushButton::clicked, this, &RePlayPage::exitButton_clicked);
    delete ui->GameCanvas;
    ui->GameCanvas = gameCanvas;
}

RePlayPage::~RePlayPage() {
    delete ui;
}

void RePlayPage::initPlay(QFileInfo fileInfo) {
    ui->RecordLabel->setText(fileInfo.fileName());
    Widget::ResetRecord(RecordManager::LoadRecord(fileInfo.filePath().toStdString()));
    game = new Game(Widget::GetMap(), Widget::GetConfig(), 1);
    gameCanvas->SetGame(game);
    auto &status = game->GetStatus();
    game->SetStatus(Game::Alive);
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(Step()));
    gameTimer->start((int)(1000 * (1. / status.config.level)));
}

void RePlayPage::gameOver() {
    gameTimer->stop();
    this->done(0);
    widget->show();
}

void RePlayPage::Step() {
    game->Step();
    const auto &status = game->GetStatus();

    if (status.state == Game::Dead) {
        gameOver();
    }

    ui->ScoreLabel->setText(QString::number(status.score));
    ui->LengthLabel->setText(QString::number(status.length));
    ui->GameCanvas->update();
}

int RePlayPage::getScore() {
    return game->GetStatus().score;
}

int RePlayPage::getLength() {
    return game->GetStatus().length;
}

void RePlayPage::showEvent(QShowEvent *event) {
    qDebug() << "QWQ";
}
