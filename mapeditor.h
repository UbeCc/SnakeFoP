#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QDialog>

namespace Ui {
class MapEditor;
}

class MapEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MapEditor(QWidget *parent = nullptr);
    ~MapEditor() override;

private:
    Ui::MapEditor *ui;
};

#endif // MAPEDITOR_H
