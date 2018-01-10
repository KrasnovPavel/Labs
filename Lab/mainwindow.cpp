#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::step);
    m_timer.setInterval(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::step()
{
    QRect rect1 = QRect(ui->pushButton->pos() + m_vel1, ui->pushButton->size());
    QRect rect2 = QRect(ui->pushButton_2->pos() + m_vel2, ui->pushButton_2->size());
    ui->pushButton->setGeometry(rect1);
    ui->pushButton_2->setGeometry(rect2);

    QVector<std::pair<uint, uint>> collisions;
    checkBorderCollision(rect1, rect2, collisions);
    checkButtonCollision(rect1, rect2, collisions);

    emit stepResult(ui->pushButton->pos(), m_vel1,
                    ui->pushButton_2->pos(), m_vel2,
                    collisions);
}

void MainWindow::checkBorderCollision(const QRect &rect1, const QRect &rect2, QVector<std::pair<uint, uint>>& collisions)
{
    if (rect1.top() <= 0)
    {
        m_vel1 = QPoint(m_vel1.x(), -m_vel1.y());
        collisions.append(std::make_pair(1, 3));
    }
    if (rect2.top() <= 0)
    {
        m_vel2 = QPoint(m_vel2.x(), -m_vel2.y());
        collisions.append(std::make_pair(2, 3));
    }
    if (rect1.left() <= 0)
    {
        m_vel1 = QPoint(-m_vel1.x(), m_vel1.y());
        collisions.append(std::make_pair(1, 4));
    }
    if (rect2.left() <= 0)
    {
        m_vel2 = QPoint(-m_vel2.x(), m_vel2.y());
        collisions.append(std::make_pair(2, 4));
    }
    if (rect1.bottom() >= ui->centralWidget->height())
    {
        m_vel1 = QPoint(m_vel1.x(), -m_vel1.y());
        collisions.append(std::make_pair(1, 5));
    }
    if (rect2.bottom() >= ui->centralWidget->height())
    {
        m_vel2 = QPoint(m_vel2.x(), -m_vel2.y());
        collisions.append(std::make_pair(2, 5));
    }
    if (rect1.right() >= ui->centralWidget->width())
    {
        m_vel1 = QPoint(-m_vel1.x(), m_vel1.y());
        collisions.append(std::make_pair(1, 6));
    }
    if (rect2.right() >= ui->centralWidget->width())
    {
        m_vel2 = QPoint(-m_vel2.x(), m_vel2.y());
        collisions.append(std::make_pair(2, 6));
    }
}

void MainWindow::checkButtonCollision(const QRect &rect1, const QRect &rect2, QVector<std::pair<uint, uint> > &collisions)
{
    if (rect1.intersects(rect2))
    {
        QPoint normal;
        if (rect1.intersected(rect2).height() > rect1.intersected(rect2).width())
        {
            normal = QPoint(1, 0);
        }
        else
        {
            normal = QPoint(0, 1);
        }

        QPoint v = m_vel2 - m_vel1;
        double pr = QPoint::dotProduct(v, normal);
        m_vel1 += pr * normal;
        m_vel2 -= pr * normal;


        collisions.append(std::make_pair(1, 2));
    }
}

void MainWindow::start(const QPoint &vel1, const QPoint &vel2)
{
    m_vel1 = vel1;
    m_vel2 = vel2;
    m_timer.start();
}

void MainWindow::startWithPos(const QPoint &pos1, const QPoint &vel1, const QPoint &pos2, const QPoint &vel2)
{
    ui->pushButton->setGeometry(QRect(pos1, ui->pushButton->size()));
    ui->pushButton_2->setGeometry(QRect(pos2, ui->pushButton_2->size()));
    m_vel1 = vel1;
    m_vel2 = vel2;
    m_timer.start();
}

void MainWindow::pause()
{
    m_timer.stop();
}

void MainWindow::setPosition(QPoint pos1, QPoint pos2)
{
    ui->pushButton->setGeometry(QRect(pos1, ui->pushButton->size()));
    ui->pushButton_2->setGeometry(QRect(pos2, ui->pushButton_2->size()));
}
