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
    void step(QVector<std::pair<QPoint, QPoint>> buttons,
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
    QString getCollisionString(int number);

signals:
    void newTimerDelay(double newDelay);
    void start(int buttonsCount);

    void startWithPos(int buttonsCount, QVector<std::pair<QPoint, QPoint> > buttonsData);
    void pause();
    void setPosition(QVector<QPoint> buttonsPos);

private:
    Ui::SettingsWindow *ui;
    bool m_state = true;
    int m_selectedRow;
    QTableWidgetItem *misc;
    QVector<QString> m_collisionsStrings;
};

#endif // SETTINGSWINDOW_H
