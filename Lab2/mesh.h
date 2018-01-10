#ifndef MESH_H
#define MESH_H

#include "vector"
#include <string>

#include "quaternion.h"
#include "vector3.h"

class Mesh
{
public:
    Mesh() : m_vertices(nullptr), m_normals(nullptr), m_amountOfVertices(0) {}

    explicit Mesh(const std::string& meshFileName,
                  const Vector3& position = Vector3(),
                  const Quaternion& rotation = Quaternion(),
                  const Vector3& scale = Vector3(1, 1, 1));

    Mesh(Mesh&& rhl) noexcept;

    Mesh(const Mesh& rhl);

    ~Mesh();

    void setMesh(const std::string& meshFileName);

    inline const double* vertexArray() const
    {
        return m_vertices;
    }

    inline const double* normalArray()
    {
        return m_normals;
    }

    inline std::size_t amountOfVertices() const
    {
        return m_amountOfVertices;
    }

    void draw() const;

    void setColor(float r, float g, float b, float a);

    inline const float* getColor()
    {
        return m_color;
    }

    Mesh& operator=(const Mesh& rhl);

    Vector3 position;
    Quaternion rotation;
    Vector3 scale = Vector3(1, 1, 1);

    bool solid = true;

private:
    void copyVertices(double *vertices,
                      double *normals,
                      const std::size_t& amountOfVertices,
                      double* gridVertices = nullptr);

    void copyColors(double* solidColors, double* gridColors, double const* color);

    void fillColors();

    void generateGrid();

    void drawSolid() const;

    void drawGrid() const;

    std::string m_meshFileName;
    double* m_vertices = nullptr;
    double* m_normals = nullptr;
    double* m_gridVertices = nullptr;
    float* m_color = new float[4];
    std::size_t m_amountOfVertices, m_amountOfData, m_amountOfGridData;
};

#endif // MESH_H
