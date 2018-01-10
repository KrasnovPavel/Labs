#include "mesh.h"
#include "meshparser.h"
#include <GL/gl.h>
#include <GL/glut.h>

#include <string.h>

Mesh::Mesh(const std::string& meshFileName, const Vector3 &position, const Quaternion &rotation, const Vector3 &scale)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    setMesh(meshFileName);
}

Mesh::Mesh(Mesh &&rhl) noexcept
{
    position = rhl.position;
    rotation = rhl.rotation;
    scale    = rhl.scale;
    copyVertices(rhl.m_vertices, rhl.m_normals, rhl.m_amountOfVertices, rhl.m_gridVertices);
    memcpy(m_color, rhl.m_color, 4*sizeof(float));
}

Mesh::Mesh(const Mesh &rhl)
{
    position = rhl.position;
    rotation = rhl.rotation;
    scale    = rhl.scale;
    copyVertices(rhl.m_vertices, rhl.m_normals, rhl.m_amountOfVertices, rhl.m_gridVertices);
    memcpy(m_color, rhl.m_color, 4*sizeof(float));
}

void Mesh::setMesh(const std::string &meshFileName)
{
    m_meshFileName = meshFileName;
    auto tmp = MeshParser::parse(meshFileName);
    copyVertices(tmp.first.data(), tmp.second.data(), tmp.first.size()/3);
    setColor(1.f, 1.f, 1.f, 1.f);
}

Mesh &Mesh::operator=(const Mesh &rhl)
{
    position = rhl.position;
    rotation = rhl.rotation;
    scale    = rhl.scale;
    copyVertices(rhl.m_vertices, rhl.m_normals, rhl.m_amountOfVertices, rhl.m_gridVertices);
    memcpy(m_color, rhl.m_color, 4*sizeof(float));
    return  *this;
}

void Mesh::copyVertices(double *vertices,
                        double *normals,
                        const std::size_t& amountOfVertices,
                        double* gridVertices)
{
    delete[] m_vertices;
    delete[] m_normals;
    m_amountOfVertices = amountOfVertices;
    m_amountOfData = m_amountOfVertices * 3;
    m_amountOfGridData = m_amountOfVertices * 6;
    m_vertices = new double[m_amountOfData];
    m_normals = new double[m_amountOfData];
    memcpy(m_vertices, vertices, m_amountOfData * sizeof(double));
    memcpy(m_normals, normals, m_amountOfData * sizeof(double));
    delete[] m_gridVertices;
    m_gridVertices = new double[m_amountOfGridData];
    if (gridVertices) memcpy(m_gridVertices, gridVertices, m_amountOfGridData*sizeof(double));
    else generateGrid();

}

void Mesh::draw() const
{
    if (solid) drawSolid();
    else drawGrid();
}

void Mesh::setColor(float r, float g, float b, float a)
{
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
    m_color[3] = a;
}

Mesh::~Mesh()
{
    delete[] m_vertices;
    delete[] m_normals;
    delete[] m_gridVertices;
}

void Mesh::generateGrid()
{
    for (int i = 0; i < m_amountOfVertices/3; ++i)
    {
        memcpy(m_gridVertices+i*6*3, m_vertices+i*3*3, 3*sizeof(double));
        memcpy(m_gridVertices+i*6*3+3*1, m_vertices+i*3*3+3*1, 3*sizeof(double));
        memcpy(m_gridVertices+i*6*3+3*2, m_vertices+i*3*3+3*1, 3*sizeof(double));
        memcpy(m_gridVertices+i*6*3+3*3, m_vertices+i*3*3+3*2, 3*sizeof(double));
        memcpy(m_gridVertices+i*6*3+3*4, m_vertices+i*3*3+3*2, 3*sizeof(double));
        memcpy(m_gridVertices+i*6*3+3*5, m_vertices+i*3*3, 3*sizeof(double));
    }
}

void Mesh::drawSolid() const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    auto tmp = rotation.toAxisAngle();
    glTranslated(position.x, position.y, position.z);
    glRotated(tmp.second * 180. * M_1_PI, tmp.first.x, tmp.first.y, tmp.first.z);
    glScaled(scale.x, scale.y, scale.z);

    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, m_vertices);
    glNormalPointer(GL_DOUBLE, 0, m_normals);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_color);
    glDrawArrays(GL_TRIANGLES, 0, m_amountOfVertices);

    glPopMatrix();
}

void Mesh::drawGrid() const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    auto tmp = rotation.toAxisAngle();
    glTranslated(position.x, position.y, position.z);
    glRotated(tmp.second * 180. * M_1_PI, tmp.first.x, tmp.first.y, tmp.first.z);

    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, m_gridVertices);
    glMaterialfv(GL_FRONT, GL_EMISSION, m_color);
    float noMaterial[] = {0,0,0,1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, noMaterial);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, noMaterial);
    glLightfv(GL_LIGHT0, GL_SPECULAR, noMaterial);
    glDrawArrays(GL_LINES, 0, m_amountOfVertices*2);
    glMaterialfv(GL_FRONT, GL_EMISSION, noMaterial);
    GLfloat lightAmbient[]  = {0.0, 0.0, 0.0, 1.0};
    GLfloat lightDiffuse[]  = {0.7, 0.7, 0.7, 1.0};
    GLfloat lightSpecular[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glPopMatrix();
}
