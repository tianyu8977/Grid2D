#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>

#include "Write_Output_VTK.h"

void knoblauch::write_output_vtk(const knoblauch::Block_2D & block,
                                 const std::string &filename) {

  const unsigned int num_i = block.get_number_of_nodes_i();
  const unsigned int num_j = block.get_number_of_nodes_j();

  vtkSmartPointer<vtkStructuredGrid> structuredGrid =
    vtkSmartPointer<vtkStructuredGrid>::New();

  //////////////////////////////////
  //get nodes
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  for(unsigned int j=0; j<num_j; ++j) {
    for(unsigned int i=0; i<num_i; ++i) {
      Vector2D node = block.get_node(i,j);
      points->InsertNextPoint(node.x(), node.y(), 0.0);
    }
  }

  //////////////////////////////////
  // Specify the dimensions of the grid
  structuredGrid->SetDimensions(num_i,num_j,1);
  structuredGrid->SetPoints(points);

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

