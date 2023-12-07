#include <QFileDialog>
#include <QMessageBox>
#include "mapeditor.h"
#include "ui_mapeditor.h"

using std::exception;

const Config MapEditor::dummyConfig = {
    1,
    0,
    0,
    {1, 0, 0}
};

MapEditor::MapEditor(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::MapEditor),
    game(nullptr),
    firstPortalPoint({-1, -1})
{
    ui->setupUi(this);
    connect(ui->NewMapButton, &QPushButton::clicked, this, &MapEditor::OnNewMapButtonClicked);
    connect(ui->LoadMapButton, &QPushButton::clicked, this, &MapEditor::OnLoadMapButtonClicked);
    connect(ui->SaveMapButton, &QPushButton::clicked, this, &MapEditor::OnSaveMapButtonClicked);
    connect(ui->MapXBorder, &QCheckBox::toggled, this, &MapEditor::OnMapXBorderChanged);
    connect(ui->MapYBorder, &QCheckBox::toggled, this, &MapEditor::OnMapYBorderChanged);
    connect(ui->ObstaclePainterButton, &QPushButton::toggled, this, &MapEditor::OnObstaclePainterButtonToggled);
    connect(ui->EraserButton, &QPushButton::toggled, this, &MapEditor::OnEraseButtonToggled);
    connect(ui->AddPortalButton, &QPushButton::toggled, this, &MapEditor::OnAddPortalButtonToggled);
    connect(ui->SpawnPointButton, &QPushButton::toggled, this, &MapEditor::OnSetSpawnPointButtonToggled);
    connect(ui->CommandLine, &QLineEdit::returnPressed, this, &MapEditor::OnCommandLineReturnPressed);

    OnNewMapButtonClicked();
}

MapEditor::~MapEditor()
{
    delete game;
    delete ui;
}

void MapEditor::UpdateCanvas()
{
    delete game;
    game = new Game(map, dummyConfig, 2);
    ui->Canvas->SetGame(game);
}

void MapEditor::OnNewMapButtonClicked()
{
    map = {
        ui->MapXSize->value(),
        ui->MapYSize->value(),
        {false, false, false, false},
        vector<Point>(),
        vector<array<Point, 2>>(),
        {0, 0}
    };

    ui->MapSizeLabel->setText(QString::number(map.width) + " x " + QString::number(map.height));

    UpdateCanvas();
}

void MapEditor::OnLoadMapButtonClicked()
{
    QString mapFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("maps/"), tr("地图文件 (*.map)"));

    if (mapFilePath.isEmpty()) return;

    QFileInfo fileInfo = QFileInfo(mapFilePath);
    try
    {
        map = MapManager::LoadMap(fileInfo.filePath().toStdString());
    }
    catch (exception&e)
    {
        QMessageBox::warning(this, "打开地图错误", e.what());
        return;
    }

    ui->MapSizeLabel->setText(QString::number(map.width) + " x " + QString::number(map.height));

    UpdateCanvas();
}

void MapEditor::OnSaveMapButtonClicked()
{
    QString mapFilePath = QFileDialog::getSaveFileName(this, tr("选择文件"),
        QDir(QCoreApplication::applicationDirPath()).filePath("maps"), tr("地图文件 (*.map)"));

    if (mapFilePath.isEmpty()) return;

    QFileInfo fileInfo = QFileInfo(mapFilePath);
    try
    {
        MapManager::SaveMap(fileInfo.filePath().toStdString(), map);
    }
    catch (exception&e)
    {
        QMessageBox::warning(this, "保存地图错误", e.what());
        return;
    }
}

void MapEditor::OnMapXBorderChanged(bool blocking)
{
    map.borderIsObstacle[0] = blocking;
    map.borderIsObstacle[1] = blocking;

    UpdateCanvas();
}

void MapEditor::OnMapYBorderChanged(bool blocking)
{
    map.borderIsObstacle[2] = blocking;
    map.borderIsObstacle[3] = blocking;

    UpdateCanvas();
}

void MapEditor::OnObstaclePainterButtonToggled(bool checked)
{
    if (checked)
    {
        ui->EraserButton->setChecked(false);
        ui->AddPortalButton->setChecked(false);
        ui->SpawnPointButton->setChecked(false);

        std::function<void(int, int, bool)> select = [this](auto&&PH1, auto&&PH2, auto&&PH3)
        {
            OnObstaclePainterMouseSelect(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2),
                std::forward<decltype(PH3)>(PH3));
        };

        ui->Canvas->SetOnMouseSelect(select);
    }
    else
    {
        ui->Canvas->SetOnMouseSelect(nullptr);
    }
}

void MapEditor::OnObstaclePainterMouseSelect(int x, int y, bool rightButton)
{
    if (map.spawnPoint == Point({x, y}))
    {
        return;
    }

    for (const auto&portal: map.portals)
    {
        if (portal[0] == Point({x, y}) || portal[1] == Point({x, y}))
        {
            return;
        }
    }

    auto it = std::find(map.obstacles.cbegin(), map.obstacles.cend(), Point({x, y}));

    if (!rightButton && it == map.obstacles.cend())
    {
        map.obstacles.push_back({x, y});
        UpdateCanvas();
    }

    else if (rightButton && it != map.obstacles.cend())
    {
        map.obstacles.erase(it);
        UpdateCanvas();
    }
}

void MapEditor::OnEraseButtonToggled(bool checked)
{
    if (checked)
    {
        ui->ObstaclePainterButton->setChecked(false);
        ui->AddPortalButton->setChecked(false);
        ui->SpawnPointButton->setChecked(false);

        std::function<void(int, int, bool)> select = [this](auto&&x, auto&&y, [[maybe_unused]] auto&&_)
        {
            OnEraseMouseSelect(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
        };

        ui->Canvas->SetOnMouseSelect(select);
    }
    else
    {
        ui->Canvas->SetOnMouseSelect(nullptr);
    }
}

void MapEditor::OnEraseMouseSelect(int x, int y)
{
    auto it = std::find(map.obstacles.cbegin(), map.obstacles.cend(), Point({x, y}));

    if (it != map.obstacles.cend())
    {
        map.obstacles.erase(it);
        UpdateCanvas();
        return;
    }

    for (auto portal = map.portals.begin(); portal != map.portals.end(); ++portal)
    {
        if ((*portal)[0] == Point({x, y}) || (*portal)[1] == Point({x, y}))
        {
            map.portals.erase(portal);
            UpdateCanvas();
            return;
        }
    }
}

void MapEditor::OnAddPortalButtonToggled(bool checked)
{
    if (checked)
    {
        ui->ObstaclePainterButton->setChecked(false);
        ui->EraserButton->setChecked(false);
        ui->SpawnPointButton->setChecked(false);

        firstPortalPoint = {-1, -1};

        std::function<void(int, int, bool)> select = [this](auto&&x, auto&&y, [[maybe_unused]] auto&&_)
        {
            OnAddPortalMouseSelect(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
        };

        ui->Canvas->SetOnMouseSelect(select);
    }
    else
    {
        ui->Canvas->SetOnMouseSelect(nullptr);
    }
}

void MapEditor::OnAddPortalMouseSelect(int x, int y)
{
    if (map.spawnPoint == Point({x, y}))
    {
        QMessageBox::warning(this, "添加传送门错误", "传送门不允许放置在出生点");
        ui->AddPortalButton->setChecked(false);
        return;
    }

    if (std::find(map.obstacles.cbegin(), map.obstacles.cend(), Point({x, y})) != map.obstacles.cend())
    {
        QMessageBox::warning(this, "添加传送门错误", "传送门不允许放置在障碍物上");
        ui->AddPortalButton->setChecked(false);
        return;
    }

    for (const auto&portal: map.portals)
    {
        if (portal[0] == Point({x, y}) || portal[1] == Point({x, y}))
        {
            QMessageBox::warning(this, "添加传送门错误", "传送门不允许放置在传送门上");
            ui->AddPortalButton->setChecked(false);
            return;
        }
    }

    if (firstPortalPoint.x == -1)
    {
        firstPortalPoint = {x, y};
        return;
    }

    if (firstPortalPoint == Point({x, y}))
    {
        return;
    }

    map.portals.push_back({firstPortalPoint, {x, y}});
    ui->AddPortalButton->setChecked(false);
    UpdateCanvas();
}

void MapEditor::OnSetSpawnPointButtonToggled(bool checked)
{
    if (checked)
    {
        ui->ObstaclePainterButton->setChecked(false);
        ui->EraserButton->setChecked(false);
        ui->AddPortalButton->setChecked(false);

        std::function<void(int, int, bool)> select = [this](auto&&x, auto&&y, [[maybe_unused]] auto&&_)
        {
            OnSetSpawnPointMouseSelect(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
        };

        ui->Canvas->SetOnMouseSelect(select);
    }
    else
    {
        ui->Canvas->SetOnMouseSelect(nullptr);
    }
}

void MapEditor::OnSetSpawnPointMouseSelect(int x, int y)
{
    if (std::find(map.obstacles.cbegin(), map.obstacles.cend(), Point({x, y})) != map.obstacles.cend())
    {
        QMessageBox::warning(this, "设置出生点错误", "出生点不允许放置在障碍物上");
        ui->SpawnPointButton->setChecked(false);
        return;
    }

    for (const auto&portal: map.portals)
    {
        if (portal[0] == Point({x, y}) || portal[1] == Point({x, y}))
        {
            QMessageBox::warning(this, "设置出生点错误", "出生点不允许放置在传送门上");
            ui->SpawnPointButton->setChecked(false);
            return;
        }
    }

    map.spawnPoint = {x, y};
    ui->SpawnPointButton->setChecked(false);
    UpdateCanvas();
}

void MapEditor::OnCommandLineReturnPressed()
{
    const auto commandString = ui->CommandLine->text().toStdString();
    std::stringstream command(commandString);

    // Try read map size
    {
        int x, y;
        command >> x >> y;
        if (command.eof() || command.good())
        {
            ui->MapXSize->setValue(x);
            ui->MapYSize->setValue(y);
            OnNewMapButtonClicked();
            goto successful_parse;
        }
    }

    command = std::stringstream(commandString);
    char c;
    command >> c;

    switch (c)
    {
        case 'o':
        {
            int x, y;
            command >> x >> y;
            if (command.eof() || command.good())
            {
                --x, --y;
                if (x < 0 || x >= map.width || y < 0 || y >= map.height)
                {
                    QMessageBox::warning(this, "添加障碍物错误", "坐标超出地图范围");
                    return;
                }

                OnObstaclePainterMouseSelect(x, y, false);
                goto successful_parse;
            }

            break;
        }

        case 'p':
        {
            int x, y;
            command >> x >> y;
            if (command.eof() || command.good())
            {
                --x, --y;
                if (x < 0 || x >= map.width || y < 0 || y >= map.height)
                {
                    QMessageBox::warning(this, "删除障碍物错误", "坐标超出地图范围");
                    return;
                }

                OnEraseMouseSelect(x, y);
                goto successful_parse;
            }

            break;
        }

        case 'w':
        case 's':
        case 'a':
        case 'd':
        {
            int v;
            command >> v;

            if (command.eof() || command.good())
            {
                if (v != 0 && v != 1)
                {
                    goto unknown_command;
                }

                switch (c)
                {
                    case 'w':
                    case 's':
                    {
                        ui->MapXBorder->setChecked(v == 1);
                        break;
                    }

                    default:
                    {
                        ui->MapYBorder->setChecked(v == 1);
                        break;
                    }
                }

                goto successful_parse;
            }

            break;
        }

        case 'f':
        {
            OnSaveMapButtonClicked();
            goto successful_parse;
        }

        case 'q':
        {
            this->accept();
            goto successful_parse;
        }

        default:
            break;
    }

unknown_command:
    QMessageBox::warning(this, "未知命令", "未知命令");
    return;

successful_parse:
    ui->CommandLine->clear();
}
