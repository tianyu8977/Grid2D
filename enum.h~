#ifndef CIRCULAR_ARC2D_H
#define CIRCULAR_ARC2D_H

#include<cassert>
#include<vector>
#include<cmath>
#include"Curve2D_Base.h"
#include"../../Math/Math_Constants.h"

namespace knoblauch {

 class Circular_Arc2D : public Curve2D_Base {
 protected:

   Vector2D node_origin;
   Vector2D node_begin;
   float_type angle;
   float_type pi = constants::PI;
   float_type theta = atan2((node_begin.y()-node_origin.y()),(node_begin.x()-node_origin.x()));
 public:

   Vector2D get_position_at_s(float_type s) {
     float_type r = sqrt((node_begin.x()-node_origin.x())*(node_begin.x()-node_origin.x())+(node_begin.y()-node_origin.y())*(node_begin.y()-node_origin.y()));
     Vector2D node_recent;

     node_recent.x() = node_origin.x()+r*cos(angle*s+theta);
     node_recent.y() = node_origin.y()+r*sin(angle*s+theta);
     return node_recent;
   }
   
   float_type get_s_at_position(Vector2D point) {
     float_type ratio_x = 0.0;
     float_type ratio_y = 0.0;
     float_type s=0.0;

     ratio_x = (point.x()-node_origin.x())/(point-node_origin).norm();
     ratio_y = (point.y()-node_origin.y())/(point-node_origin).norm();

     if(ratio_x>=0 && ratio_y>=0) {
       s = (asin(ratio_y)-theta)/angle;
     }
     if(ratio_x<0 && ratio_y>=0) {
       s = (acos(ratio_x)-theta)/angle;
     }
     if(ratio_x<0 && ratio_y<0) {
       s = (2*pi-acos(ratio_y)-theta)/angle;
     }
     if(ratio_x>=0 && ratio_y<0) {
       s = (2*pi-asin(ratio_y)-theta)/angle;
     }
       if(s<0) {
	 s=0.0;
       }
       if(s>1) {
	 s=1.0;
       }
       return s;
   }

   Circular_Arc2D(const Vector2D &origin,
		  const Vector2D &begin,
		  float_type angle_in) :
   node_origin(origin),
   node_begin(begin),
   angle(angle_in) {}
   };
 }

#endif
