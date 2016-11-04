#ifndef WRITE_SOLUTION_VTK_H
#define WRITE_SOLUTION_VTK_H

#include<string>
#include<sstream>
#include<cassert>

#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>

#include"Solution_Block_2D.h"

namespace {

  std::string default_variable_name(int i) {
    std::stringstream name;
    name << "U_" << i;
    return name.str();
  }
}

namespace knoblauch {

void write_solution_vtk(const Solution_Block_2D & solution_block,
                        const std::string &filename) {

  
  const unsigned int num_i = solution_block.get_number_of_nodes_i();
  const unsigned int num_j = solution_block.get_number_of_nodes_j();

  vtkSmartPointer<vtkStructuredGrid> structuredGrid =
    vtkSmartPointer<vtkStructuredGrid>::New();

  //////////////////////////////////
  //get nodes
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  //////////////////////////////////
  //values of all variables
  vtkSmartPointer<vtkDoubleArray> field
    = vtkSmartPointer<vtkDoubleArray>::New();
  field->SetName("Function");


  //solution_block.give_value(solution_block);
  std::vector<float_type> U = solution_block.get_variable();

  ////////////////////////////////////
  // output nodes
  for(unsigned int j=0; j<num_j; ++j) {
    for(unsigned int i=0; i<num_i; ++i) {
      Vector2D node = solution_block.get_node(i,j);
      
      points->InsertNextPoint(node.x(), node.y(), 0.0);
      //for(unsigned int k=0; k<num_variables; ++k) {
      //all_fields[k]->InsertNextValue(U[k]);
      //}
    }
  }

  /////////////////////////////////////
  // Output cell-centered values
  for(unsigned int j=0; j<num_j-1; ++j) {
    for(unsigned int i=0; i<num_i-1; ++i) {
      int k = (num_i-1)*j + i;
      field->InsertNextValue(U[k]);
    }
  }

  //////////////////////////////////
  // Specify the dimensions of the grid
  structuredGrid->SetDimensions(num_i,num_j,1);
  structuredGrid->SetPoints(points);
  structuredGrid->GetCellData()->SetScalars(field);

  //////////////////////////////////
  // Write file
  vtkSmartPointer<vtkXMLStructuredGridWriter> writer =
    vtkSmartPointer<vtkXMLStructuredGridWriter>::New();

  writer->SetFileName(filename.c_str());

#if VTK_MAJOR_VERSION <= 5
  writer->SetInput(structuredGrid);
#else
  writer->SetInputData(structuredGrid);
#endif


  writer->Write();


}

} //namespace knoblauch

#endif //#ifndef WRITE_SOLUTION_VTK_H
