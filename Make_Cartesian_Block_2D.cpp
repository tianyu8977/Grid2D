
#include<vector>
#include<memory>

#include"Make_Cartesian_Block_2D.h"
#include"Curve2D/Line_Segment2D.h"
#include"Curve2D/Spline2D.h"

////////////////////////////////////////////////////
//     Function to make cartesian block 2D

knoblauch::Block_2D
knoblauch::make_cartesian_block_2D(double x_min, double x_max, unsigned int num_x,
                                   double y_min, double y_max, unsigned int num_y) {

  assert(x_max>x_min);
  assert(y_max>y_min);

  unsigned int number_of_nodes = num_x*num_y;
  std::vector<Vector2D> nodes(number_of_nodes);
  int current_node_number = 0;
  Vector2D current_node;
  std::vector<Vector2D> knot(9);

  ///////////////////////////////////////////////////////////////////
  //  Build boundaries
  Vector2D begin;
  Vector2D end;

  begin.x() = x_max;
  begin.y() = y_min;
  end.x()   = x_max;
  end.y()   = y_max;
  std::shared_ptr<Curve2D_Base> boundary_i_pos( new Line_Segment2D(begin,end) );

  begin.x() = x_min;
  begin.y() = y_min;
  end.x()   = x_min;
  end.y()   = y_max;
  std::shared_ptr<Curve2D_Base> boundary_i_neg( new Line_Segment2D(begin,end) );

  begin.x() = x_min;
  begin.y() = y_max;
  end.x()   = x_max;
  end.y()   = y_max;
  std::shared_ptr<Curve2D_Base> boundary_j_pos( new Line_Segment2D(begin,end) );

  begin.x() = x_min;
  begin.y() = y_min;
  end.x()   = x_max;
  end.y()   = y_min;
  std::shared_ptr<Curve2D_Base> boundary_j_neg( new Line_Segment2D(begin,end) );
  
  //  knot[0].x() = 1.0;
  // knot[0].y() = 1.0;
  // knot[1].x() = 1.15;
  // knot[1].y() = 0.9;
  // knot[2].x() = 1.25;
  // knot[2].y() = 1.0;
  // knot[3].x() = 1.30;
  // knot[3].y() = 1.1;
  // knot[4].x() = 1.5;
  // knot[4].y() = 1.0;
  // knot[5].x() = 1.60;
  // knot[5].y() = 0.9; 
  // knot[6].x() = 1.70;
  // knot[6].y() = 0.9;
  // knot[7].x() = 1.75;
  // knot[7].y() = 1.0;
  // knot[8].x() = 2.0;
  // knot[8].y() = 1.0;
  // std::shared_ptr<Curve2D_Base> boundary_j_neg( new Spline2D(knot) );

  ///////////////////////////////////////////////////////////////////
  //  Build nodes
  float_type delta_x = (x_max-x_min)/static_cast<float_type>(num_x-1);
  float_type delta_y = (y_max-y_min)/static_cast<float_type>(num_y-1);

  for(unsigned int j=0; j<num_y; ++j) {
    float_type y_position = y_min + static_cast<float_type>(j)*delta_y;
    current_node(1) = y_position;
    for(unsigned int i=0; i<num_x; ++i) {
      float_type x_position = x_min + static_cast<float_type>(i)*delta_x;
      current_node(0) = x_position;
      nodes[current_node_number] = current_node;
      ++current_node_number;
    } //for i
  } //for j

  return Block_2D (num_x, num_y, nodes, boundary_i_pos, boundary_i_neg, boundary_j_pos, boundary_j_neg);
}
