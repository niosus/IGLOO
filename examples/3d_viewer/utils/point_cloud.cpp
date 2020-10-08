// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "examples/3d_viewer/utils/point_cloud.h"
#include "absl/strings/str_split.h"

#include <filesystem>
#include <fstream>

std::unique_ptr<PointCloud> PointCloud::FromFile(const std::string& file_name) {
  std::filesystem::path path(file_name);
  if (!std::filesystem::exists(path)) {
    LOG(ERROR) << "Path " << path << " does not exist.";
    return nullptr;
  }
  PointVector points;
  IntensityVector intensities;
  // Open file and read its contents.
  if (path.extension() == ".txt") {
    DLOG(INFO) << "Reading text cloud from " << path;
    std::locale::global(std::locale("en_US.UTF-8"));
    std::ifstream file{path.string()};
    for (std::string line; std::getline(file, line, '\n');) {
      const std::vector<std::string> split{absl::StrSplit(line, " ")};
      if (split.size() != 4u) {
        LOG(WARNING) << "Format of cloud line is wrong. Skipping.";
        continue;
      }
      points.emplace_back(
          std::stof(split[0]), std::stof(split[1]), std::stof(split[2]));
      intensities.emplace_back(std::stof(split[3]));
    }
    CHECK_GT(points.size(), 0u);
    return std::make_unique<PointCloud>(points, intensities);
  }
  if (path.extension() == ".bin") {
    DLOG(INFO) << "Reading binary cloud from " << path;
    std::fstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (file.good()) {
      file.seekg(0, std::ios::beg);
      for (int i = 0; file.good() && !file.eof(); ++i) {
        Eigen::Vector3f point;
        float intensity = 0;
        file.read(reinterpret_cast<char*>(&point.x()), sizeof(float));
        file.read(reinterpret_cast<char*>(&point.y()), sizeof(float));
        file.read(reinterpret_cast<char*>(&point.z()), sizeof(float));
        points.push_back(point);
        file.read(reinterpret_cast<char*>(&intensity), sizeof(float));
        intensities.push_back(intensity);
      }
      file.close();
    }
    CHECK_GT(points.size(), 0u);
    CHECK_EQ(points.size(), intensities.size());
    return std::make_unique<PointCloud>(points, intensities);
  }
  LOG(FATAL) << "Unknown extention for file: " << path;
  return nullptr;
}
