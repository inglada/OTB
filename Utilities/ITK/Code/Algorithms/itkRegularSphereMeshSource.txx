/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegularSphereMeshSource.txx,v $
  Language:  C++
  Date:      $Date: 2009-09-17 11:04:02 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRegularSphereMeshSource_txx
#define __itkRegularSphereMeshSource_txx

#include "itkRegularSphereMeshSource.h"

namespace itk
{

/**
 *
 */
template<class TOutputMesh>
RegularSphereMeshSource<TOutputMesh>
::RegularSphereMeshSource()
{
  /**
   * Create the output
   */
  typename TOutputMesh::Pointer output = TOutputMesh::New();
  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput(0, output.GetPointer());
  m_Center.Fill(0);
  m_Scale.Fill(1);
  m_Resolution = 2;
}

/*
 *
 */
template<class TOutputMesh>
void
RegularSphereMeshSource<TOutputMesh>
::GenerateData()
{
  unsigned long tripoints[3] = {0,1,2};
  typename OutputMeshType::Pointer outputMesh = this->GetOutput();  

  outputMesh->SetCellsAllocationMethod( OutputMeshType::CellsAllocatedDynamicallyCellByCell );

  PointsContainerPointer  myPoints = outputMesh->GetPoints();
  
  PointType p1;
  unsigned long idx = 0;

  p1[0] = 1 * m_Scale[0] + m_Center[0];
  p1[1] = 0 * m_Scale[1] + m_Center[1];
  p1[2] = 0 * m_Scale[2] + m_Center[2];
  outputMesh->SetPoint(idx++, p1);

  p1[0] = -1 * m_Scale[0] + m_Center[0];
  p1[1] = 0 * m_Scale[1] + m_Center[1];
  p1[2] = 0 * m_Scale[2] + m_Center[2];
  outputMesh->SetPoint(idx++, p1);

  p1[0] = 0 * m_Scale[0] + m_Center[0];
  p1[1] = 1 * m_Scale[1] + m_Center[1];
  p1[2] = 0 * m_Scale[2] + m_Center[2];
  outputMesh->SetPoint(idx++, p1);

  p1[0] = 0 * m_Scale[0] + m_Center[0];
  p1[1] = -1 * m_Scale[1] + m_Center[1];
  p1[2] = 0* m_Scale[2] + m_Center[2];
  outputMesh->SetPoint(idx++, p1);
  
  p1[0] = 0* m_Scale[0] + m_Center[0];
  p1[1] = 0* m_Scale[1] + m_Center[1];
  p1[2] = 1* m_Scale[2] + m_Center[2];
  outputMesh->SetPoint(idx++, p1);

  p1[0] = 0* m_Scale[0] + m_Center[0];
  p1[1] = 0* m_Scale[1] + m_Center[1];
  p1[2] = -1* m_Scale[2] + m_Center[2];
  outputMesh->SetPoint(idx++, p1);

  /* Six equidistant points lying on the unit sphere */
  const unsigned long XPLUS = 0;
  const unsigned long XMIN  = 1;
  const unsigned long YPLUS = 2;
  const unsigned long YMIN  = 3;
  const unsigned long ZPLUS = 4;
  const unsigned long ZMIN  = 5;

  tripoints[0] = YPLUS; tripoints[1] = ZPLUS; tripoints[2] = XPLUS; 
  this->AddCell( outputMesh, tripoints, 0 );
  
  tripoints[0] = YPLUS; tripoints[1] = XMIN; tripoints[2] = ZPLUS; 
  this->AddCell( outputMesh, tripoints, 1 );
  
  tripoints[0] = XMIN; tripoints[1] = YMIN; tripoints[2] = ZPLUS; 
  this->AddCell( outputMesh, tripoints, 2 );

  tripoints[0] = ZPLUS; tripoints[1] = YMIN; tripoints[2] = XPLUS; 
  this->AddCell( outputMesh, tripoints, 3 );

  tripoints[0] = ZMIN; tripoints[1] = YPLUS; tripoints[2] = XPLUS; 
  this->AddCell( outputMesh, tripoints, 4 );

  tripoints[0] = YPLUS; tripoints[1] = ZMIN; tripoints[2] = XMIN; 
  this->AddCell( outputMesh, tripoints, 5 );

  tripoints[0] = ZMIN; tripoints[1] = YMIN; tripoints[2] = XMIN; 
  this->AddCell( outputMesh, tripoints, 6 );

  tripoints[0] = ZMIN; tripoints[1] = XPLUS; tripoints[2] = YMIN; 
  this->AddCell( outputMesh, tripoints, 7 );

  // linear subdivision of the original triangular mesh
  // followed by a repositioning of the point on the sphere
  // for each resolution.
  // it's an IN-PLACE process.
  unsigned int i;
  for (i = 0; i < m_Resolution; i++) 
    {
    typename OutputMeshType::CellsContainerPointer myCells = outputMesh->GetCells();
    typename OutputMeshType::CellsContainer::Iterator cells = myCells->Begin();
    
    typename OutputMeshType::Pointer result = OutputMeshType::New();
    PointType v[3];
    PointType* v_pt[3];
    v_pt[0] = &v[0];
    v_pt[1] = &v[1];
    v_pt[2] = &v[2];
    const unsigned long *tp;
    unsigned long cellIdx=0;
    unsigned long pointIdxOffset = outputMesh->GetNumberOfPoints();
    unsigned long pointIdx = pointIdxOffset;
    unsigned long newIdx[3] = {0,1,2};
  
    // container for the processed edges
    // when subdividing a triangle, the corresponding subdivided
    // edges are stocked here with the Id of the middle point.
    PointMapType::Pointer handledEdges = PointMapType::New();

    // for the points Id to be consecutive,
    // and for the Ids to exist only if the point has been copied
    // i.e. even if the container is a vector,
    // we ned to copy the old points first.
    for( unsigned long j = 0; j < pointIdxOffset; j++ )
      {
      outputMesh->GetPoint( j,v_pt[0]);
      // this is needed when the PointType is a QuadEdgeMeshPoint 
      PointType localPt;
      localPt[0] = v[0][0];
      localPt[1] = v[0][1];
      localPt[2] = v[0][2];
      // copy the point in the output mesh
      result->SetPoint(j, localPt);
      }

    // subdivide each triangular cell
    while( cells != myCells->End() ) 
      {
      // this is a QE specific case
      // with an itk::Mesh, no edge should be present in the cell container
      if( cells.Value()->GetNumberOfPoints() > 2 )
        {
        // get the point Ids
        tp = cells.Value()->GetPointIds();

        // for each point of the input triangle, create a copy in the output mesh
        for( unsigned int ii = 0; ii < 3; ii++ )
          {
          // get the point's geometry from previous mesh
          outputMesh->GetPoint(tp[ii],v_pt[ii]);

          // This is a QE specific case
          // if the point already is in the output mesh
          // we should not overwrite it as it would
          // reset the link to the Quad Edge Ring
          if( !result->GetPoints()->IndexExists( tp[ii] ) )
            {
            // this is needed when the PointType is a QuadEdgeMeshPoint 
            PointType localPt;
            localPt[0] = v[ii][0];
            localPt[1] = v[ii][1];
            localPt[2] = v[ii][2];

            // copy the point in the output mesh
            result->SetPoint(tp[ii], localPt);
            }
          }

        // point 1
        if (!handledEdges->IndexExists(std::make_pair(tp[0], tp[1])) &&
            !handledEdges->IndexExists(std::make_pair(tp[1], tp[0])))
          {
          newIdx[0]=pointIdx;
          handledEdges->InsertElement(std::make_pair(tp[0], tp[1]), pointIdx);
          result->SetPoint(pointIdx++, this->Divide(v[0],v[1]) );
          }
        else
          {
          if (handledEdges->IndexExists(std::make_pair(tp[0], tp[1]))) 
            {
            newIdx[0] = handledEdges->GetElement(std::make_pair(tp[0], tp[1]));
            }
          else 
            {
            newIdx[0] = handledEdges->GetElement(std::make_pair(tp[1], tp[0]));
            }
          }

        // point 2
        if (!handledEdges->IndexExists(std::make_pair(tp[1], tp[2])) && 
            !handledEdges->IndexExists(std::make_pair(tp[2], tp[1])))
          {
          newIdx[1] = pointIdx;
          handledEdges->InsertElement(std::make_pair(tp[1], tp[2]), pointIdx);
          result->SetPoint(pointIdx++, this->Divide(v[1],v[2]));
          }
        else
          { 
          if (handledEdges->IndexExists(std::make_pair(tp[1], tp[2]))) 
            {
            newIdx[1] = handledEdges->GetElement(std::make_pair(tp[1], tp[2]));
            }
          else 
            {
            newIdx[1] = handledEdges->GetElement(std::make_pair(tp[2], tp[1]));
            }
          }  

        // point 3
        if (!handledEdges->IndexExists(std::make_pair(tp[2], tp[0])) &&
            !handledEdges->IndexExists(std::make_pair(tp[0], tp[2])))
          {
          newIdx[2] = pointIdx;
          handledEdges->InsertElement(std::make_pair(tp[2], tp[0]), pointIdx);
          result->SetPoint(pointIdx++, this->Divide(v[2],v[0]));
          }
        else
          {
          if (handledEdges->IndexExists(std::make_pair(tp[2], tp[0]))) 
            {
            newIdx[2] = handledEdges->GetElement(std::make_pair(tp[2], tp[0]));
            }
          else 
            {
            newIdx[2] = handledEdges->GetElement(std::make_pair(tp[0], tp[2]));
            }
          }

        // create the 4 output triangles in place of the input triangle
        tripoints[0] = tp[0]; 
        tripoints[1] = newIdx[0]; 
        tripoints[2] = newIdx[2]; 
        this->AddCell(result,tripoints,cellIdx);
        cellIdx++;

        tripoints[0] = newIdx[0];
        tripoints[1] = tp[1];
        tripoints[2] = newIdx[1];
        this->AddCell(result,tripoints,cellIdx);
        cellIdx++;

        tripoints[0] = newIdx[1];
        tripoints[1] = tp[2];
        tripoints[2] = newIdx[2]; 
        this->AddCell(result,tripoints,cellIdx);
        cellIdx++;

        tripoints[0] = newIdx[0];
        tripoints[1] = newIdx[1]; 
        tripoints[2] = newIdx[2]; 
        this->AddCell(result,tripoints,cellIdx);
        cellIdx++;
        }

      // for all cells
      cells++;
      }

    // Release input memory
    cells = myCells->Begin();
    while( cells != myCells->End() )
      {
      const CellInterfaceType * cellToBeDeleted = cells->Value();
      delete cellToBeDeleted;
      cells++;
      }

    // set output
    outputMesh->Graft(result);

    result->SetCells( NULL );
    }
}


template<class TOutputMesh>
typename RegularSphereMeshSource<TOutputMesh>::PointType
RegularSphereMeshSource<TOutputMesh>
::Divide( const PointType & p1, const PointType & p2) const
{
  PointType p;
  PointType f;
  PointType result;

  VectorType d;
  VectorType c;

  d = p2 - p1;
  p = p1 + (d * 0.5);
  c = p - m_Center;

  f[0] = m_Scale[0] / c.GetNorm();
  f[1] = m_Scale[1] / c.GetNorm();
  f[2] = m_Scale[2] / c.GetNorm();

  c[0] *= f[0];
  c[1] *= f[1];
  c[2] *= f[2];

  result[0] = m_Center[0] + c[0]; 
  result[1] = m_Center[1] + c[1];
  result[2] = m_Center[2] + c[2];

  return (result);
}

template<class TOutputMesh>
void
RegularSphereMeshSource<TOutputMesh>
::AddCell( OutputMeshType * mesh, const unsigned long * pointIds, unsigned long idx)
{
  CellAutoPointer testCell(new TriCellType, true);
  testCell->SetPointIds(pointIds);
  mesh->SetCell(idx, testCell );
}


template<class TOutputMesh>
void
RegularSphereMeshSource<TOutputMesh>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Center: " << m_Center << std::endl;
  os << indent << "Scale: " << m_Scale << std::endl;
  os << indent << "Resolution: " << m_Resolution << std::endl;
}


} //end of namespace itk


#endif
