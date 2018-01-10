#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "mesh.h"
#include "meshwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addMesh();
    void removeMesh();
    void redraw();

private:
    Ui::MainWindow *ui;
    QVector<std::pair<Mesh*, MeshWidget*>> m_meshes;
};

#endif // MAINWINDOW_H
