#include "mapeditor.h"
#include "ui_mapeditor.h"

MapEditor::MapEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapEditor)
{
    ui->setupUi(this);
}

MapEditor::~MapEditor()
{
    delete ui;
}
