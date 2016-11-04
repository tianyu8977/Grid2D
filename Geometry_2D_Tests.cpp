#include<vector>

#include "gtest/gtest.h"

#include "Geometry_2D.h"

namespace {

  std::vector<knoblauch::Area_and_Centroid_2D>
  compute_area_and_centroid_quad_8permutations(const knoblauch::Vector2D &node0,
                                               const knoblauch::Vector2D &node1,
                                               const knoblauch::Vector2D &node2,
                                               const knoblauch::Vector2D &node3) {

    std::vector<knoblauch::Area_and_Centroid_2D> result(8);

    //four starting nodes, one way around
    result[0] = knoblauch::compute_area_and_centroid(node0,node1,node2,node3);
    result[1] = knoblauch::compute_area_and_centroid(node1,node2,node3,node0);
    result[2] = knoblauch::compute_area_and_centroid(node2,node3,node0,node1);
    result[3] = knoblauch::compute_area_and_centroid(node3,node0,node1,node2);

    //other way around
    result[4] = knoblauch::compute_area_and_centroid(node3,node2,node1,node0);
    result[5] = knoblauch::compute_area_and_centroid(node2,node1,node0,node3);
    result[6] = knoblauch::compute_area_and_centroid(node1,node0,node3,node2);
    result[7] = knoblauch::compute_area_and_centroid(node0,node3,node2,node1);

    return result;
  }

}//end anonymous namespace


////////////////////////////////////////////////
//  Test area and centroid of unit square
TEST(Geometry_2D,AreaCentroidUnitSquare) {

  using float_type = knoblauch::float_type;
  using Vector2D = knoblauch::Vector2D;

  Vector2D node0;
  node0.x() = 0.0;
  node0.y() = 0.0;

  Vector2D node1;
  node1.x() = 1.0;
  node1.y() = 0.0;

  Vector2D node2;
  node2.x() = 1.0;
  node2.y() = 1.0;

  Vector2D node3;
  node3.x() = 0.0;
  node3.y() = 1.0;

  float_type expected_area = 1.0;
  Vector2D expected_centroid;

  expected_centroid.x() = 0.5;
  expected_centroid.y() = 0.5;

  std::vector<knoblauch::Area_and_Centroid_2D> areas_and_centroids
    = compute_area_and_centroid_quad_8permutations (node0,node1,node2,node3);

  for(auto a_and_c : areas_and_centroids) {
    ASSERT_DOUBLE_EQ(expected_area, a_and_c.area);
    ASSERT_DOUBLE_EQ(expected_centroid.x(), a_and_c.centroid.x());
    ASSERT_DOUBLE_EQ(expected_centroid.y(), a_and_c.centroid.y());
  }


}

////////////////////////////////////////////////
//  Test area and centroid of quad with nodes
//  node0 = (0.1,0.7)
//  node1 = (-0.1,-0.1)
//  node2 = (1.0,-0.7)
//  node1 = (-0.2,-0.2)
TEST(Geometry_2D,AreaCentroidTestQuad1) {

  using float_type = knoblauch::float_type;
  using Vector2D = knoblauch::Vector2D;

  Vector2D node0;
  node0.x() =  0.1;
  node0.y() =  0.7;

  Vector2D node1;
  node1.x() = -0.1;
  node1.y() = -0.1;

  Vector2D node2;
  node2.x() =  1.0;
  node2.y() = -0.7;

  Vector2D node3;
  node3.x() = -0.2;
  node3.y() = -0.2;

  node1 = node0;

  float_type area_triangle1 = 0.5*( (node2.x()-node3.x())*(node0.y()-node3.y())
                               -(node0.x()-node3.x())*(node2.y()-node3.y()));

  Vector2D centroid_triangle1 = (node0+node2+node3)/3.0;

  float_type area_triangle2 = 0.5*( (node2.x()-node1.x())*(node0.y()-node1.y())
                               -(node0.x()-node1.x())*(node2.y()-node1.y()));

  Vector2D centroid_triangle2 = (node0+node1+node2)/3.0;

  float_type expected_area = area_triangle1-area_triangle2;
  Vector2D expected_centroid =  (area_triangle1*centroid_triangle1
                                 - area_triangle2*centroid_triangle2) / expected_area;

  float_type tolerance = expected_area*1.0e-10;

  std::vector<knoblauch::Area_and_Centroid_2D> areas_and_centroids
    = compute_area_and_centroid_quad_8permutations (node0,node1,node2,node3);

  for(auto a_and_c : areas_and_centroids) {
    ASSERT_NEAR(expected_area, a_and_c.area, tolerance);
    ASSERT_NEAR(expected_centroid.x(), a_and_c.centroid.x(), tolerance);
    ASSERT_NEAR(expected_centroid.y(), a_and_c.centroid.y(), tolerance);
  }


}
