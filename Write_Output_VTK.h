#ifndef WRITE_OUTPUT_VTK_H
#define WRITE_OUTPUT_VTK_H

#include<string>

#include"Block_2D.h"

namespace knoblauch {

void write_output_vtk(const Block_2D & block, const std::string &filename);

} //namespace knoblauch

#endif //#ifndef WRITE_OUTPUT_VTK_H
