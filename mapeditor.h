#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QDialog>
#include "game.h"

namespace Ui {
class MapEditor;
}

class MapEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MapEditor(QWidget *parent = nullptr);
    ~MapEditor() override;

private slots:
    void OnNewMapButtonClicked();

    void OnLoadMapButtonClicked();

    void OnSaveMapButtonClicked();

    void OnMapXBorderChanged(bool blocking);

    void OnMapYBorderChanged(bool blocking);

    void OnObstaclePainterButtonToggled(bool checked);

    void OnEraseButtonToggled(bool checked);

    void OnAddPortalButtonToggled(bool checked);

    void OnSetSpawnPointButtonToggled(bool checked);

private:
    void UpdateCanvas();

    void OnObstaclePainterMouseSelect(int x, int y);

    void OnEraseMouseSelect(int x, int y);

    void OnAddPortalMouseSelect(int x, int y);

    void OnSetSpawnPointMouseSelect(int x, int y);

private:
    Ui::MapEditor *ui;
    Map map;
    Game *game;
    const static Config dummy_config;
    Point firstPortalPoint;
};

#endif // MAPEDITOR_H
