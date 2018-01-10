#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void stepResult(const QPoint& pos1,
                    const QPoint& vel1,
                    const QPoint& pos2,
                    const QPoint& vel2,
                    QVector<std::pair<uint, uint>> collisions);

public slots:
    void start(const QPoint& vel1,
               const QPoint& vel2);

    void startWithPos(const QPoint& pos1,
                      const QPoint& vel1,
                      const QPoint& pos2,
                      const QPoint& vel2);
    void pause();

    void setPosition(QPoint pos1, QPoint pos2);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void step();

private:
    void checkBorderCollision(const QRect& rect1, const QRect& rect2, QVector<std::pair<uint, uint> > &collisions);
    void checkButtonCollision(const QRect& rect1, const QRect& rect2, QVector<std::pair<uint, uint> > &collisions);

    Ui::MainWindow *ui;
    QTimer m_timer;
    QPoint m_vel1, m_vel2;
};

#endif // MAINWINDOW_H
