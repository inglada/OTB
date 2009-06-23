/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshScalarDataVTKPolyDataWriter.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-02 18:43:05 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkQuadEdgeMeshScalarDataVTKPolyDataWriter_txx
#define __itkQuadEdgeMeshScalarDataVTKPolyDataWriter_txx

#include "itkQuadEdgeMeshScalarDataVTKPolyDataWriter.h"

namespace itk
{

// 
// Constructor
// 
template< class TMesh >
QuadEdgeMeshScalarDataVTKPolyDataWriter<TMesh> 
::QuadEdgeMeshScalarDataVTKPolyDataWriter() 
{
  m_CellDataName = "";
  m_PointDataName = "";
}

// 
// Destructor
// 
template< class TMesh >
QuadEdgeMeshScalarDataVTKPolyDataWriter<TMesh> 
::~QuadEdgeMeshScalarDataVTKPolyDataWriter()
{
}

template< class TMesh >
void
QuadEdgeMeshScalarDataVTKPolyDataWriter<TMesh> 
::GenerateData()
{
  this->Superclass::GenerateData();
  this->WriteCellData();
  this->WritePointData();
}

template< class TMesh >
void
QuadEdgeMeshScalarDataVTKPolyDataWriter<TMesh> 
::WriteCellData()
{
  CellDataContainerPointer celldata = this->m_Input->GetCellData();

  if( celldata )
    {
    if( celldata->Size() != 0 )
      {
      std::ofstream outputFile( this->m_FileName.c_str(), std::ios_base::app );

      outputFile <<"CELL_DATA " <<this->m_Input->GetNumberOfFaces() <<std::endl;
      outputFile <<"SCALARS ";

      if( m_CellDataName != "" )
        {
        outputFile <<m_CellDataName <<" " <<m_CellDataName <<std::endl;
        }
      else
        {
        outputFile <<"double double" <<std::endl;
        }
      
      outputFile <<"LOOKUP_TABLE default" <<std::endl;

      unsigned long k(0);

      CellsContainerPointer cells = this->m_Input->GetCells();
      CellsContainerIterator it = cells->Begin();

      for( CellDataContainerIterator c_it = celldata->Begin();
          c_it != celldata->End();
          ++c_it, ++it )
        {
        CellType* cellPointer = it.Value();
        if( cellPointer->GetType() != 1 )
          {
          outputFile <<c_it.Value();
          if( k++ % 3 == 0 )
            {
            outputFile <<std::endl;
            }
          }
        }
      outputFile <<std::endl;
      outputFile.close();
      }
    }
}

template< class TMesh >
void
QuadEdgeMeshScalarDataVTKPolyDataWriter<TMesh> 
::WritePointData()
{
  PointDataContainerConstPointer pointdata = this->m_Input->GetPointData();

  if( pointdata )
    {
    std::ofstream outputFile( this->m_FileName.c_str(), std::ios_base::app );

    outputFile <<"POINT_DATA " <<this->m_Input->GetNumberOfPoints() <<std::endl;
    outputFile <<"SCALARS ";

    if( m_PointDataName != "" )
      {
      outputFile <<m_PointDataName <<" " <<m_PointDataName <<std::endl;
      }
    else
      {
      outputFile <<"double double"<<std::endl;
      }

    outputFile <<"LOOKUP_TABLE default" <<std::endl; 

    unsigned long k = 0;

    PointDataContainerIterator c_it = pointdata->Begin();

    while(  c_it != pointdata->End() )
      {
      outputFile <<c_it.Value() <<" ";
      if( k % 3 == 0 )
        {
        outputFile <<std::endl;
        }

      ++c_it;
      ++k;
      }

    outputFile << std::endl;
    outputFile.close();
    }
}

}

#endif
