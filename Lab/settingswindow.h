#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

public slots:
    void step(const QPoint& pos1,
              const QPoint& vel1,
              const QPoint& pos2,
              const QPoint& vel2,
              QVector<std::pair<uint, uint>> collisions);

private slots:
    void start_pause();
    void setState(bool state);
    void saveData();
    void loadData();
    void nextStep();
    void prevStep();
    void resetData();
    void selectRow();
    void setData();

signals:
    void newTimerDelay(double newDelay);
    void start(const QPoint& vel1,
               const QPoint& vel2);

    void startWithPos(const QPoint& pos1,
                      const QPoint& vel1,
                      const QPoint& pos2,
                      const QPoint& vel2);
    void pause();
    void setPosition(QPoint pos1, QPoint pos2);

private:
    Ui::SettingsWindow *ui;
    bool m_state = true;
    int m_selectedRow;
    QTableWidgetItem *x1;
    QTableWidgetItem *y1;
    QTableWidgetItem *dx1;
    QTableWidgetItem *dy1;
    QTableWidgetItem *x2;
    QTableWidgetItem *y2;
    QTableWidgetItem *dx2;
    QTableWidgetItem *dy2;
    QTableWidgetItem *misc;
    QVector<QString> m_collisionsStrings;
};

#endif // SETTINGSWINDOW_H
