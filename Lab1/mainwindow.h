#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void stepResult(QVector<std::pair<QPoint, QPoint>> buttons,
                    QVector<std::pair<uint, uint>> collisions);

public slots:
    void start(int buttonsCount);

    void startWithPos(int buttonsCount, QVector<std::pair<QPoint, QPoint> > buttonsData);
    void pause();

    void setPosition(QVector<QPoint> buttonsPos);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void step();

private:
    void checkBorderCollision(const QRect& rect, int number, QVector<std::pair<uint, uint> > &collisions);
    void checkButtonCollision(const QRect& rect1, const QRect& rect2, int number1,
                              int num2, QVector<std::pair<uint, uint> > &collisions);

    Ui::MainWindow *ui;
    QTimer m_timer;
    QVector<QPoint> m_buttonsData;
    QVector<QPushButton*> m_buttons;
};

#endif // MAINWINDOW_H
