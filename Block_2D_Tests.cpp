#include<vector>

#include "gtest/gtest.h"

#include "Block_2D.h"

//////////////////////////////////////////////////
////  test L-value version of import_nodes
//TEST(Block_2D,ImportNodesLValue) {
//
//  using Vector2D = knoblauch::Vector2D;
//
//  unsigned int number_of_nodes_x = 3;
//  unsigned int number_of_nodes_y = 2;
//  unsigned int number_of_nodes   = number_of_nodes_x*number_of_nodes_y;
//
//  std::vector<Vector2D> nodes(number_of_nodes);
//
//  nodes[0].x() = -1.0;
//  nodes[0].y() =  1.0;
//  nodes[1].x() = -0.1;
//  nodes[1].y() =  1.2;
//  nodes[2].x() =  0.5;
//  nodes[2].y() =  1.3;
//
//  nodes[3].x() = -1.1;
//  nodes[3].y() =  3.0;
//  nodes[4].x() =  0.0;
//  nodes[4].y() =  4.2;
//  nodes[5].x() =  0.7;
//  nodes[5].y() =  3.3;
//
//  knoblauch::Block_2D block(number_of_nodes_x,number_of_nodes_y,nodes);
//
//  int node_index(0);
//  for(unsigned int j=0; j<number_of_nodes_y; ++j) {
//    for(unsigned int i=0; i<number_of_nodes_x; ++i) {
//      EXPECT_DOUBLE_EQ(nodes[node_index].x(), block.get_node(i,j).x() );
//      EXPECT_DOUBLE_EQ(nodes[node_index].y(), block.get_node(i,j).y() );
//      ++node_index;
//    }
//  }
//
//}
//
//////////////////////////////////////////////////
////  test R-value version of import_nodes
//TEST(Block_2D,ImportNodesRValue) {
//
//  using Vector2D = knoblauch::Vector2D;
//
//  unsigned int number_of_nodes_x = 2;
//  unsigned int number_of_nodes_y = 2;
//  unsigned int number_of_nodes   = number_of_nodes_x*number_of_nodes_y;
//
//  std::vector<Vector2D> nodes(number_of_nodes);
//
//  nodes[0].x() = -1.1;
//  nodes[0].y() = -1.0;
//  nodes[1].x() =  0.9;
//  nodes[1].y() = -0.9;
//
//  nodes[2].x() = -0.5;
//  nodes[2].y() =  1.3;
//  nodes[3].x() =  1.1;
//  nodes[3].y() =  0.5;
//
//  knoblauch::Block_2D block(number_of_nodes_x,number_of_nodes_y);
//
//  std::vector<Vector2D> nodes_copy = nodes;
//
//  block.import_nodes(std::move(nodes_copy));
//
//  int node_index(0);
//  for(unsigned int j=0; j<number_of_nodes_y; ++j) {
//    for(unsigned int i=0; i<number_of_nodes_x; ++i) {
//      EXPECT_DOUBLE_EQ(nodes[node_index].x(), block.get_node(i,j).x() );
//      EXPECT_DOUBLE_EQ(nodes[node_index].y(), block.get_node(i,j).y() );
//      ++node_index;
//    }
//  }
//
//  EXPECT_EQ(0,nodes_copy.size());  //did it really move?
//
//}
//
