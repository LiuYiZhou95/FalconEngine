#pragma once

#include <glm/matrix.hpp>

namespace FalconEngine
{

class Vector3f;
class Matrix4f;
// @summary 3x3 matrix in column major format:
//
// (0.0, 1.0, 2.0, 3.0,
//  0.1, 1.1, 2.1, 3.1,
//  0.2, 1.2, 2.2, 3.2,
//  0.3, 1.3, 2.3, 3.3)
class Matrix3f : public glm::mat3
{
public:
    /************************************************************************/
    /* Static Members                                                       */
    /************************************************************************/
    static Matrix3f Zero;
    static Matrix3f Identity;

    static Matrix3f Inverse(const Matrix3f& mat);
    static Matrix3f Transpose(const Matrix3f& mat);

    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    Matrix3f();
    Matrix3f(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    Matrix3f(
        const float& x0, const float& y0, const float& z0,
        const float& x1, const float& y1, const float& z1,
        const float& x2, const float& y2, const float& z2);

    virtual ~Matrix3f();

    // Explicit Conversion
    explicit Matrix3f(const Matrix4f& m);

    // Implicit Conversion
    Matrix3f(const glm::mat3& m);
    operator glm::mat3();
};

}