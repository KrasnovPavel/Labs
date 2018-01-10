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

    m_collisionsStrings = {"Кнопка №1", "Кнопка №2", "Верхняя граница",
                           "Левая граница", "Нижняя граница", "Правая граница"};

    setState(true);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::step(const QPoint &pos1, const QPoint &vel1, const QPoint &pos2, const QPoint &vel2, QVector<std::pair<uint, uint> > collisions)
{
    ui->table->setRowCount(ui->table->rowCount()+1);
    x1 = new QTableWidgetItem(QString("%1").arg(pos1.x()));
    y1 = new QTableWidgetItem(QString("%1").arg(pos1.y()));
    dx1 = new QTableWidgetItem(QString("%1").arg(vel1.x()));
    dy1 = new QTableWidgetItem(QString("%1").arg(vel1.y()));
    x2 = new QTableWidgetItem(QString("%1").arg(pos2.x()));
    y2 = new QTableWidgetItem(QString("%1").arg(pos2.y()));
    dx2 = new QTableWidgetItem(QString("%1").arg(vel2.x()));
    dy2 = new QTableWidgetItem(QString("%1").arg(vel2.y()));
    QString col = "";
    for (int i = 0; i < collisions.size(); ++i)
    {
        col += "(" + m_collisionsStrings.at(collisions.at(i).first-1) + "<>" +
               m_collisionsStrings.at(collisions.at(i).second-1) + ")";
    }
    misc = new QTableWidgetItem(col);
    ui->table->setItem(ui->table->rowCount()-1, 0, x1);
    ui->table->setItem(ui->table->rowCount()-1, 1, y1);
    ui->table->setItem(ui->table->rowCount()-1, 2, dx1);
    ui->table->setItem(ui->table->rowCount()-1, 3, dy1);
    ui->table->setItem(ui->table->rowCount()-1, 4, x2);
    ui->table->setItem(ui->table->rowCount()-1, 5, y2);
    ui->table->setItem(ui->table->rowCount()-1, 6, dx2);
    ui->table->setItem(ui->table->rowCount()-1, 7, dy2);
    ui->table->setItem(ui->table->rowCount()-1, 8, misc);
    ui->spinBox_dx1->setValue(std::round(vel1.x()));
    ui->spinBox_dy1->setValue(std::round(vel1.y()));
    ui->spinBox_dx2->setValue(std::round(vel2.x()));
    ui->spinBox_dy2->setValue(std::round(vel2.y()));
}

void SettingsWindow::start_pause()
{
    setState(!m_state);
    if (!m_state)
    {
        if (ui->table->rowCount() > 0)
        {
            ui->table->setRowCount(m_selectedRow+1);
            emit startWithPos(QPoint(ui->table->item(m_selectedRow, 0)->text().toInt(),
                                     ui->table->item(m_selectedRow, 1)->text().toInt()),
                              QPoint(ui->table->item(m_selectedRow, 2)->text().toInt(),
                                     ui->table->item(m_selectedRow, 3)->text().toInt()),
                              QPoint(ui->table->item(m_selectedRow, 4)->text().toInt(),
                                     ui->table->item(m_selectedRow, 5)->text().toInt()),
                              QPoint(ui->table->item(m_selectedRow, 6)->text().toInt(),
                                     ui->table->item(m_selectedRow, 7)->text().toInt()));
        }
        else
        {
            emit start(QPoint(ui->spinBox_dx1->value(), ui->spinBox_dy1->value()),
                       QPoint(ui->spinBox_dx2->value(), ui->spinBox_dy2->value()));
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
    ui->spinBox_dx1->setEnabled(state);
    ui->spinBox_dy1->setEnabled(state);
    ui->spinBox_dx2->setEnabled(state);
    ui->spinBox_dy2->setEnabled(state);
    ui->nextButton->setEnabled(state);
    ui->previousButton->setEnabled(state);
    ui->resetButton->setEnabled(state);
    ui->saveButton->setEnabled(state);
    ui->loadButton->setEnabled(state);
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
            for (int i = 0; i < ui->table->columnCount() && i < wordList.size(); ++i)
            {
                misc = new QTableWidgetItem(wordList.at(i));
                ui->table->setItem(ui->table->rowCount()-1, i, misc);
            }
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
    ui->spinBox_dx1->setValue(ui->table->item(m_selectedRow, 2)->text().toInt());
    ui->spinBox_dy1->setValue(ui->table->item(m_selectedRow, 3)->text().toInt());
    ui->spinBox_dx2->setValue(ui->table->item(m_selectedRow, 6)->text().toInt());
    ui->spinBox_dy2->setValue(ui->table->item(m_selectedRow, 7)->text().toInt());
    emit setPosition(QPoint(ui->table->item(m_selectedRow, 0)->text().toInt(),
                            ui->table->item(m_selectedRow, 1)->text().toInt()),
                     QPoint(ui->table->item(m_selectedRow, 4)->text().toInt(),
                            ui->table->item(m_selectedRow, 5)->text().toInt()));
}
