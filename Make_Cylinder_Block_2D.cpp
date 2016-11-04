#include<vector>
#include<cassert>
#include<cmath>
#include<memory>

#include"Make_Cylinder_Block_2D.h"
#include"../Math/Math_Constants.h"
#include"Curve2D/Circular_Arc2D.h"
#include"Curve2D/Line_Segment2D.h"

//  Function to make cylinder block 2D

knoblauch::Block_2D
knoblauch::make_cylinder_block_2D(float_type r_min, float_type r_max,
				  unsigned int num_r, unsigned int num_theta) {

  assert(num_r > 1);
  assert(num_theta > 2);
  assert(r_max > r_min);

  float_type pi = constants::PI;
  unsigned int number_of_nodes = num_r*num_theta;
  std::vector<Vector2D> nodes(number_of_nodes);
  int current_node_number = 0;
  Vector2D current_node;

  //////////////////////////////////////////////////////////////////
  //  Build boundaryies
  Vector2D origin_c;
  Vector2D begin_c;
  Vector2D begin_l;
  Vector2D end_l;
  float_type angle_c = pi;

  origin_c.x()  = 0.0;
  origin_c.y()  = 0.0;
  begin_c.x()   = r_max;
  begin_c.y()   = 0.0;
  std::shared_ptr<Curve2D_Base> boundary_j_pos( new Circular_Arc2D(origin_c,begin_c,angle_c) );

  origin_c.x()  = 0.0;
  origin_c.y()  = 0.0;
  begin_c.x()   = r_min;
  begin_c.y()   = 0.0;
  std::shared_ptr<Curve2D_Base> boundary_j_neg( new Circular_Arc2D(origin_c,begin_c,angle_c) );

  begin_l.x()   = r_min;
  begin_l.y()   = 0.0;
  end_l.x()     = r_max;
  end_l.y()     = 0.0;
  std::shared_ptr<Curve2D_Base> boundary_i_pos( new Line_Segment2D(begin_l, end_l) );

  begin_l.x()   = -r_max;
  begin_l.y()   = 0.0;
  end_l.x()     = -r_min;
  end_l.y()     = 0.0;
  std::shared_ptr<Curve2D_Base> boundary_i_neg( new Line_Segment2D(begin_l, end_l) );


  /////////////////////////////////////////////////////////////////////////
  //  Build nodes
  float_type delta_r = (r_max-r_min)/static_cast<float_type>(num_r-1);
  float_type delta_theta = pi/static_cast<float_type>(num_theta-1);
  
  for(unsigned int j=0; j<num_r; ++j) {
    float_type r = r_min + static_cast<float_type>(j)*delta_r;
    for(unsigned int i=0; i<num_theta; ++i) {
      float_type theta = pi - static_cast<float_type>(i)*delta_theta;
      float_type x = r*cos(theta);
      float_type y = r*sin(theta);
      current_node(0) = x;
      current_node(1) = y;
      nodes[current_node_number] = current_node;
      ++current_node_number;
    } //for i
  } //for j

  return Block_2D (num_theta, 
		   num_r, 
		   nodes, 
		   boundary_i_pos, 
		   boundary_i_neg, 
		   boundary_j_pos, 
		   boundary_j_neg);
}
