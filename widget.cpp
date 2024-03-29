#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MainPage),
    settingPage(new SettingPage(this)),
    replayPage(new ReplayPage(this)),
    playPage(new PlayPage(this)),
    gameConfigPath(QDir(QCoreApplication::applicationDirPath()).filePath("config/default.cfg")),
    gameMapPath(QDir(QCoreApplication::applicationDirPath()).filePath("maps/default.map")),
    mapEditor(new MapEditor(this)),
    configEditor(new ConfigEditor(this))
{
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(ui->replayButton, &QPushButton::clicked, this, &Widget::OnReplayButtonClicked);
    connect(ui->enterButton, &QPushButton::clicked, this, &Widget::OnEnterButtonClicked);
    connect(ui->MapEditorButton, &QPushButton::clicked, this, &Widget::OnMapEditorButtonClicked);
    connect(ui->ConfigEditorButton, &QPushButton::clicked, this, &Widget::OnConfigEditorButtonClicked);

    const auto appDir = QDir(QCoreApplication::applicationDirPath());
    QDir::setCurrent(appDir.absolutePath());
    appDir.mkpath("maps/");
    appDir.mkpath("records/");
    appDir.mkpath("config/");
}

Widget::~Widget()
{
    delete ui;
    delete settingPage;
    delete replayPage;
    delete playPage;
    delete mapEditor;
}

void Widget::OnReplayButtonClicked()
{
    QString recordFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("records/"), tr("回放文件 (*.rec)"));

    if (recordFilePath.isEmpty()) return;

    QFileInfo fileInfo = QFileInfo(recordFilePath);
    if (replayPage->InitPlay(fileInfo))
    {
        this->hide();
        replayPage->show();
    }
}

void Widget::OnEnterButtonClicked()
{
    if (settingPage->exec() == QDialog::Accepted)
    {
        if (playPage->InitPlay())
        {
            playPage->show();
            this->hide();
        }
    }
}

void Widget::OnMapEditorButtonClicked()
{
    mapEditor->exec();
}

void Widget::OnConfigEditorButtonClicked()
{
    configEditor->exec();
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
    gameRecord.Reset(map, config);
}

void Widget::ResetRecord(const Record &record)
{
    seqPtr = 0;
    movementPtr = 0;
    foodPtr = 0;
    gameRecord.Reset(record);
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

Record Widget::GetRecord() const
{
    return gameRecord;
}

void Widget::PrintRecord()
{
    qDebug() << "TimeSize: " << gameRecord.timestamp.size() << "\n";
    qDebug() << "Food: \n";
    for (const auto &i: gameRecord.foodSequence)
    {
        qDebug() << i.first.x << " " << i.first.y << "\n";
    }
    qDebug() << "Movement: \n";
    for (char i: gameRecord.moveSequence)
    {
        qDebug() << i << " ";
    }
    qDebug() << "\n";
}

pair<Point, int> Widget::GetNextFood()
{
    if (foodPtr >= (int) gameRecord.foodSequence.size())
    {
        throw std::runtime_error("No more foods");
    }

    return gameRecord.foodSequence[foodPtr++];
}

char Widget::GetNextMovement()
{
    if (movementPtr >= (int) gameRecord.moveSequence.length())
    {
        throw std::runtime_error("No more movements");
    }

    return gameRecord.moveSequence[movementPtr++];
}

char Widget::NextAction()
{
    if (seqPtr >= (int) gameRecord.sequence.length())
    {
        throw std::runtime_error("No more actions");
    }

    return gameRecord.sequence[seqPtr++];
}

char Widget::GetCurrentAction()
{
    if (seqPtr >= (int) gameRecord.sequence.length())
    {
        throw std::runtime_error("Current action not found");
    }

    return gameRecord.sequence[seqPtr];
}

Map Widget::GetMap() const
{
    return gameRecord.map;
}

const Config& Widget::GetConfig() const
{
    return gameRecord.config;
}

bool Widget::IsEnd() const
{
    return seqPtr >= (int) gameRecord.sequence.length();
}

void Widget::UpdateTime(int tme)
{
    gameRecord.timestamp.push_back(tme);
}

int Widget::GetCurrentStep()
{
    if (seqPtr >= (int) gameRecord.sequence.length())
    {
        throw std::runtime_error("Current step not found");
    }

    return gameRecord.timestamp[seqPtr];
}

void Widget::SetMode(const bool _mode)
{
    mode = _mode;
}

Record &Widget::GetGameRecord()
{
    return gameRecord;
}

bool Widget::GetMode() const
{
    return mode;
}

void Widget::ResetRecord()
{
    seqPtr = 0;
    movementPtr = 0;
    foodPtr = 0;
}
