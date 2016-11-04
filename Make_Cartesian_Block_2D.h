#ifndef MAKE_CARTESIAN_BLOCK_2D_H
#define MAKE_CARTESIAN_BLOCK_2D_H

#include"Block_2D.h"

namespace knoblauch {

  Block_2D make_cartesian_block_2D(double x_min, double x_max, unsigned int num_x,
                                   double y_min, double y_max, unsigned int num_y);


}//namespace knoblauch



#endif //#ifndef BLOCK_2D_H
