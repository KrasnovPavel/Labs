#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addMesh);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addMesh()
{
    QString name = QFileDialog::getOpenFileName(this,
                                                "Choose ASCII STL file to open",
                                                ".",
                                                "STL files (.stl)");
    m_meshes.push_back(std::make_pair(new Mesh(name.toStdString()),
                                      new MeshWidget(m_meshes.size() + 1,
                                                     QUrl(name).fileName())));
    ui->scrollAreaWidgetContents->layout()->addWidget(m_meshes.last().second);
    connect(m_meshes.last().second, &MeshWidget::remove, this, &MainWindow::removeMesh);
    m_meshes.last().second->show();
}

void MainWindow::removeMesh()
{
    for (int i = 0; i < m_meshes.size(); ++i)
    {
        if (m_meshes.at(i).second == sender())
        {
            delete m_meshes.at(i).first;
            delete m_meshes.at(i).second;
            m_meshes.remove(i);
            break;
        }
    }
}

void MainWindow::redraw()
{
    for(auto m: m_meshes)
    {
        m.first->position = m.second->position();
        m.first->solid = !m.second->isGrid();
        m.first->setColor(m.second->color().redF(),
                          m.second->color().greenF(),
                          m.second->color().blueF(),
                          m.second->color().alphaF());
        if (m.second->isRotating())
        {
            m.first->rotation *= m.second->angle();
        }
        else
        {
            m.first->rotation = m.second->angle();
        }
        if (m.second->isVisible())
        {
            m.first->draw();
        }
    }
}

