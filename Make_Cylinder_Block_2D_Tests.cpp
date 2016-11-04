#include "gtest/gtest.h"

#include "Block_2D.h"
#include "Make_Cylinder_Block_2D.h"

//////////////////////////////////////////////////
//    Build test fixture in anonymous namespace
namespace {

const static int num_r = 51;
const static int num_theta = 101;
const static knoblauch::float_type r_min = 2.0;
const static knoblauch::float_type r_max = 30.0;


class Make_Cylinder_Block_2D_Test : public ::testing::Test {
  public:

  using float_type    = knoblauch::float_type;
  using Vector2D      = knoblauch::Vector2D;

  Make_Cylinder_Block_2D_Test() :
    cylinder_block(knoblauch::make_cylinder_block_2D(r_min,r_max,num_r,num_theta) ) {}


  knoblauch::Block_2D cylinder_block;

}; //end test-fixture class

} //end anonymous namespace

//////////////////////////////////////////////////
//                Tests
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//    Ensure nodes are in the right place
TEST_F(Make_Cylinder_Block_2D_Test,NodeLocations) {

  float_type pi = knoblauch::constants::PI;
  float_type delta_r = (r_max-r_min)/static_cast<float_type>(num_r-1);
  float_type delta_theta = pi/static_cast<float_type>(num_theta-1);

  for(unsigned int j=0; j<num_r; ++j) {
    for(unsigned int i=0; i<num_theta; ++i) {
      Vector2D node_location = cylinder_block.get_node(i,j);
      float_type expected_r = r_min + static_cast<float_type>(j)*delta_r;
      float_type expected_theta = pi - (static_cast<float_type>(i))*delta_theta;
      float_type expected_x = expected_r*cos(expected_theta);
      float_type expected_y = expected_r*sin(expected_theta);
      ASSERT_DOUBLE_EQ(expected_x, node_location(0));
      ASSERT_DOUBLE_EQ(expected_y, node_location(1));
   }
  }

}

//////////////////////////////////////////////////
//    Ensure centroids are in the right place
TEST_F(Make_Cylinder_Block_2D_Test,CentroidLocations) {

  float_type tolerance = 1.0e-10;

  float_type pi = knoblauch::constants::PI;
  float_type delta_r = (r_max-r_min)/static_cast<float_type>(num_r-1);
  float_type delta_theta = pi/static_cast<float_type>(num_theta-1);
  float_type h = delta_r*cos(delta_theta/2);

  for(unsigned int j=0; j<num_r-1; ++j) {
    for(unsigned int i=0; i<num_theta-1; ++i) {
      Vector2D centroid_location = cylinder_block.get_cell_centroid(i,j);
      float_type r = r_min + (static_cast<float_type>(j)+1.0)*delta_r;
      float_type theta = pi - static_cast<float_type>(i)*delta_theta;
      float_type boundary_positive = 2.0*r*sin(delta_theta/2.0);
      float_type boundary_negative = 2.0*(r-delta_r)*sin(delta_theta/2.0);
      float_type h_centroid = h*(2.0*boundary_negative+boundary_positive)/(3.0*boundary_negative+3.0*boundary_positive);
      float_type x_0 = r*cos(theta);
      float_type x_1 = r*cos(theta-delta_theta);
      float_type x_m = (x_0+x_1)/2.0;
      float_type y_0 = r*sin(theta);
      float_type y_1 = r*sin(theta-delta_theta);
      float_type y_m = (y_0+y_1)/2.0;
      float_type expected_x = x_m-h_centroid*cos(theta-delta_theta/2.0);
      float_type expected_y = y_m-h_centroid*sin(theta-delta_theta/2.0);
      ASSERT_NEAR(expected_x, centroid_location(0), tolerance);
      ASSERT_NEAR(expected_y, centroid_location(1), tolerance);
   }
  }

}

//////////////////////////////////////////////////
//    Ensure cell areas are right
TEST_F(Make_Cylinder_Block_2D_Test,CellAreas) {

  float_type tolerance = 1.0e-10;

  float_type pi = knoblauch::constants::PI;
  float_type delta_r = (r_max-r_min)/static_cast<float_type>(num_r-1);
  float_type delta_theta = pi/static_cast<float_type>(num_theta-1);
  float_type h = delta_r*cos(delta_theta/2);

  for(unsigned int j=0; j<num_r-1; ++j) {
    for(unsigned int i=0; i<num_theta-1; ++i) {
      float_type r = r_min + static_cast<float_type>(j)*delta_r;
      float_type delta_arc_positive = 2*(r+delta_r)*sin(delta_theta/2);
      float_type delta_arc_negative = 2*r*sin(delta_theta/2);
      float_type expected_area = (delta_arc_positive+delta_arc_negative)*h/2;

      float_type area = cylinder_block.get_cell_area(i,j);
      ASSERT_NEAR(expected_area, area, tolerance);
   }
  }

}

//////////////////////////////////////////////////
//    Ensure cell unit normals are right
TEST_F(Make_Cylinder_Block_2D_Test,CellUnitNormals) {

  float_type tolerance = 1.0e-10;
  float_type pi = knoblauch::constants::PI;
  float_type delta_theta = pi/static_cast<float_type>(num_theta-1);

  for(unsigned int j=0; j<num_r-1; ++j) {
    for(unsigned int i=0; i<num_theta-1; ++i) {

      float_type expected_theta_0 = pi - (static_cast<float_type>(i)    )*delta_theta;
      float_type expected_theta_1 = pi - (static_cast<float_type>(i)+0.5)*delta_theta;
      float_type expected_theta_2 = pi - (static_cast<float_type>(i)+ 1 )*delta_theta;

      Vector2D expected_normal;
      Vector2D normal;

      expected_normal(0) =  sin(expected_theta_2);
      expected_normal(1) = -cos(expected_theta_2);
      normal = cylinder_block.get_outward_unit_normal_positive_i(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);


      expected_normal(0) = -sin(expected_theta_0);
      expected_normal(1) =  cos(expected_theta_0);
      normal = cylinder_block.get_outward_unit_normal_negative_i(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);

      expected_normal(0) = cos(expected_theta_1);
      expected_normal(1) = sin(expected_theta_1);
      normal = cylinder_block.get_outward_unit_normal_positive_j(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);

      expected_normal *= -1.0;
      normal = cylinder_block.get_outward_unit_normal_negative_j(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);

   }
  }

}

//////////////////////////////////////////////////
//    Ensure edge legths are right
TEST_F(Make_Cylinder_Block_2D_Test,CellEdgeLengths) {

  float_type tolerance = 1.0e-10;
  float_type pi = knoblauch::constants::PI;
  float_type delta_r = (r_max-r_min)/static_cast<float_type>(num_r-1);
  float_type delta_theta = pi/static_cast<float_type>(num_theta-1);

  for(unsigned int j=0; j<num_r-1; ++j) {
    for(unsigned int i=0; i<num_theta-1; ++i) {
      float_type r = r_min + static_cast<float_type>(j)*delta_r;
      float_type delta_arc_positive = 2.0*(r+delta_r)*sin(delta_theta/2.0);
      float_type delta_arc_negative = 2.0*r*sin(delta_theta/2.0);

      float_type length = cylinder_block.get_edge_length_positive_i(i,j);
      ASSERT_NEAR(delta_r, length, tolerance);

      length = cylinder_block.get_edge_length_negative_i(i,j);
      ASSERT_NEAR(delta_r, length, tolerance);

      length = cylinder_block.get_edge_length_positive_j(i,j);
      ASSERT_NEAR(delta_arc_positive, length, tolerance);

      length = cylinder_block.get_edge_length_negative_j(i,j);
      ASSERT_NEAR(delta_arc_negative, length, tolerance);

    }
  }

}

//////////////////////////////////////////////////
//    Ensure edge centers are right
TEST_F(Make_Cylinder_Block_2D_Test,CellEdgeCentre) {

  float_type tolerance = 1.0e-10;
  float_type pi = knoblauch::constants::PI;
  float_type delta_r = (r_max-r_min)/static_cast<float_type>(num_r-1);
  float_type delta_theta = pi/static_cast<float_type>(num_theta-1);

  for(unsigned int j=0; j<num_r-1; ++j) {
    for(unsigned int i=0; i<num_theta-1; ++i) {

      Vector2D expected_centre;
      Vector2D actual_centre;

      //+ve i
      float_type r = r_min + (static_cast<float_type>(j)+0.5)*delta_r;
      float_type theta = pi - (static_cast<float_type>(i)+1.0)*delta_theta;
      expected_centre.x() = r*cos(theta);
      expected_centre.y() = r*sin(theta);
      actual_centre = cylinder_block.get_edge_centre_positive_i(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);

      //-ve i
      r = r_min + (static_cast<float_type>(j)+0.5)*delta_r;
      theta = pi - (static_cast<float_type>(i))*delta_theta;
      expected_centre.x() = r*cos(theta);
      expected_centre.y() = r*sin(theta);
      actual_centre = cylinder_block.get_edge_centre_negative_i(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);

      //+ve j
      r = r_min + (static_cast<float_type>(j)+1.0)*delta_r;
      float_type x_0 = r*cos(theta);
      float_type x_1 = r*cos(theta-delta_theta);
      expected_centre.x() = (x_0+x_1)/2;
      float_type y_0 = r*sin(theta);
      float_type y_1 = r*sin(theta-delta_theta);
      expected_centre.y() = (y_0+y_1)/2;
      actual_centre = cylinder_block.get_edge_centre_positive_j(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);

      //-ve j
      r = r_min + static_cast<float_type>(j)*delta_r;
      x_0 = r*cos(theta);
      x_1 = r*cos(theta-delta_theta);
      expected_centre.x() = (x_0+x_1)/2;
      y_0 = r*sin(theta);
      y_1 = r*sin(theta-delta_theta);
      expected_centre.y() = (y_0+y_1)/2;
      actual_centre = cylinder_block.get_edge_centre_negative_j(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);
    }
  }

}

//////////////////////////////////////////////////
//    Get Number of Cells
TEST_F(Make_Cylinder_Block_2D_Test,NumberOfCells) {
  int expected = (num_r-1)*(num_theta-1);
  ASSERT_EQ(expected, cylinder_block.get_number_of_cells());
}

//////////////////////////////////////////////////
//    Get Number of Cells i direction
TEST_F(Make_Cylinder_Block_2D_Test,NumberOfCellsI) {
  int expected = (num_theta-1);
  ASSERT_EQ(expected, cylinder_block.get_number_of_cells_i());
}

//////////////////////////////////////////////////
//    Get Number of Cells j direction
TEST_F(Make_Cylinder_Block_2D_Test,NumberOfCellsJ) {
  int expected = (num_r-1);
  ASSERT_EQ(expected, cylinder_block.get_number_of_cells_j());
}

//////////////////////////////////////////////////
//    Get Number of Nodes
TEST_F(Make_Cylinder_Block_2D_Test,NumberOfNodes) {
  int expected = num_r*num_theta;
  ASSERT_EQ(expected, cylinder_block.get_number_of_nodes());
}

//////////////////////////////////////////////////
//    Get Number of Nodes i direction
TEST_F(Make_Cylinder_Block_2D_Test,NumberOfNodesI) {
  int expected = num_theta;
  ASSERT_EQ(expected, cylinder_block.get_number_of_nodes_i());
}

//////////////////////////////////////////////////
//    Get Number of Nodes j direction
TEST_F(Make_Cylinder_Block_2D_Test,NumberOfNodesJ) {
  int expected = num_r;
  ASSERT_EQ(expected, cylinder_block.get_number_of_nodes_j());
}
