#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <cmath>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &SettingsWindow::start_pause);
    connect(ui->nextButton, &QPushButton::clicked, this, &SettingsWindow::nextStep);
    connect(ui->previousButton, &QPushButton::clicked, this, &SettingsWindow::prevStep);
    connect(ui->resetButton, &QPushButton::clicked, this, &SettingsWindow::resetData);
    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsWindow::saveData);
    connect(ui->loadButton, &QPushButton::clicked, this, &SettingsWindow::loadData);
    connect(ui->table, &QTableWidget::cellChanged, this, &SettingsWindow::selectRow);

    m_collisionsStrings = {"Верхняя граница","Левая граница", "Нижняя граница",
                           "Правая граница", "Кнопка №"};

    setState(true);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::step(QVector<std::pair<QPoint, QPoint> > buttons,
                          QVector<std::pair<uint, uint> > collisions)
{
    ui->table->setRowCount(ui->table->rowCount()+1);
    ui->table->setColumnCount(buttons.size()*4+1);

    for (int i = 0; i < buttons.size(); ++i)
    {
        misc = new QTableWidgetItem(QString("%1").arg(buttons.at(i).first.x()));
        ui->table->setItem(ui->table->rowCount()-1, i*4, misc);
        misc = new QTableWidgetItem(QString("%1").arg(buttons.at(i).first.y()));
        ui->table->setItem(ui->table->rowCount()-1, i*4+1, misc);
        misc = new QTableWidgetItem(QString("%1").arg(buttons.at(i).second.x()));
        ui->table->setItem(ui->table->rowCount()-1, i*4+2, misc);
        misc = new QTableWidgetItem(QString("%1").arg(buttons.at(i).second.y()));
        ui->table->setItem(ui->table->rowCount()-1, i*4+3, misc);
    }

    QString col = "";
    for (int i = 0; i < collisions.size(); ++i)
    {
        col += "(" + getCollisionString(collisions.at(i).first) + "<>" +
               getCollisionString(collisions.at(i).second) + ")";
    }
    misc = new QTableWidgetItem(col);
    ui->table->setItem(ui->table->rowCount()-1, ui->table->columnCount()-1, misc);
}

void SettingsWindow::start_pause()
{
    setState(!m_state);
    if (!m_state)
    {
        if (ui->table->rowCount() > 0)
        {
            ui->table->setRowCount(m_selectedRow+1);
            QVector<std::pair<QPoint, QPoint> > buttons;
            for (int i = 0; i < ui->table->columnCount()-2; i += 4)
            {
                buttons.push_back(std::make_pair(QPoint(ui->table->item(m_selectedRow, i)->text().toInt(),
                                                        ui->table->item(m_selectedRow, i+1)->text().toInt()),
                                                 QPoint(ui->table->item(m_selectedRow, i+2)->text().toInt(),
                                                        ui->table->item(m_selectedRow, i+3)->text().toInt())));
            }
            emit startWithPos(ui->spinBox_buttons->value(), buttons);
        }
        else
        {
            emit start(ui->spinBox_buttons->value());
        }
    }
    else
    {
        emit pause();
    }
}

void SettingsWindow::setState(bool state)
{
    ui->startButton->setText(state?"Старт":"Пауза");
    ui->nextButton->setEnabled(state);
    ui->previousButton->setEnabled(state);
    ui->resetButton->setEnabled(state);
    ui->saveButton->setEnabled(state);
    ui->loadButton->setEnabled(state);
    ui->spinBox_buttons->setEnabled(state);
    m_state = state;
}

void SettingsWindow::saveData()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Сохранить данные в файл"), "",
                                                    tr("CSV (*.csv);;All Files (*)"));
    if (fileName.isEmpty()) return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QTextStream out(&file);
        for (int i = 0; i < ui->table->rowCount(); ++i)
        {
            for (int j = 0; j < ui->table->columnCount(); ++j)
            {
                out << ui->table->item(i, j)->text() << ",";
            }
            out << "\n";
        }
        file.close();
    }
}

void SettingsWindow::loadData()
{
    resetData();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Загрузить данные из файла"), "",
                                                    tr("CSV (*.csv);;All Files (*)"));
    if (fileName.isEmpty()) return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QTextStream in(&file);
        while (!in.atEnd())
        {
            ui->table->setRowCount(ui->table->rowCount()+1);
            QString line = in.readLine();
            QStringList wordList = line.split(",");
            ui->table->setColumnCount(wordList.size());
            for (int i = 0; i < ui->table->columnCount(); ++i)
            {
                misc = new QTableWidgetItem(wordList.at(i));
                ui->table->setItem(ui->table->rowCount()-1, i, misc);
            }
            ui->spinBox_buttons->setValue((wordList.size()-1)/4);
        }
        file.close();
    }
    selectRow();
    setData();
}

void SettingsWindow::nextStep()
{
    if (m_selectedRow < ui->table->rowCount()-1)
    {
        ui->table->selectRow(++m_selectedRow);
        setData();
    }
}

void SettingsWindow::prevStep()
{
    if (m_selectedRow > 0)
    {
        ui->table->selectRow(--m_selectedRow);
        setData();
    }
}

void SettingsWindow::resetData()
{
    ui->table->setRowCount(0);
}

void SettingsWindow::selectRow()
{
    m_selectedRow = ui->table->rowCount()-1;
    ui->table->selectRow(m_selectedRow);
}

void SettingsWindow::setData()
{
    QVector<QPoint> buttons;
    for (int i = 0; i < ui->table->columnCount()-2; i += 4)
    {
        buttons.push_back(QPoint(ui->table->item(m_selectedRow, i)->text().toInt(),
                                 ui->table->item(m_selectedRow, i+1)->text().toInt()));
    }
    emit setPosition(buttons);
}

QString SettingsWindow::getCollisionString(int number)
{
    if (number < 4) return m_collisionsStrings.at(number);
    else            return m_collisionsStrings.at(4) + QString("%1").arg(number-4);

}
