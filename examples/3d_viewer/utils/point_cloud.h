// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.
#ifndef CODE_OPENGL_TUTORIALS_EXAMPLES_3D_VIEWER_UTILS_POINT_CLOUD_H_
#define CODE_OPENGL_TUTORIALS_EXAMPLES_3D_VIEWER_UTILS_POINT_CLOUD_H_

#include "glog/logging.h"
#include "utils/eigen_utils.h"

#include <memory>
#include <string>

class PointCloud {
 public:
  using PointVector = eigen::vector<Eigen::Vector3f>;
  using IntensityVector = std::vector<float>;

  PointCloud() {}
  explicit PointCloud(const PointVector& points) : points_{points} {}
  explicit PointCloud(const PointVector& points,
                      const IntensityVector& intensities)
      : points_{points}, intensities_{intensities} {
    CHECK_EQ(points.size(), intensities.size());
  }

  static std::unique_ptr<PointCloud> FromFile(const std::string& file_name);

  const auto& points() const noexcept { return points_; }
  const auto& intensities() const noexcept { return intensities_; }

 private:
  PointVector points_;
  IntensityVector intensities_;
};

#endif  // CODE_OPENGL_TUTORIALS_EXAMPLES_3D_VIEWER_UTILS_POINT_CLOUD_H_
