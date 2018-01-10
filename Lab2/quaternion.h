#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

#include "vector3.h"
class Quaternion
{
public:
    Quaternion() {}

    Quaternion(const double& angle, const Vector3& axis);

    Quaternion(const double& roll, const double& pitch, const double& yaw);

    Vector3& rotateVector(Vector3& vector) const;

    Vector3 rotatedVector(Vector3 vector) const;

    Quaternion& operator *=(const Quaternion& rhs);

    friend inline Quaternion operator *(Quaternion lhs, const Quaternion& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    inline double length() const
    {
        return 1/m_invLength;
    }

    inline Quaternion& normalize()
    {
        (*this) = normalized();
        calculate();
        return *this;
    }

    inline Quaternion normalized() const
    {
        return (*this) * m_invLength;
    }

    Quaternion& operator=(const Quaternion& rhl)
    {
        vector = rhl.vector;
        scalar = rhl.scalar;
        calculate();
        return *this;
    }

    inline bool operator ==(const Quaternion& rhs) const
    {   //TODO: Epsilon check
        return (vector == rhs.vector) && (scalar == rhs.scalar);
    }

    inline bool operator !=(const Quaternion& rhs) const
    {
        return !(*this == rhs);
    }

    inline Quaternion inverted() const
    {
        return Quaternion(scalar, -vector);
    }

    std::pair<Vector3, double> toAxisAngle() const;

    virtual std::string toString() const;

private:
    void calculate();

    Quaternion(const double& w, const double& x, const double& y, const double& z);

    inline Quaternion& operator *=(const double& rhs)
    {
        vector.x *= rhs;
        vector.y *= rhs;
        vector.z *= rhs;
        scalar *= rhs;
        calculate();
        return *this;
    }

    friend inline Quaternion operator *(Quaternion lhs, const double& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    Vector3 vector;
    double scalar = 1;
    double m_invLength, m_angle;
    Vector3 m_axis;
};


#endif // QUATERNION_H
