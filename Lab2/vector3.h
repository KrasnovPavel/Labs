#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <string>

class Vector3
{
public:
    Vector3() {}

    Vector3(Vector3&& other) noexcept
        : x(other.x), y(other.y), z(other.z){}

    Vector3(const Vector3& other)
        : x(other.x), y(other.y), z(other.z){}

    inline Vector3(const double& x, const double& y, const double& z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void set(const double& x, const double& y, const double& z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    static double dot(const Vector3& first, const Vector3& second);

    inline double dot(const Vector3 &other) const
    {
        return Vector3::dot(*this, other);
    }

    static Vector3 cross(const Vector3& first, const Vector3& second);

    Vector3 cross(const Vector3& other) const
    {
        return Vector3::cross(*this, other);
    }

    inline double length() const
    {
        return std::sqrt(x*x + y*y + z*z);
    }

    inline bool operator ==(const Vector3& rhs) const
    {   //TODO: Epsilon check
        return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
    }

    inline bool operator !=(const Vector3& rhs) const
    {
        return !(*this == rhs);
    }

    inline Vector3& operator -=(const Vector3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    inline Vector3& operator +=(const Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    inline Vector3& operator *=(const double& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    inline Vector3& operator /=(const double& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    friend inline Vector3 operator +(Vector3 lhs, const Vector3& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend inline Vector3 operator -(Vector3 lhs, const Vector3& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend inline Vector3 operator -(Vector3 rhs)
    {
        rhs *= -1;
        return rhs;
    }

    friend inline Vector3 operator *(Vector3 lhs, const double& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    friend inline Vector3 operator /(Vector3 lhs, const double& rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    inline double distance(const Vector3& other) const
    {
        return ((*this) - other).length();
    }

    inline Vector3& operator=(const Vector3& rhl)
    {
        x = rhl.x;
        y = rhl.y;
        z = rhl.z;
        return *this;
    }

    inline Vector3& normalize()
    {
        (*this) /= this->length();
        return *this;
    }

    inline Vector3 normalized() const
    {
        return (*this) / this->length();
    }

    virtual std::string toString() const;

    double x = 0;
    double y = 0;
    double z = 0;
};

#endif // VECTOR3_H
