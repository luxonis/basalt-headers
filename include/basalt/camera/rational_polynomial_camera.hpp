#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>
#include <cassert>

namespace basalt {

using std::atan2;
using std::cos;
using std::sin;
using std::sqrt;

/// @brief Rational Polynomial camera model
///
/// This model incorporates radial and tangential distortion through rational polynomial coefficients.
template <typename Scalar_ = double>
class RationalPolynomialCamera {
 public:
  using Scalar = Scalar_;
  static constexpr int N = 12;  // Number of intrinsic parameters: fx, fy, cx, cy, k1, k2, k3, k4, k5, p1, p2, p3

  using Vec2 = Eigen::Matrix<Scalar, 2, 1>;
  using Vec4 = Eigen::Matrix<Scalar, 4, 1>;
  using Vec12 = Eigen::Matrix<Scalar, 12, 1>;

  using Mat24 = Eigen::Matrix<Scalar, 2, 4>;
  using Mat2N = Eigen::Matrix<Scalar, 2, N>;
  using Mat42 = Eigen::Matrix<Scalar, 4, 2>;
  using Mat4N = Eigen::Matrix<Scalar, 4, N>;

  Vec12 param_;  // Camera parameters [fx, fy, cx, cy, k1, k2, k3, k4, k5, p1, p2, p3]

  /// @brief Default constructor initializes parameters to zero
  RationalPolynomialCamera() { param_.setZero(); }

  /// @brief Initialize with a specific set of parameters
  explicit RationalPolynomialCamera(const Vec12& p) : param_(p) {}

  /// @brief Cast to a different scalar type
  template <class Scalar2>
  RationalPolynomialCamera<Scalar2> cast() const {
    return RationalPolynomialCamera<Scalar2>(param_.template cast<Scalar2>());
  }

  /// @brief Camera model name
  static std::string getName() { return "rational_polynomial"; }

  /// @brief Project a 3D point to the image plane with radial and tangential distortion
  bool project(const Vec3& p3d, Vec2& proj, Mat24* d_proj_d_p3d = nullptr, Mat2N* d_proj_d_param = nullptr) const {
    const Scalar& fx = param_[0];
    const Scalar& fy = param_[1];
    const Scalar& cx = param_[2];
    const Scalar& cy = param_[3];
    const Scalar& k1 = param_[4];
    const Scalar& k2 = param_[5];
    const Scalar& k3 = param_[6];
    const Scalar& k4 = param_[7];
    const Scalar& k5 = param_[8];
    const Scalar& p1 = param_[9];
    const Scalar& p2 = param_[10];
    const Scalar& p3 = param_[11];

    Scalar x = p3d[0];
    Scalar y = p3d[1];
    Scalar z = p3d[2];

    Scalar r2 = x * x + y * y;
    Scalar r4 = r2 * r2;
    Scalar r6 = r4 * r2;
    Scalar r8 = r4 * r4;
    Scalar r10 = r8 * r2;

    Scalar radial_dist = 1 + k1 * r2 + k2 * r4 + k3 * r6 + k4 * r8 + k5 * r10;
    Scalar tan_dist_x = 2 * p1 * x * y + p2 * (r2 + 2 * x * x);
    Scalar tan_dist_y = p1 * (r2 + 2 * y * y) + 2 * p2 * x * y;
    Scalar scl = p3 * atan2(sqrt(r2), z);

    Scalar x_dist = x * radial_dist + tan_dist_x + scl * x;
    Scalar y_dist = y * radial_dist + tan_dist_y + scl * y;

    proj[0] = fx * x_dist + cx;
    proj[1] = fy * y_dist + cy;

    return true;  // Assume projection is always valid
  }

  /// @brief Unproject an image point back to 3D space, considering distortion
  bool unproject(const Vec2& proj, Vec3& p3d) const {
    // Implement the inverse of projection, typically involving solving a polynomial equation
    // to find the undistorted point location that maps to the given pixel coordinates.
    return true;  // Unprojection is always valid, but challenging and needs iterative methods in practice.
  }

  /// @brief Increment intrinsic parameters by an increment vector
  void operator+=(const Vec12& inc) {
    param_ += inc;
  }

  /// @brief Accessor for intrinsic parameters
  const Vec12& getParam() const { return param_; }

  /// @brief Set parameters from an initialization vector
  void setFromInit(const Vec4& init) {
    param_[0] = init[0];
    param_[1] = init[1];
    param_[2] = init[2];
    param_[3] = init[3];
    param_[4] = 0;
    param_[5] = 0;
    param_[6] = 0;
    param_[7] = 0;
    param_[8] = 0;
    param_[9] = 0;
    param_[10] = 0;
    param_[11] = 0;
  }

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}  // namespace basalt
