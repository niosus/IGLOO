// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "examples/3d_viewer/utils/point_cloud.h"

#include "gtest/gtest.h"

TEST(PointCloudTest, InitEmpty) {
  PointCloud cloud{};
  EXPECT_EQ(0u, cloud.points().size());
  EXPECT_EQ(0u, cloud.intensities().size());
}

TEST(PointCloudTest, InitKitti) {
  auto cloud_ptr =
      PointCloud::FromFile("examples/3d_viewer/utils/test_data/cloud.txt");
  ASSERT_NE(nullptr, cloud_ptr);
  EXPECT_EQ(121796u, cloud_ptr->points().size());
  EXPECT_EQ(121796u, cloud_ptr->intensities().size());
}
