#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsScene>
#include "View.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImageFromFile();
    void loadPreset();
private:
    void loadImageToScene(QString fileName);
    QGraphicsScene* m_scene;
    View* m_view;
};
