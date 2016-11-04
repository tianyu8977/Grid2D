#ifndef GEOMETRY_2D_H
#define GEOMETRY_2D_H

#include"../Typedefs/Typedefs.h"
#include"../Lin_Alg/Basic_Linear_Algebra.h"

namespace knoblauch {

////////////////////////////////////////
//  struct to return area and centroid
struct Area_and_Centroid_2D {
  float_type area = 0.0;
  Vector2D centroid = Vector2D::Zero();
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

///////////////////////////////////////////
//  compute both the area and centroid
// of an arbitrary, non-self-intersecting
// quadrilateral.  Nodes must be in
// order of occurence around quadrilateral's
// perimeter.
// See: http://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
// and: http://paulbourke.net/geometry/polygonmesh/
inline Area_and_Centroid_2D compute_area_and_centroid(Vector2D node0,
                                                      Vector2D node1,
                                                      Vector2D node2,
                                                      Vector2D node3) {
  Area_and_Centroid_2D area_and_centroid;

  float_type temp = cross_product(node0,node1);
  area_and_centroid.area      = temp;
  area_and_centroid.centroid  = temp*(node0+node1);

  temp = cross_product(node1,node2);
  area_and_centroid.area     += temp;
  area_and_centroid.centroid += temp*(node1+node2);

  temp = cross_product(node2,node3);
  area_and_centroid.area     += temp;
  area_and_centroid.centroid += temp*(node2+node3);

  temp = cross_product(node3,node0);
  area_and_centroid.area     += temp;
  area_and_centroid.centroid += temp*(node3+node0);

  area_and_centroid.area /= 2.0;
  area_and_centroid.centroid /= 6.0*area_and_centroid.area;

  if(area_and_centroid.area < 0.0) {
    area_and_centroid.area *= -1.0;
  }

  return area_and_centroid;
}

}//namespace knoblauch

#endif //#ifndef GEOMETRY_2D_H
