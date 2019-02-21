
#ifndef _EGG_ENGINE_MATRIX_H_
#define _EGG_ENGINE_MATRIX_H_

#include <eggRuntime/Math.h>

//#include <vmmlib/frustum.hpp>
//#include <vmmlib/matrix.hpp>
//#include <vmmlib/vector.hpp>

/*
vmml 说明
目前使用的是 https://github.com/Eyescale/vmmlib/tree/master/vmmlib 的版本


默认初始化的matrix是全部为零的，需要初始化单位矩阵。
vmml::mat4f m;
vmml::identity(m);

视锥体是个独立计算对象
vmml::frustumf f;
f.set_perspective(30, 1.0, 0.01, 1000.0);
mProjactiveMatrix = f.compute_matrix();

//

*/

#include <Eigen/Eigen>



/*
http://spointeau.blogspot.com/2013/12/hello-i-am-looking-at-opengl-3.html
*/

template<class T>
Eigen::Matrix<T, 4, 4> perspective
(
    double fovy,
    double aspect,
    double zNear,
    double zFar
)
{
    typedef Eigen::Matrix<T, 4, 4> Matrix4;

    assert(aspect > 0);
    assert(zFar > zNear);

    //double radf = Math::degToRad(fovy);
    double radf = fovy/180.0*M_PI;

    double tanHalfFovy = tan(radf / 2.0);
    Matrix4 res = Matrix4::Zero();
    res(0, 0) = 1.0 / (aspect * tanHalfFovy);
    res(1, 1) = 1.0 / (tanHalfFovy);
    res(2, 2) = -(zFar + zNear) / (zFar - zNear);
    res(3, 2) = -1.0;
    res(2, 3) = -(2.0 * zFar * zNear) / (zFar - zNear);
    return res;
}

template<class T>
Eigen::Matrix<T, 4, 4> lookAt
(
    Eigen::Matrix<T, 3, 1> const & eye,
    Eigen::Matrix<T, 3, 1> const & center,
    Eigen::Matrix<T, 3, 1> const & up
)
{
    typedef Eigen::Matrix<T, 4, 4> Matrix4;
    typedef Eigen::Matrix<T, 3, 1> Vector3;

    Vector3 f = (center - eye).normalized();
    Vector3 u = up.normalized();
    Vector3 s = f.cross(u).normalized();
    u = s.cross(f);

    Matrix4 res;
    res << s.x(), s.y(), s.z(), -s.dot(eye),
        u.x(), u.y(), u.z(), -u.dot(eye),
        -f.x(), -f.y(), -f.z(), f.dot(eye),
        0, 0, 0, 1;

    return res;
}


/*
https://github.com/OpenGP/htrack/blob/master/util/eigen_opengl_helpers.h
*/

namespace Eigen {

    /// @brief Returns a perspective transformation matrix like the one from gluPerspective
    /// @see http://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
    /// @see glm::perspective
    template<typename Scalar>
    Eigen::Matrix<Scalar, 4, 4> perspective(Scalar fovy, Scalar aspect, Scalar zNear, Scalar zFar) {
        Transform<Scalar, 3, Projective> tr;
        tr.matrix().setZero();
        assert(aspect > 0);
        assert(zFar > zNear);
        assert(zNear > 0);
        Scalar radf = M_PI * fovy / 180.0;
        Scalar tan_half_fovy = std::tan(radf / 2.0);
        tr(0, 0) = 1.0 / (aspect * tan_half_fovy);
        tr(1, 1) = 1.0 / (tan_half_fovy);
        tr(2, 2) = -(zFar + zNear) / (zFar - zNear);
        tr(3, 2) = -1.0;
        tr(2, 3) = -(2.0 * zFar * zNear) / (zFar - zNear);
        return tr.matrix();
    }

    template<typename Scalar>
    Eigen::Matrix<Scalar, 4, 4> scale(Scalar x, Scalar y, Scalar z) {
        Transform<Scalar, 3, Affine> tr;
        tr.matrix().setZero();
        tr(0, 0) = x;
        tr(1, 1) = y;
        tr(2, 2) = z;
        tr(3, 3) = 1;
        return tr.matrix();
    }

    template<typename Scalar>
    Eigen::Matrix<Scalar, 4, 4> translate(Scalar x, Scalar y, Scalar z) {
        Transform<Scalar, 3, Affine> tr;
        tr.matrix().setIdentity();
        tr(0, 3) = x;
        tr(1, 3) = y;
        tr(2, 3) = z;
        return tr.matrix();
    }

    /// @brief Returns a view transformation matrix like the one from glu's lookAt
    /// @see http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
    /// @see glm::lookAt
    template<typename Derived>
    Eigen::Matrix<typename Derived::Scalar, 4, 4> lookAt(Derived const & eye, Derived const & center, Derived const & up) {
        typedef Eigen::Matrix<typename Derived::Scalar, 4, 4> Matrix4;
        typedef Eigen::Matrix<typename Derived::Scalar, 3, 1> Vector3;
        Vector3 f = (center - eye).normalized();
        Vector3 u = up.normalized();
        Vector3 s = f.cross(u).normalized();
        u = s.cross(f);
        Matrix4 mat = Matrix4::Zero();
        mat(0, 0) = s.x();
        mat(0, 1) = s.y();
        mat(0, 2) = s.z();
        mat(0, 3) = -s.dot(eye);
        mat(1, 0) = u.x();
        mat(1, 1) = u.y();
        mat(1, 2) = u.z();
        mat(1, 3) = -u.dot(eye);
        mat(2, 0) = -f.x();
        mat(2, 1) = -f.y();
        mat(2, 2) = -f.z();
        mat(2, 3) = f.dot(eye);
        mat.row(3) << 0, 0, 0, 1;
        return mat;
    }

    /// @see glm::ortho
    template<typename Scalar>
    Eigen::Matrix<Scalar, 4, 4> ortho(Scalar const& left,
        Scalar const& right,
        Scalar const& bottom,
        Scalar const& top,
        Scalar const& zNear,
        Scalar const& zFar) {
        Eigen::Matrix<Scalar, 4, 4> mat = Eigen::Matrix<Scalar, 4, 4>::Identity();
        mat(0, 0) = Scalar(2) / (right - left);
        mat(1, 1) = Scalar(2) / (top - bottom);
        mat(2, 2) = -Scalar(2) / (zFar - zNear);
        mat(3, 0) = -(right + left) / (right - left);
        mat(3, 1) = -(top + bottom) / (top - bottom);
        mat(3, 2) = -(zFar + zNear) / (zFar - zNear);
        return mat;
    }

} /// eigen::


namespace Eigen {

    template<typename Scalar>
    Eigen::Matrix<Scalar, 4, 4> rotate(Scalar a, Scalar x, Scalar y, Scalar z) {
        Transform<Scalar, 3, Affine> tr;
        Matrix<Scalar, 3, 1> v(x, y, z);
        tr.matrix().setIdentity();
        tr.rotate(Eigen::AngleAxisf(a, v));
        return tr.matrix();
    }

} /// eigen::


#endif // !_EGG_ENGINE_MATRIX_H_
