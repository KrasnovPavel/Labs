#include "meshwidget.h"
#include "ui_meshwidget.h"

#include <cmath>

#include <QCheckBox>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QPixmap>

const double MeshWidget::beta = M_PI / 180;
const double MeshWidget::alpha = 0.02 * MeshWidget::beta;

MeshWidget::MeshWidget(const int &number, const QString &name, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MeshWidget)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &MeshWidget::toggleSettings);
    connect(ui->removeButton, &QPushButton::clicked, this, &MeshWidget::remove);
    connect(ui->visibleCheckBox, &QCheckBox::toggled, this, &MeshWidget::visibilityChanged);
    connect(ui->gridCheckBox, &QCheckBox::toggled, this, &MeshWidget::gridStateChanged);
    connect(ui->rotationCheckBox, &QCheckBox::toggled, this, &MeshWidget::rotatingChanged);
    connect(ui->colorButton, &QPushButton::clicked, this, &MeshWidget::changeColor);
    connect(ui->xSlider, &QSlider::valueChanged, this, &MeshWidget::positionChanged);
    connect(ui->ySlider, &QSlider::valueChanged, this, &MeshWidget::positionChanged);
    connect(ui->zSlider, &QSlider::valueChanged, this, &MeshWidget::positionChanged);
    connect(ui->rollSlider, &QSlider::valueChanged, this, &MeshWidget::angleChanged);
    connect(ui->pitchSlider, &QSlider::valueChanged, this, &MeshWidget::angleChanged);
    connect(ui->yawSlider, &QSlider::valueChanged, this, &MeshWidget::angleChanged);
    ui->nameLabel->setText(tr("Model №%1 (").arg(number) + name + ")");
    drawColorButton();
}

MeshWidget::~MeshWidget()
{
    delete ui;
}

const Quaternion &MeshWidget::angle() const
{
    return m_angle;
}

const Vector3 &MeshWidget::position() const
{
    return m_pos;
}

const QColor &MeshWidget::color() const
{
    return m_color;
}

const bool& MeshWidget::isVisible() const
{
    return m_isVisible;
}

const bool& MeshWidget::isGrid() const
{
    return m_isGrid;
}

const bool& MeshWidget::isRotating() const
{
    return m_isRotating;
}

void MeshWidget::angleChanged()
{
    const double gamma = m_isRotating?alpha:beta;
    m_angle = Quaternion(ui->rollSlider->value() * gamma,
                         ui->pitchSlider->value() * gamma,
                         ui->yawSlider->value() * gamma);
}

void MeshWidget::positionChanged()
{
    m_pos.set(ui->xSlider->value(),
              ui->ySlider->value(),
              ui->zSlider->value());
}

void MeshWidget::changeColor()
{
    m_color = QColorDialog::getColor();
    drawColorButton();
}

void MeshWidget::toggleSettings()
{
    ui->settings->setVisible(!ui->settings->isVisible());
    if (ui->settings->isVisible())
    {
        ui->openButton->setIcon(QIcon(":/Icons/downTriangle.svg"));
    }
    else
    {
        ui->openButton->setIcon(QIcon(":/Icons/rightTriangle.svg"));
    }
}

void MeshWidget::drawColorButton()
{
    QPixmap p(16, 16);
    p.fill(m_color);
    ui->colorButton->setIcon(QIcon(p));
    ui->colorButton->setIconSize(QSize(16, 16));
}

void MeshWidget::visibilityChanged(const bool& newState)
{
    m_isVisible = newState;
}

void MeshWidget::gridStateChanged(const bool& newState)
{
    m_isGrid = newState;
}

void MeshWidget::rotatingChanged(const bool& newState)
{
    m_isRotating = newState;
    angleChanged();
}
