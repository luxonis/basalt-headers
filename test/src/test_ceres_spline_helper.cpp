

#include <iostream>

#include "gtest/gtest.h"

#include <basalt/spline/ceres_spline_helper.h>
#include <basalt/spline/rd_spline.h>
#include <basalt/spline/so3_spline.h>

template <int N>
void test_ceres_spline_helper_so3() {
  static const int64_t dt_ns = 2e9;

  basalt::So3Spline<N> spline(dt_ns);
  spline.genRandomTrajectory(3 * N);

  for (int64_t t_ns = 0; t_ns < spline.maxTimeNs(); t_ns += 1e8) {
    Sophus::SO3d pos1 = spline.evaluate(t_ns);
    Eigen::Vector3d vel1 = spline.velocityBody(t_ns);
    Eigen::Vector3d accel1 = spline.accelerationBody(t_ns);
    Eigen::Vector3d jerk1 = spline.jerkBody(t_ns);

    Sophus::SO3d pos2;
    Eigen::Vector3d vel2, accel2, jerk2;

    {
      double pow_inv_dt = 1e9 / dt_ns;

      int64_t st_ns = (t_ns);

      BASALT_ASSERT_STREAM(st_ns >= 0, "st_ns " << st_ns << " time_ns " << t_ns
                                                << " start_t_ns " << 0);

      int64_t s = st_ns / dt_ns;
      double u = double(st_ns % dt_ns) / double(dt_ns);

      BASALT_ASSERT_STREAM(s >= 0, "s " << s);
      BASALT_ASSERT_STREAM(size_t(s + N) <= spline.getKnots().size(),
                           "s " << s << " N " << N << " knots.size() "
                                << spline.getKnots().size());

      std::vector<const double*> vec;
      for (int i = 0; i < N; i++) {
        vec.emplace_back(spline.getKnots()[s + i].data());
      }

      basalt::CeresSplineHelper<N>::template evaluate_lie<double, Sophus::SO3>(
          &vec[0], u, pow_inv_dt, &pos2);
      basalt::CeresSplineHelper<N>::template evaluate_lie<double, Sophus::SO3>(
          &vec[0], u, pow_inv_dt, nullptr, &vel2);
      basalt::CeresSplineHelper<N>::template evaluate_lie<double, Sophus::SO3>(
          &vec[0], u, pow_inv_dt, nullptr, nullptr, &accel2);
      basalt::CeresSplineHelper<N>::template evaluate_lie<double, Sophus::SO3>(
          &vec[0], u, pow_inv_dt, nullptr, nullptr, nullptr, &jerk2);
    }

    EXPECT_TRUE(pos1.matrix().isApprox(pos2.matrix()));
    EXPECT_TRUE(vel1.isApprox(vel2));
    EXPECT_TRUE(accel1.isApprox(accel2));
    EXPECT_TRUE(jerk1.isApprox(jerk2));
  }
}

template <int N>
void test_ceres_spline_helper_rd() {
  static const int DIM = 3;
  static const int64_t dt_ns = 2e9;

  basalt::RdSpline<DIM, N> spline(dt_ns);
  spline.genRandomTrajectory(3 * N);

  for (int64_t t_ns = 0; t_ns < spline.maxTimeNs(); t_ns += 1e8) {
    Eigen::Vector3d pos1 = spline.evaluate<0>(t_ns);
    Eigen::Vector3d vel1 = spline.evaluate<1>(t_ns);
    Eigen::Vector3d accel1 = spline.evaluate<2>(t_ns);

    Eigen::Vector3d pos2, vel2, accel2;

    {
      double pow_inv_dt = 1e9 / dt_ns;

      int64_t st_ns = (t_ns);

      BASALT_ASSERT_STREAM(st_ns >= 0, "st_ns " << st_ns << " time_ns " << t_ns
                                                << " start_t_ns " << 0);

      int64_t s = st_ns / dt_ns;
      double u = double(st_ns % dt_ns) / double(dt_ns);

      BASALT_ASSERT_STREAM(s >= 0, "s " << s);
      BASALT_ASSERT_STREAM(size_t(s + N) <= spline.getKnots().size(),
                           "s " << s << " N " << N << " knots.size() "
                                << spline.getKnots().size());

      std::vector<const double*> vec;
      for (int i = 0; i < N; i++) {
        vec.emplace_back(spline.getKnots()[s + i].data());
      }

      basalt::CeresSplineHelper<N>::template evaluate<double, DIM, 0>(
          &vec[0], u, pow_inv_dt, &pos2);
      basalt::CeresSplineHelper<N>::template evaluate<double, DIM, 1>(
          &vec[0], u, pow_inv_dt, &vel2);
      basalt::CeresSplineHelper<N>::template evaluate<double, DIM, 2>(
          &vec[0], u, pow_inv_dt, &accel2);
    }

    EXPECT_TRUE(pos1.isApprox(pos2));
    EXPECT_TRUE(vel1.isApprox(vel2));
    EXPECT_TRUE(accel1.isApprox(accel2));
  }
}

TEST(CeresSplineTestSuite, CeresSplineHelperSO3_4) {
  test_ceres_spline_helper_so3<4>();
}

TEST(CeresSplineTestSuite, CeresSplineHelperSO3_5) {
  test_ceres_spline_helper_so3<5>();
}

TEST(CeresSplineTestSuite, CeresSplineHelperSO3_6) {
  test_ceres_spline_helper_so3<6>();
}

TEST(CeresSplineTestSuite, CeresSplineHelperRd_4) {
  test_ceres_spline_helper_so3<4>();
}

TEST(CeresSplineTestSuite, CeresSplineHelperRd_5) {
  test_ceres_spline_helper_so3<5>();
}

TEST(CeresSplineTestSuite, CeresSplineHelperRd_6) {
  test_ceres_spline_helper_so3<6>();
}
