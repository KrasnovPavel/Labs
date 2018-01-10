#include "quaternion.h"

Quaternion::Quaternion(const double &angle, const Vector3 &axis)
{
    vector = axis.normalized() * std::sin(angle/2);
    scalar = std::cos(angle/2);
    calculate();
    normalize();
}

Quaternion::Quaternion(const double &roll, const double &pitch, const double &yaw)
{
    Quaternion r = Quaternion(roll, Vector3(1, 0, 0));
    Quaternion p = Quaternion(pitch, Vector3(0, 1, 0));
    Quaternion y = Quaternion(yaw, Vector3(0, 0, 1));

    *this = r * p * y;
    calculate();
    normalize();
}

Vector3& Quaternion::rotateVector(Vector3 &vector) const
{
    Quaternion v(0, vector.x, vector.y, vector.z);
    vector = (*this * v * inverted()).vector;
    return vector;
}

Vector3 Quaternion::rotatedVector(Vector3 vector) const
{
    return rotateVector(vector);
}

Quaternion &Quaternion::operator*=(const Quaternion &rhs)
{
    double tmp = scalar * rhs.scalar - vector.dot(rhs.vector);
    vector = Vector3::cross(vector, rhs.vector) + vector*rhs.scalar + rhs.vector*scalar;
    scalar = tmp;
    calculate();
    normalize();
    return *this;
}

void Quaternion::calculate()
{
    m_invLength = 1 / std::sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z + scalar*scalar);
    m_angle = 2*std::acos(scalar);
    m_axis = vector / std::sqrt(1-scalar*scalar);
}

Quaternion::Quaternion(const double &w, const double &x, const double &y, const double &z)
        : scalar(w), vector(x, y, z)
{
    calculate();
}

std::pair<Vector3, double> Quaternion::toAxisAngle() const
{
    return std::make_pair(m_axis, m_angle);
}

std::string Quaternion::toString() const
{
    auto tmp = toAxisAngle();
    return "{" + tmp.first.toString() + ", " + std::to_string(tmp.second * 180 * M_1_PI) + "}";
}
