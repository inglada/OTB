/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshToQuadEdgeMeshFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-04-22 01:41:45 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkQuadEdgeMeshToQuadEdgeMeshFilter_txx
#define __itkQuadEdgeMeshToQuadEdgeMeshFilter_txx

#include "itkQuadEdgeMeshToQuadEdgeMeshFilter.h"

namespace itk
{
// ---------------------------------------------------------------------
template< class TInputMesh, class TOutputMesh >
QuadEdgeMeshToQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::QuadEdgeMeshToQuadEdgeMeshFilter()
{
  this->Superclass::SetNumberOfRequiredInputs( 1 );
  this->Superclass::SetNumberOfRequiredOutputs( 1 );

  this->Superclass::SetNthOutput( 0, OutputMeshType::New() );
}

// ---------------------------------------------------------------------
template< class TInputMesh, class TOutputMesh >
void 
QuadEdgeMeshToQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::CopyInputMeshToOutputMesh()
{
  InputMeshConstPointer in = this->GetInput();
  OutputMeshPointer out = this->GetOutput();

  // Copy points
  InputPointsContainerConstIterator inIt = in->GetPoints()->Begin();
  while( inIt != in->GetPoints()->End() )
    {
    OutputPointType pOut;
    pOut.CastFrom( inIt.Value() );
    out->SetPoint( inIt.Index(), pOut );
    inIt++;
    } 

  // Copy Edge Cells
  InputCellsContainerConstIterator ecIt = in->GetEdgeCells()->Begin();
  while( ecIt != in->GetEdgeCells()->End() )
    {
    InputEdgeCellType* pe = 
      dynamic_cast< InputEdgeCellType* >( ecIt.Value());
    //if( pe )
      {
      out->AddEdgeWithSecurePointList( pe->GetQEGeom()->GetOrigin(),
                                       pe->GetQEGeom()->GetDestination() );
      }
    ecIt++;
    }


  // Copy cells
  InputCellsContainerConstIterator cIt = in->GetCells()->Begin();
  while( cIt != in->GetCells()->End() )
    {
    InputPolygonCellType* pe = 
      dynamic_cast< InputPolygonCellType* >( cIt.Value());
    //if( pe )
      {
      InputPointIdList points;
      InputPointsIdInternalIterator pit = pe->InternalPointIdsBegin();
      while( pit != pe->InternalPointIdsEnd( ) )
        {
        points.push_back( ( *pit ) );
        ++pit;
        }
      out->AddFaceWithSecurePointList( points, false );
      }
    cIt++;
    }
}

} // end namespace itk

#endif 
