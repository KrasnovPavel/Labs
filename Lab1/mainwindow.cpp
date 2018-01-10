#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdlib>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::step);
    m_timer.setInterval(100);
    std::srand(std::time(0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::step()
{
    QVector<std::pair<uint, uint>> collisions;
    for (int i = 0; i < m_buttons.size(); ++i)
    {
        QRect rect = QRect(m_buttons.at(i)->pos() + m_buttonsData.at(i), m_buttons.at(i)->size());
        m_buttons.at(i)->setGeometry(rect);

        checkBorderCollision(rect, i, collisions);
    }
    QVector<std::pair<QPoint, QPoint>> res;
    res.reserve(m_buttons.size());

    for (int i = 0; i < m_buttons.size(); ++i)
    {
        QRect rect1 = m_buttons.at(i)->geometry();
        for (int j = i+1; j < m_buttons.size(); ++j)
        {
            QRect rect2 = m_buttons.at(j)->geometry();
            checkButtonCollision(rect1, rect2, i, j, collisions);
        }
        res.append(std::make_pair(m_buttons.at(i)->pos(), m_buttonsData.at(i)));
    }

    emit stepResult(res, collisions);
}

void MainWindow::checkBorderCollision(const QRect &rect, int number, QVector<std::pair<uint, uint>>& collisions)
{
    if (rect.top() <= 0)
    {
        m_buttonsData[number] = QPoint(m_buttonsData[number].x(), -m_buttonsData[number].y());
        collisions.append(std::make_pair(number+4, 0));
    }
    if (rect.left() <= 0)
    {
        m_buttonsData[number] = QPoint(-m_buttonsData[number].x(), m_buttonsData[number].y());
        collisions.append(std::make_pair(number+4, 1));
    }
    if (rect.bottom() >= ui->centralWidget->height())
    {
        m_buttonsData[number] = QPoint(m_buttonsData[number].x(), -m_buttonsData[number].y());
        collisions.append(std::make_pair(number+4, 2));
    }
    if (rect.right() >= ui->centralWidget->width())
    {
        m_buttonsData[number] = QPoint(-m_buttonsData[number].x(), m_buttonsData[number].y());
        collisions.append(std::make_pair(number+4, 3));
    }
}

void MainWindow::checkButtonCollision(const QRect &rect1, const QRect &rect2,
                                      int number1, int number2,
                                      QVector<std::pair<uint, uint> > &collisions)
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

        QPoint v = m_buttonsData[number2] - m_buttonsData[number1];
        double pr = QPoint::dotProduct(v, normal);
        m_buttonsData[number1] += pr * normal;
        m_buttonsData[number2] -= pr * normal;


        collisions.append(std::make_pair(number1, number2));
    }
}

void MainWindow::start(int buttonsCount)
{
    if (buttonsCount < m_buttons.size())
    {
        for (int i = m_buttons.size()-1; i >= buttonsCount; --i)
        {
            delete m_buttons.at(i);
        }
        m_buttons.resize(buttonsCount);
        m_buttonsData.resize(buttonsCount);
    }
    else
    {
        m_buttonsData.resize(buttonsCount);
        for (int i = m_buttons.size(); i < buttonsCount; ++i)
        {
            m_buttons.push_back(new QPushButton(QString("%1").arg(m_buttons.size()), ui->centralWidget));
            m_buttons.at(i)->show();
            m_buttons.at(i)->setGeometry(QRect(QPoint(std::rand() % (ui->centralWidget->width()-32),
                                                      std::rand() % (ui->centralWidget->height()-32)),
                                               QSize(32, 32)));
            m_buttonsData[i] = QPoint(std::rand() % 10, std::rand() % 10);
        }

    }
    for (int i = 0; i < m_buttons.size(); ++i)
    {
        m_buttons.at(i)->setGeometry(QRect(QPoint(std::rand() % (ui->centralWidget->width()-32),
                                                  std::rand() % (ui->centralWidget->height()-32)),
                                           QSize(32, 32)));
        m_buttonsData[i] = QPoint(std::rand() % 10, std::rand() % 10);
    }
    m_timer.start();
}

void MainWindow::startWithPos(int buttonsCount, QVector<std::pair<QPoint, QPoint>> buttonsData)
{
    if (buttonsCount < m_buttons.size())
    {
        for (int i = m_buttons.size()-1; i >= buttonsCount; --i)
        {
            delete m_buttons.at(i);
        }
        m_buttons.resize(buttonsCount);
        m_buttonsData.resize(buttonsCount);
    }
    else
    {
        m_buttonsData.resize(buttonsCount);
        for (int i = m_buttons.size(); i < buttonsCount; ++i)
        {
            m_buttons.push_back(new QPushButton(QString("%1").arg(m_buttons.size()), ui->centralWidget));
            m_buttons.at(i)->show();
            m_buttons.at(i)->setGeometry(QRect(QPoint(std::rand() % (ui->centralWidget->width()-32),
                                                      std::rand() % (ui->centralWidget->height()-32)),
                                               QSize(32, 32)));
            m_buttonsData[i] = QPoint(std::rand() % 10, std::rand() % 10);
        }
    }
    for (int i = 0; i < m_buttons.size() && i < buttonsData.size(); ++i)
    {
        m_buttons.at(i)->setGeometry(QRect(buttonsData.at(i).first, m_buttons.at(i)->size()));
        m_buttonsData[i] = buttonsData.at(i).second;
    }
    m_timer.start();
}

void MainWindow::pause()
{
    m_timer.stop();
}

void MainWindow::setPosition(QVector<QPoint> buttonsPos)
{
    for (int i = 0; i < m_buttons.size(); ++i)
    {
        delete m_buttons.at(i);
    }
    m_buttons.resize(buttonsPos.size());
    m_buttonsData.resize(buttonsPos.size());
    for (int i = 0; i < m_buttons.size(); ++i)
    {
        m_buttons[i] = new QPushButton(QString("%1").arg(i), ui->centralWidget);
        m_buttons.at(i)->show();
        m_buttons.at(i)->setGeometry(QRect(buttonsPos.at(i), QSize(32, 32)));
    }
}
