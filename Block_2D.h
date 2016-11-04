#ifndef BLOCK_2D_H
#define BLOCK_2D_H

#include<cassert>
#include<vector>
#include<memory>

#include"../Typedefs/Typedefs.h"
#include"../Lin_Alg/Basic_Linear_Algebra.h"
#include"../Math/Math_Constants.h"
#include"Geometry_2D.h"
#include"Curve2D/Curve2D_Base.h"

namespace knoblauch {

class Block_2D {
public:
	
  //no default constructor
  Block_2D() = delete;


  //default copy and move constructor
  Block_2D(const Block_2D &) = default;
  Block_2D(Block_2D&&) = default;

  //other constructor
  Block_2D(unsigned int num_nodes_i,
           unsigned int num_nodes_j,
           const std::vector<Vector2D>& nodes_in,
	   const std::shared_ptr<Curve2D_Base>& boundary_i_pos_in,
           const std::shared_ptr<Curve2D_Base>& boundary_i_neg_in,
	   const std::shared_ptr<Curve2D_Base>& boundary_j_pos_in,
	   const std::shared_ptr<Curve2D_Base>& boundary_j_neg_in) :
    number_of_nodes_i(num_nodes_i),
    number_of_nodes_j(num_nodes_j),
    number_of_nodes((number_of_nodes_i)*(number_of_nodes_j)),
    number_of_cells_i(number_of_nodes_i-1),
    number_of_cells_j(number_of_nodes_j-1),
    number_of_cells(number_of_cells_i*number_of_cells_j),
    cells_stride(number_of_cells_i),
    nodes_stride(number_of_nodes_i),
    nodes(nodes_in),
    centroids(number_of_cells),
    areas(number_of_cells),
    boundary_i_pos(boundary_i_pos_in),
    boundary_i_neg(boundary_i_neg_in),
    boundary_j_pos(boundary_j_pos_in),
    boundary_j_neg(boundary_j_neg_in)
  {
    assert(number_of_nodes==nodes.size());
    update_all_cell_centroids_and_areas();
  }


  //assignemnt operators
  Block_2D& operator=(const Block_2D&) = default;
  Block_2D& operator=(Block_2D&&) = default;
    
  //number of cells and nodes
  int get_number_of_cells() const {return number_of_cells;}
  int get_number_of_cells_i() const {return number_of_cells_i;}
  int get_number_of_cells_j() const {return number_of_cells_j;}
  int get_number_of_nodes() const {return number_of_nodes;}
  int get_number_of_nodes_i() const {return number_of_nodes_i;}
  int get_number_of_nodes_j() const {return number_of_nodes_j;}

  //access functions
  Vector2D   get_node(unsigned int i, unsigned int j) const;
  Vector2D   get_cell_centroid(unsigned int i, unsigned int j) const;
  float_type get_cell_area(unsigned int i, unsigned int j) const;

  //edge_length
  float_type get_edge_length_positive_i(unsigned int i, unsigned int j) const;
  float_type get_edge_length_negative_i(unsigned int i, unsigned int j) const;
  float_type get_edge_length_positive_j(unsigned int i, unsigned int j) const;
  float_type get_edge_length_negative_j(unsigned int i, unsigned int j) const;

  //unit normals
  Vector2D get_outward_unit_normal_positive_i(unsigned int i, unsigned int j) const;
  Vector2D get_outward_unit_normal_negative_i(unsigned int i, unsigned int j) const;
  Vector2D get_outward_unit_normal_positive_j(unsigned int i, unsigned int j) const;
  Vector2D get_outward_unit_normal_negative_j(unsigned int i, unsigned int j) const;

  //edge_centre
  Vector2D get_edge_centre_positive_i(unsigned int i, unsigned int j) const;
  Vector2D get_edge_centre_negative_i(unsigned int i, unsigned int j) const;
  Vector2D get_edge_centre_positive_j(unsigned int i, unsigned int j) const;
  Vector2D get_edge_centre_negative_j(unsigned int i, unsigned int j) const;

  std::shared_ptr<Curve2D_Base> get_boundary_i_pos() const {return boundary_i_pos;}
  std::shared_ptr<Curve2D_Base> get_boundary_i_neg() const {return boundary_i_neg;}
  std::shared_ptr<Curve2D_Base> get_boundary_j_pos() const {return boundary_j_pos;}
  std::shared_ptr<Curve2D_Base> get_boundary_j_neg() const {return boundary_j_neg;}

  protected:

  unsigned int number_of_nodes_i;
  unsigned int number_of_nodes_j;
  unsigned int number_of_nodes;
  unsigned int number_of_cells_i;
  unsigned int number_of_cells_j;
  unsigned int number_of_cells;
  unsigned int cells_stride;
  unsigned int nodes_stride;

  std::vector<Vector2D>        nodes;
  std::vector<Vector2D>    centroids;
  std::vector<float_type>      areas;

  //4 boundary curves
  std::shared_ptr<Curve2D_Base> boundary_i_pos;
  std::shared_ptr<Curve2D_Base> boundary_i_neg;
  std::shared_ptr<Curve2D_Base> boundary_j_pos;
  std::shared_ptr<Curve2D_Base> boundary_j_neg;

  //update centroids and areas
  void update_cell_centroid_and_area(unsigned int i, unsigned int j);
  void update_all_cell_centroids_and_areas();

  //get cell and node indices
  unsigned int get_node_index(unsigned int i, unsigned int j) const;
  unsigned int get_cell_index(unsigned int i, unsigned int j) const;

};  // class Block_2D


}//namespace knoblauch

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//                   Inline functions
//


///////////////////////////////////////////////////////////////
////              Import Nodes
//inline void knoblauch::Block_2D::import_nodes(std::vector<Vector2D>&& nodes_in) {
//  assert(number_of_nodes==nodes_in.size());
//  nodes = std::move(nodes_in);
//}
//
//inline void knoblauch::Block_2D::import_nodes(const std::vector<Vector2D>& nodes_in) {
//  assert(number_of_nodes==nodes_in.size());
//  nodes = nodes_in;
//}
//
//////////////////////////////////////////////////////////////
//                Get node and cell index
inline unsigned int knoblauch::Block_2D::get_node_index (unsigned int i, unsigned int j) const {
  assert(i<number_of_nodes_i);
  assert(j<number_of_nodes_j);
  return i+j*nodes_stride;
}

inline unsigned int knoblauch::Block_2D::get_cell_index (unsigned int i, unsigned int j) const {
  assert(i<number_of_cells_i);
  assert(j<number_of_cells_j);
  return i+j*cells_stride;
}

///////////////////////////////////////////////////////////////
///                       get node
inline knoblauch::Vector2D knoblauch::Block_2D::get_node(unsigned int i,
                                                         unsigned int j) const {
  return nodes[get_node_index(i,j)];
}

///////////////////////////////////////////////////////////////
///                   get cell centroid
inline knoblauch::Vector2D knoblauch::Block_2D::get_cell_centroid(unsigned int i,
                                                                  unsigned int j) const {

  return centroids[get_cell_index(i,j)];
}

///////////////////////////////////////////////////////////////
///                   get cell area
inline knoblauch::float_type knoblauch::Block_2D::get_cell_area(unsigned int i,
                                                                unsigned int j) const {

  return areas[get_cell_index(i,j)];
}

///////////////////////////////////////////////////////////////
///                   get cell edge lengths
inline knoblauch::float_type
knoblauch::Block_2D::get_edge_length_positive_i(unsigned int i, unsigned int j) const {
  return (get_node(i+1,j+1)-get_node(i+1,j)).norm();
}

inline knoblauch::float_type
knoblauch::Block_2D::get_edge_length_negative_i(unsigned int i, unsigned int j) const {
  return (get_node(i,j+1)-get_node(i,j)).norm();
}

inline knoblauch::float_type
knoblauch::Block_2D::get_edge_length_positive_j(unsigned int i, unsigned int j) const {
  return (get_node(i+1,j+1)-get_node(i,j+1)).norm();
}

inline knoblauch::float_type
knoblauch::Block_2D::get_edge_length_negative_j(unsigned int i, unsigned int j) const {
  return (get_node(i+1,j)-get_node(i,j)).norm();
}

///////////////////////////////////////////////////////////////
///                   get cell unit normals
inline knoblauch::Vector2D
knoblauch::Block_2D::get_outward_unit_normal_positive_i(unsigned int i,
                                                        unsigned int j) const {
  Vector2D normal = Rotation2D(constants::PI_2)*(get_node(i+1,j)-get_node(i+1,j+1));
  return normal/normal.norm();
}

inline knoblauch::Vector2D
knoblauch::Block_2D::get_outward_unit_normal_negative_i(unsigned int i,
                                                        unsigned int j) const {
  Vector2D normal = Rotation2D(constants::PI_2)*(get_node(i,j+1)-get_node(i,j));
  return normal/normal.norm();
}

inline knoblauch::Vector2D
knoblauch::Block_2D::get_outward_unit_normal_positive_j(unsigned int i,
                                                        unsigned int j) const {
  Vector2D normal = Rotation2D(constants::PI_2)*(get_node(i+1,j+1)-get_node(i,j+1));
  return normal/normal.norm();
}

inline knoblauch::Vector2D
knoblauch::Block_2D::get_outward_unit_normal_negative_j(unsigned int i,
                                                        unsigned int j) const {
  Vector2D normal = Rotation2D(constants::PI_2)*(get_node(i,j)-get_node(i+1,j));
  return normal/normal.norm();
}

///////////////////////////////////////////////////////////////
///                   get edge centre
inline knoblauch::Vector2D
knoblauch::Block_2D::get_edge_centre_positive_i(unsigned int i, unsigned int j) const {
  return 0.5*(get_node(i+1,j)+get_node(i+1,j+1));
}

inline knoblauch::Vector2D
knoblauch::Block_2D::get_edge_centre_negative_i(unsigned int i, unsigned int j) const {
  return 0.5*(get_node(i,j)+get_node(i,j+1));
}

inline knoblauch::Vector2D
knoblauch::Block_2D::get_edge_centre_positive_j(unsigned int i, unsigned int j) const {
  return 0.5*(get_node(i,j+1)+get_node(i+1,j+1));
}

inline knoblauch::Vector2D
knoblauch::Block_2D::get_edge_centre_negative_j(unsigned int i, unsigned int j) const {
  return 0.5*(get_node(i,j)+get_node(i+1,j));
}

///////////////////////////////////////////////////////////////
///             update cell centroids and areas
inline void knoblauch::Block_2D::update_all_cell_centroids_and_areas() {

  for(unsigned int i=0; i<number_of_cells_i; ++i) {
    for(unsigned int j=0; j<number_of_cells_j; ++j) {
      update_cell_centroid_and_area(i,j);
    }
  }

}

inline void knoblauch::Block_2D::update_cell_centroid_and_area(unsigned int i, unsigned int j) {
  Area_and_Centroid_2D a_and_c = compute_area_and_centroid(get_node(i  , j  ),
                                                           get_node(i+1, j  ),
                                                           get_node(i+1, j+1),
                                                           get_node(i  , j+1));
  unsigned int index = get_cell_index(i,j);
  areas[index]     = a_and_c.area;
  centroids[index] = a_and_c.centroid;
}


#endif //#ifndef BLOCK_2D_H
