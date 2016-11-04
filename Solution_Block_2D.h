#ifndef SOLUTION_BLOCK2D_H
#define SOLUTION_BLOCK2D_H

#include<cassert>
#include<vector>
#include<cmath>
#include"../Math/Math_Constants.h"
#include"Block_2D.h"
#include"Grid_Tree/Refine_Block2D_I.h"
#include"Grid_Tree/Refine_Block2D_J.h"
#include"Grid_Tree/Refine_Block2D_IJ.h"
#include"Refinement_Flag.h"
#include"Geometry_2D.h"


namespace knoblauch {

  struct error_norms {
    float_type L1 = 0.0;
    float_type L2 = 0.0;
    float_type L_inf = 0.0;
    int num_cells = 0;
  };
  
  class Solution_Block_2D : public Block_2D {

  protected:
    std::vector<float_type> variable;

  public:

  Solution_Block_2D(const Solution_Block_2D& block) :
    Block_2D(block),
    variable(get_number_of_cells()) {}

  Solution_Block_2D(const Block_2D& block) :
    Block_2D(block),
    variable(get_number_of_cells()) {}

  Solution_Block_2D(Solution_Block_2D&& block) :
    Block_2D(block),
    variable(get_number_of_cells()) {}

  Solution_Block_2D(Block_2D&& block) :
    Block_2D(block),
    variable(get_number_of_cells()) {}

  Solution_Block_2D(unsigned int num_nodes_i,
		    unsigned int num_nodes_j,
		    const std::vector<Vector2D>& nodes_in,
		    const std::shared_ptr<Curve2D_Base>& boundary_i_pos_in,
		    const std::shared_ptr<Curve2D_Base>& boundary_i_neg_in,
		    const std::shared_ptr<Curve2D_Base>& boundary_j_pos_in,
		    const std::shared_ptr<Curve2D_Base>& boundary_j_neg_in) :
    Block_2D(num_nodes_i,
	     num_nodes_j,
	     nodes_in,
	     boundary_i_pos_in,
	     boundary_i_neg_in,
	     boundary_j_pos_in,
	     boundary_j_neg_in),
    variable(get_number_of_cells()) {}      
    
    Solution_Block_2D& operator=(const Solution_Block_2D& block) {
      Block_2D::operator=(block);
      variable.resize(get_number_of_cells());
      return *this;
    }

    Solution_Block_2D& operator=(Solution_Block_2D&& block) {
      Block_2D::operator=(std::move(block));
      variable.resize(get_number_of_cells());
      return *this;
    }

    Solution_Block_2D& operator=(const Block_2D& block) {
      Block_2D::operator=(block);
      variable.resize(get_number_of_cells());
      return *this;
    }

    Solution_Block_2D& operator=(Block_2D&& block) {
      Block_2D::operator=(std::move(block));
      variable.resize(get_number_of_cells());
      return *this;
    }

      float_type pi = knoblauch::constants::PI;
      float_type Helper_Abgrall_2D_Function(const float_type r) {
	if (r<=-0.1e1/0.3e1) {
	  return (-r*std::sin(0.3e1/0.2e1*pi*r*r));
	} 
	else if (std::fabs(r) < 0.1e1 / 0.3e1) {
	  return (std::fabs(std::sin(0.2e1*pi*r)));
	} 
	else if (0.1e1/0.3e1<=r) {
	  return (0.2e1*r-0.1e1+std::sin(0.3e1*pi*r)/0.6e1);
	} 
	else {
	  return 0.0e0;
	}
      }
     
      float_type Abgrall_2D_Function(const float_type x, const float_type y) {
	return sin(5.0*x);
	if (x <= std::cos(pi*y)/0.2e1) {
	  return (2.0+Helper_Abgrall_2D_Function(x-1.0/std::tan(std::sqrt(pi/0.2e1))*y));
	} 
	else if (std::cos(pi*y)/0.2e1<x) {
	  return (2.0+Helper_Abgrall_2D_Function(x+1.0/std::tan(std::sqrt(pi/0.2e1))*y)+std::cos(0.2e1*pi*y));
	} 
	else {
	  return 0.0e0;
	}
      } 

      void give_value() {
	std::vector<Vector2D> position(get_number_of_cells());
	std::vector<float_type> r(get_number_of_cells());
	std::vector<float_type> judge(get_number_of_cells());
	int number_of_variable = get_number_of_cells();
	int num_nodes_i = get_number_of_nodes_i();
	int num_nodes_j = get_number_of_nodes_j();
	int n = 0;
	for(int j=0; j<num_nodes_j-1; j++) {
	  for(int i=0; i<num_nodes_i-1; i++) {
     	    position[n] = get_cell_centroid(i,j);
	    n++;
	  }
	}

	for(int i=0; i<number_of_variable; i++) {
	  variable[i] = Abgrall_2D_Function(position[i].x(),position[i].y());
	}
      }
      
      std::vector<float_type>  get_variable() const {
	return variable;
      }

      void give_value_test() {
	std::vector<Vector2D> position(get_number_of_cells());
	int number_of_cells = get_number_of_cells();
	int num_nodes_i = get_number_of_nodes_i();
	int num_nodes_j = get_number_of_nodes_j();
	int n = 0;
	for(int j=0; j<num_nodes_j-1; j++) {
	  for(int i=0; i<num_nodes_i-1; i++) {
     	    position[n] = get_cell_centroid(i,j);
	    n++;
	  }
	}

	for(int i=0; i<number_of_cells; i++) {
	  if(position[i].x() <= -0.3) {
	    variable[i] = 1.0;
	}  else if(position[i].x() > -0.3 && sqrt(position[i].x()+0.3) <= position[i].y()) {
	  variable[i] = 1.0;
	}
	else {
	  variable[i] = sin(2*position[i].x())*cos(2*position[i].y());
	    }
      }
  }
      
      Refinement_Flag decide_refine_test() {

	float_type criteria = 0.002;
	int num_cells_i = get_number_of_cells_i();
	int num_cells_j = get_number_of_cells_j();
	float_type dif_max_i = 0.0;
	float_type dif_max_j = 0.0;
	int num_of_dif_i = (num_cells_i-1)*num_cells_j;
	int num_of_dif_j = (num_cells_i)*(num_cells_j-1);
	int n_i = 0;
	int n_j = 0;
	int current_i = 0;
	int current_j = 0;
	int number_i = 0;
	int number_j = 0;
	std::vector<float_type> judge_i(num_of_dif_i);
	std::vector<float_type> judge_j(num_of_dif_j);

	for(int j=0; j<num_cells_j; j++) {
	  for(int i=0; i<num_cells_i-1; i++) {
	    current_i = (num_cells_i*j)+i;
	    judge_i[n_i]= fabs(variable[current_i+1]-variable[current_i]);
	    n_i++;
	  }
	}

	for(int k=0; k<num_of_dif_i-1; k++) {
	  if(judge_i[number_i]<judge_i[k+1]) {
	      number_i = k+1; 
	  }
	}
	  dif_max_i = judge_i[number_i];

	for(int j=0; j<num_cells_j-1; j++) {
	  for(int i=0; i<num_cells_i; i++) {
	    current_j = (num_cells_i*j)+i;
	    judge_j[n_j]= fabs(variable[current_j+num_cells_i]-variable[current_j]);
	    n_j++;
	  }
	}

	for(int k=0; k<num_of_dif_j-1; k++) {
	  if(judge_j[number_j]<judge_j[k+1]) {
	      number_j = k+1; 
	  }
	}
	  dif_max_j = judge_j[number_j];

	  if(dif_max_i>criteria && dif_max_j<criteria) {
	    return Refinement_Flag::refine_i;
	  }
	  else if(dif_max_i<criteria && dif_max_j>criteria) {
	    return Refinement_Flag::refine_j;
	  }	 
	  else if(dif_max_i>criteria && dif_max_j>criteria) {
	    return Refinement_Flag::refine_ij;
	  }
	  else {
	    return Refinement_Flag::none;
	  }
      }
   
      Refinement_Flag decide_refine() {
	float_type criteria =1.0E-6;
	int num_cells_i = get_number_of_cells_i();
	int num_cells_j = get_number_of_cells_j();
	float_type dif_max_i = 0.0;
	float_type dif_max_j = 0.0;
	int num_of_dif_i = (num_cells_i-1)*num_cells_j;
	int num_of_dif_j = (num_cells_i)*(num_cells_j-1);
	int n_i = 0;
	int n_j = 0;
	int current_i = 0;
	int current_j = 0;
	int number_i = 0;
	int number_j = 0;
	std::vector<float_type> judge_i(num_of_dif_i);
	std::vector<float_type> judge_j(num_of_dif_j);

	for(int j=0; j<num_cells_j; j++) {
	  for(int i=0; i<num_cells_i-1; i++) {
	    current_i = (num_cells_i*j)+i;
	    judge_i[n_i]= fabs(variable[current_i+1]-variable[current_i])*(get_cell_area(i,j)+get_cell_area(i+1,j))/2.0;
	    n_i++;
	  }
	}

	for(int k=0; k<num_of_dif_i-1; k++) {
	  if(judge_i[number_i]<judge_i[k+1]) {
	      number_i = k+1; 
	  }
	}
	  dif_max_i = judge_i[number_i];

	for(int j=0; j<num_cells_j-1; j++) {
	  for(int i=0; i<num_cells_i; i++) {
	    current_j = (num_cells_i*j)+i;
	    judge_j[n_j]= fabs(variable[current_j+num_cells_i]-variable[current_j])*(get_cell_area(i,j)+get_cell_area(i,j+1))/2.0;
	    n_j++;
	  }
	}

	for(int k=0; k<num_of_dif_j-1; k++) {
	  if(judge_j[number_j]<judge_j[k+1]) {
	      number_j = k+1; 
	  }
	}
	  dif_max_j = judge_j[number_j];

	  if(dif_max_i>criteria && dif_max_j<criteria) {
	    return Refinement_Flag::refine_i;
	  }
	  else if(dif_max_i<criteria && dif_max_j>criteria) {
	    return Refinement_Flag::refine_j;
	  }	 
	  else if(dif_max_i>criteria && dif_max_j>criteria) {
	    return Refinement_Flag::refine_ij;
	  }
	  else {
	    return Refinement_Flag::none;
	  }
      }
      
      float_type compare(float_type v0, float_type v1, float_type v2, float_type v3, float_type v4) {
	float_type n = 0.0;
	if(v0<v1) {
	  n = v1;
	} else {
	  n = v0;
	}
	if(n<v2) {
	  n = v2;
	}
	if(n<v3) {
	  n = v3;
	}
	if(n<v4) {
	  n = v4;
	}
	return n;
      }

      error_norms compute_errors() {
	
	error_norms L1_L2_Linf;

	float_type value_const = 0.0;
	std::vector<Vector2D> node(9);
	std::vector<Vector2D> centroid(4);
	std::vector<float_type> ei(4);
	std::vector<float_type> area(4);
	std::vector<float_type> value(4);
	L1_L2_Linf.num_cells = get_number_of_cells();
	int num_of_nodes_i = get_number_of_nodes_i();
	int num_of_nodes_j = get_number_of_nodes_j();

	for(int j=0; j<num_of_nodes_j-1; j++) {
	  for(int i=0; i<num_of_nodes_i-1; i++) {
	    node[0] = get_node(i,j);
	    node[1] = get_edge_centre_negative_j(i,j);
	    node[2] = get_node(i+1,j);
	    node[3] = get_edge_centre_negative_i(i,j);
	    node[4] = get_cell_centroid(i,j);
	    node[5] = get_edge_centre_positive_i(i,j);
	    node[6] = get_node(i,j+1);
	    node[7] = get_edge_centre_positive_j(i,j);
	    node[8] = get_node(i+1,j+1);

	    Area_and_Centroid_2D values_0 = compute_area_and_centroid(node[0],node[1],node[4],node[3]);
	    Area_and_Centroid_2D values_1 = compute_area_and_centroid(node[1],node[2],node[5],node[4]);
	    Area_and_Centroid_2D values_2 = compute_area_and_centroid(node[3],node[4],node[7],node[6]);
	    Area_and_Centroid_2D values_3 = compute_area_and_centroid(node[4],node[5],node[8],node[7]);
	    area[0] = values_0.area;
	    area[1] = values_1.area;
	    area[2] = values_2.area;
	    area[3] = values_3.area;
	    centroid[0] = values_0.centroid;
	    centroid[1] = values_1.centroid;
	    centroid[2] = values_2.centroid;
	    centroid[3] = values_3.centroid;

	    value_const = Abgrall_2D_Function(node[4].x(),node[4].y());
	    value[0] = Abgrall_2D_Function(centroid[0].x(),centroid[0].y());
	    value[1] = Abgrall_2D_Function(centroid[1].x(),centroid[1].y());
	    value[2] = Abgrall_2D_Function(centroid[2].x(),centroid[2].y());
	    value[3] = Abgrall_2D_Function(centroid[3].x(),centroid[3].y());
	    ei[0] = std::fabs(value[0]-value_const);
	    ei[1] = std::fabs(value[1]-value_const);
	    ei[2] = std::fabs(value[2]-value_const);
	    ei[3] = std::fabs(value[3]-value_const);

	    L1_L2_Linf.L1 = L1_L2_Linf.L1+ei[0]*area[0]+ei[1]*area[1]+ei[2]*area[2]+ei[3]*area[3];
	    L1_L2_Linf.L2 = L1_L2_Linf.L2+ei[0]*ei[0]+ei[1]*ei[1]+ei[2]*ei[2]+ei[3]*ei[3];
	    L1_L2_Linf.L_inf = compare(L1_L2_Linf.L_inf,ei[0],ei[1],ei[2],ei[3]);
	  }
	}
	return L1_L2_Linf;
      }
  }; 
}//namespace knoblauch

#endif
