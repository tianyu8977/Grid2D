#include "gtest/gtest.h"

#include "Block_2D.h"
#include "Make_Cartesian_Block_2D.h"

//////////////////////////////////////////////////
//    Build test fixture in anonymous namespace
namespace {

const static int num_x = 100;
const static int num_y = 50;
const static knoblauch::float_type x_min = -10.0;
const static knoblauch::float_type x_max =  10.0;
const static knoblauch::float_type y_min =   5.0;
const static knoblauch::float_type y_max =  12.0;

class Cartesian_Block_2D_Test : public ::testing::Test {
  public:

  using float_type    = knoblauch::float_type;
  using Vector2D      = knoblauch::Vector2D;

  Cartesian_Block_2D_Test() :
    cartesian_block(knoblauch::make_cartesian_block_2D(x_min,x_max,num_x,
                                                       y_min,y_max,num_y) ) {}


  knoblauch::Block_2D cartesian_block;

}; //end test-fixture class

} //end anonymous namespace

//////////////////////////////////////////////////
//                Tests
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//    Ensure nodes are in the right place
TEST_F(Cartesian_Block_2D_Test,NodeLocations) {

  float_type delta_x = (x_max-x_min)/static_cast<float_type>(num_x-1);
  float_type delta_y = (y_max-y_min)/static_cast<float_type>(num_y-1);

  for(unsigned int i=0; i<num_x; ++i) {
    for(unsigned int j=0; j<num_y; ++ j) {
      Vector2D node_location = cartesian_block.get_node(i,j);
      float_type expected_x = x_min + static_cast<float_type>(i)*delta_x;
      float_type expected_y = y_min + static_cast<float_type>(j)*delta_y;
      ASSERT_DOUBLE_EQ(expected_x, node_location(0));
      ASSERT_DOUBLE_EQ(expected_y, node_location(1));
   }
  }

}

//////////////////////////////////////////////////
//    Ensure centroids are in the right place
TEST_F(Cartesian_Block_2D_Test,CentroidLocations) {

  float_type tolerance = 1.0e-10;

  float_type delta_x = (x_max-x_min)/static_cast<float_type>(num_x-1);
  float_type delta_y = (y_max-y_min)/static_cast<float_type>(num_y-1);

  for(unsigned int i=0; i<num_x-1; ++i) {
    for(unsigned int j=0; j<num_y-1; ++ j) {
      Vector2D centroid_location = cartesian_block.get_cell_centroid(i,j);
      float_type expected_x = x_min + (static_cast<float_type>(i)+0.5)*delta_x;
      float_type expected_y = y_min + (static_cast<float_type>(j)+0.5)*delta_y;
      ASSERT_NEAR(expected_x, centroid_location(0), tolerance);
      ASSERT_NEAR(expected_y, centroid_location(1), tolerance);
   }
  }

}

//////////////////////////////////////////////////
//    Ensure cell areas are right
TEST_F(Cartesian_Block_2D_Test,CellAreas) {

  float_type tolerance = 1.0e-10;

  float_type delta_x = (x_max-x_min)/static_cast<float_type>(num_x-1);
  float_type delta_y = (y_max-y_min)/static_cast<float_type>(num_y-1);

  float_type expected_area = delta_x*delta_y;

  for(unsigned int i=0; i<num_x-1; ++i) {
    for(unsigned int j=0; j<num_y-1; ++ j) {
      float_type area = cartesian_block.get_cell_area(i,j);
      ASSERT_NEAR(expected_area, area, tolerance);
   }
  }

}

//////////////////////////////////////////////////
//    Ensure cell unit normals are right
TEST_F(Cartesian_Block_2D_Test,CellUnitNormals) {

  float_type tolerance = 1.0e-10;

  for(unsigned int i=0; i<num_x-1; ++i) {
    for(unsigned int j=0; j<num_y-1; ++ j) {

      Vector2D expected_normal = knoblauch::unit_vector2D_x;
      Vector2D normal = cartesian_block.get_outward_unit_normal_positive_i(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);

      expected_normal = -knoblauch::unit_vector2D_x;
      normal = cartesian_block.get_outward_unit_normal_negative_i(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);

      expected_normal = knoblauch::unit_vector2D_y;
      normal = cartesian_block.get_outward_unit_normal_positive_j(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);

      expected_normal = -knoblauch::unit_vector2D_y;
      normal = cartesian_block.get_outward_unit_normal_negative_j(i,j);
      ASSERT_NEAR(expected_normal.x(), normal.x(), tolerance);
      ASSERT_NEAR(expected_normal.y(), normal.y(), tolerance);

   }
  }

}

//////////////////////////////////////////////////
//    Ensure edge legths are right
TEST_F(Cartesian_Block_2D_Test,CellEdgeLengths) {

  float_type tolerance = 1.0e-10;

  float_type delta_x = (x_max-x_min)/static_cast<float_type>(num_x-1);
  float_type delta_y = (y_max-y_min)/static_cast<float_type>(num_y-1);

  for(unsigned int i=0; i<num_x-1; ++i) {
    for(unsigned int j=0; j<num_y-1; ++ j) {

      float_type length = cartesian_block.get_edge_length_positive_i(i,j);
      ASSERT_NEAR(delta_y, length, tolerance);

      length = cartesian_block.get_edge_length_negative_i(i,j);
      ASSERT_NEAR(delta_y, length, tolerance);

      length = cartesian_block.get_edge_length_positive_j(i,j);
      ASSERT_NEAR(delta_x, length, tolerance);

      length = cartesian_block.get_edge_length_negative_j(i,j);
      ASSERT_NEAR(delta_x, length, tolerance);

    }
  }

}

//////////////////////////////////////////////////
//    Ensure edge centers are right
TEST_F(Cartesian_Block_2D_Test,CellEdgeCentre) {

  float_type tolerance = 1.0e-10;

  float_type delta_x = (x_max-x_min)/static_cast<float_type>(num_x-1);
  float_type delta_y = (y_max-y_min)/static_cast<float_type>(num_y-1);

  for(unsigned int i=0; i<num_x-1; ++i) {
    for(unsigned int j=0; j<num_y-1; ++ j) {

      Vector2D expected_centre;
      Vector2D actual_centre;

      //+ve i
      expected_centre.x() = x_min+static_cast<float_type>(i+1)*delta_x;
      expected_centre.y() = y_min+(static_cast<float_type>(j)+0.5)*delta_y;
      actual_centre = cartesian_block.get_edge_centre_positive_i(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);

      //-ve i
      expected_centre.x() = x_min+static_cast<float_type>(i)*delta_x;
      expected_centre.y() = y_min+(static_cast<float_type>(j)+0.5)*delta_y;
      actual_centre = cartesian_block.get_edge_centre_negative_i(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);

      //+ve j
      expected_centre.x() = x_min+(static_cast<float_type>(i)+0.5)*delta_x;
      expected_centre.y() = y_min+static_cast<float_type>(j+1)*delta_y;
      actual_centre = cartesian_block.get_edge_centre_positive_j(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);

      //-ve j
      expected_centre.x() = x_min+(static_cast<float_type>(i)+0.5)*delta_x;
      expected_centre.y() = y_min+static_cast<float_type>(j)*delta_y;
      actual_centre = cartesian_block.get_edge_centre_negative_j(i,j);
      ASSERT_NEAR(expected_centre.x(), actual_centre.x(), tolerance);
      ASSERT_NEAR(expected_centre.y(), actual_centre.y(), tolerance);
    }
  }

}

//////////////////////////////////////////////////
//    Get Number of Cells
TEST_F(Cartesian_Block_2D_Test,NumberOfCells) {
  int expected = (num_x-1)*(num_y-1);
  EXPECT_EQ(expected, cartesian_block.get_number_of_cells());
}

//////////////////////////////////////////////////
//    Get Number of Cells i direction
TEST_F(Cartesian_Block_2D_Test,NumberOfCellsI) {
  int expected = (num_x-1);
  EXPECT_EQ(expected, cartesian_block.get_number_of_cells_i());
}

//////////////////////////////////////////////////
//    Get Number of Cells j direction
TEST_F(Cartesian_Block_2D_Test,NumberOfCellsJ) {
  int expected = (num_y-1);
  EXPECT_EQ(expected, cartesian_block.get_number_of_cells_j());
}

//////////////////////////////////////////////////
//    Get Number of Nodes
TEST_F(Cartesian_Block_2D_Test,NumberOfNodes) {
  int expected = num_x*num_y;
  EXPECT_EQ(expected, cartesian_block.get_number_of_nodes());
}

//////////////////////////////////////////////////
//    Get Number of Nodes i direction
TEST_F(Cartesian_Block_2D_Test,NumberOfNodesI) {
  int expected = num_x;
  EXPECT_EQ(expected, cartesian_block.get_number_of_nodes_i());
}

//////////////////////////////////////////////////
//    Get Number of Nodes j direction
TEST_F(Cartesian_Block_2D_Test,NumberOfNodesJ) {
  int expected = num_y;
  EXPECT_EQ(expected, cartesian_block.get_number_of_nodes_j());
}
