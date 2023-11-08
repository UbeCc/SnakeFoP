#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MainPage),
    settingPage(new SettingPage(this)),
    replayPage(new RePlayPage(this)),
    playPage(new PlayPage(this)),
    gameConfigPath(QDir(QCoreApplication::applicationDirPath()).filePath("config/default.txt")),
    gameMapPath(QDir(QCoreApplication::applicationDirPath()).filePath("maps/test_map.txt")),
    mapEditor(new MapEditor(this))
{
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_replayButton_clicked()
{
    QString recordFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("records/"),
        tr("所有文件 (*)"));
    QFileInfo fileInfo = QFileInfo(recordFilePath);
    if (replayPage->initPlay(fileInfo))
    {
        this->hide();
        replayPage->show();
    }
}

void Widget::on_enterButton_clicked()
{
    if (settingPage->exec() == QDialog::Accepted)
    {
        if (playPage->initPlay())
        {
            playPage->show();
            this->hide();
        }
    }
}

void Widget::on_MapEditorButton_clicked()
{
    mapEditor->exec();
}


void Widget::SetGameConfigPath(const QFileInfo &path)
{
    gameConfigPath = path;
}

void Widget::SetGameMapPath(const QFileInfo &path)
{
    gameMapPath = path;
}

QFileInfo Widget::GetGameConfigPath() const
{
    return gameConfigPath;
}

QFileInfo Widget::GetGameMapPath() const
{
    return gameMapPath;
}

void Widget::ResetRecord(const Map &map, const Config &config)
{
    seqPtr = 0;
    movementPtr = 0;
    foodPtr = 0;
    gameRecord.reset(map, config);
}

void Widget::ResetRecord(const Record &record)
{
    seqPtr = 0;
    movementPtr = 0;
    foodPtr = 0;
    gameRecord.reset(record);
}

void Widget::UpdateRecordFood(int x, int y, int v)
{
    gameRecord.sequence += "F";
    gameRecord.foodSequence.emplace_back(Point{x, y}, v);
}

void Widget::UpdateRecordMovement(char movement)
{
    gameRecord.sequence += "M";
    gameRecord.moveSequence += movement;
}

Record Widget::GetRecord()
{
    return gameRecord;
}

int Widget::seqPtr = 0;
int Widget::movementPtr = 0;
int Widget::foodPtr = 0;
bool Widget::mode = false;
Record Widget::gameRecord;

void Widget::PrintRecord()
{
    qDebug() << "TimeSize: " << gameRecord.timestamp.size() << "\n";
    qDebug() << "Food: \n";
    for (const auto &i: gameRecord.foodSequence)
    {
        qDebug() << i.first.x << " " << i.first.y << "\n";
    }
    qDebug() << "Movement: \n";
    for (char i : gameRecord.moveSequence)
    {
        qDebug() << i << " ";
    }
    qDebug() << "\n";
}

pair<Point, int> Widget::GetNextFood()
{
    return gameRecord.foodSequence[foodPtr++];
}

char Widget::GetNextMovement()
{
    return gameRecord.moveSequence[movementPtr++];
}

char Widget::NextAction()
{
    return gameRecord.sequence[seqPtr++];
}

char Widget::GetCurrentAction()
{
    return gameRecord.sequence[seqPtr];
}

Map Widget::GetMap()
{
    return gameRecord.map;
}

Config Widget::GetConfig()
{
    return gameRecord.config;
}

bool Widget::IsEnd()
{
    return seqPtr == (int) gameRecord.sequence.length();
}

void Widget::UpdateTime(int tme)
{
    gameRecord.timestamp.push_back(tme);
}

int Widget::GetCurrentStep()
{
    return gameRecord.timestamp[seqPtr];
}
