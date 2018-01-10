#ifndef MESHSETTINGS_H
#define MESHSETTINGS_H

#include <QFrame>
#include <vector3.h>
#include <quaternion.h>

namespace Ui {
class MeshWidget;
}

class MeshWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MeshWidget(const int& number, const QString& name, QWidget *parent = 0);
    ~MeshWidget();

    const Quaternion& angle() const;

    const Vector3& position() const;

    const QColor& color() const;

    const bool& isVisible() const;

    const bool& isGrid() const;

    const bool& isRotating() const;

private slots:
    void angleChanged();
    void positionChanged();
    void visibilityChanged(const bool& newState);
    void gridStateChanged(const bool& newState);
    void rotatingChanged(const bool& newState);
    void changeColor();
    void toggleSettings();
    void drawColorButton();

signals:
    void remove();

private:
    Quaternion m_angle;
    Vector3 m_pos;
    QColor m_color;
    bool m_isVisible = true;
    bool m_isGrid = false;
    bool m_isRotating = true;

    const static double alpha;
    const static double beta;

    Ui::MeshWidget *ui;
};

#endif // MESHSETTINGS_H
