/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkTriangleMeshToSimplexMeshFilter.txx,v $
Language:  C++
Date:      $Date: 2004/07/31 20:49:26 $
Version:   $Revision: 1.3 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __TriangleMeshToSimplexMeshFilter_txx
#define __TriangleMeshToSimplexMeshFilter_txx
#include "itkTriangleMeshToSimplexMeshFilter.h"

namespace itk
  {

  template<typename TInputMesh, typename TOutputMesh>
    TriangleMeshToSimplexMeshFilter<TInputMesh,TOutputMesh>
    ::TriangleMeshToSimplexMeshFilter()
    {

    OutputMeshPointer output = TOutputMesh::New();
    this->ProcessObject::SetNumberOfRequiredOutputs(1);
    this->ProcessObject::SetNthOutput(0, output.GetPointer());
    m_HandledEdgeIds = IdVectorType::New();
    m_FaceSet = NULL;
    }

  template <typename TInputMesh, typename TOutputMesh>
    TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>
    ::~TriangleMeshToSimplexMeshFilter()
    {
    delete m_FaceSet;
    }


  template <typename TInputMesh, typename TOutputMesh>
    void TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>
    ::GenerateData()
    {
    Initialize();
    CreateSimplexPoints();
    CreateSimplexNeighbors();
    CreateCells();
    this->GetOutput()->BuildCellLinks();
    }


  template <typename TInputMesh, typename TOutputMesh>
    void TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>
    ::Initialize()
    {

    const unsigned long *tp;

    InputMeshPointer input = this->GetInput(0);
    m_IdOffset = input->GetNumberOfCells();


    m_EdgeCellId = 0;
    m_EdgeNeighborList = EdgeNeighborListType::New();
    m_VertexNeighborList = VertexNeighborListType::New();
    m_LineCellIndices = LineCellIndexType::New();
    m_Edges = EdgeMapType::New();
    if(m_FaceSet)
      {
      delete m_FaceSet;
      }

    m_FaceSet = new IndexSetType();
      
    InputPointType v1, v2, v3;


    for(unsigned int idx1=0; idx1 < m_IdOffset; idx1++)
      {
      m_FaceSet->insert(idx1);

      CellAutoPointer triCellPointer;
      input->GetCell( idx1, triCellPointer );
      tp = triCellPointer->GetPointIds();
      input->GetPoint(tp[0], &v1);
      input->GetPoint (tp[1], &v2);
      input->GetPoint (tp[2], &v3);

      CreateNewEdge(  idx1, 0, tp[0], tp[1] );
      CreateNewEdge(  idx1, 1, tp[1], tp[2] );
      CreateNewEdge(  idx1, 2, tp[2], tp[0] );
      }
    }


  template <typename TInputMesh, typename TOutputMesh>
    void TriangleMeshToSimplexMeshFilter<TInputMesh,TOutputMesh>
    ::CreateSimplexPoints()
    {
    //create the points of the simplex mesh
    IndexSetType::iterator faceIterator = m_FaceSet->begin();
    while (faceIterator != m_FaceSet->end())
      {
      InputPointType newPoint = ComputeFaceCenter(*faceIterator);
      OutputPointType copyPoint;
      copyPoint.CastFrom(newPoint);

      unsigned int id = *faceIterator;
      this->GetOutput()->SetPoint( id, copyPoint);
      this->GetOutput()->SetGeometryData( id, new itk::SimplexMeshGeometry() );
      faceIterator++;
      }
    }



  template <typename TInputMesh, typename TOutputMesh>
    void 
    TriangleMeshToSimplexMeshFilter<TInputMesh,TOutputMesh>
    ::CreateEdgeForTrianglePair( unsigned long pointIndex, unsigned long boundaryId)
    {
    EdgeIdentifierType facePair = m_EdgeNeighborList->GetElement(boundaryId);
    if (facePair.first == pointIndex) {
      this->GetOutput()->AddNeighbor( pointIndex , facePair.second);
      }
    else {
      this->GetOutput()->AddNeighbor( pointIndex , facePair.first);
      }
  if (!m_HandledEdgeIds->IndexExists( boundaryId )) 
    {
    unsigned long edgeId = this->GetOutput()->AddEdge( facePair.first, facePair.second );
    m_LineCellIndices->InsertElement(facePair, edgeId);
    m_HandledEdgeIds->InsertElement(boundaryId,edgeId);
    }
    }


  template <typename TInputMesh, typename TOutputMesh>
    void TriangleMeshToSimplexMeshFilter<TInputMesh,TOutputMesh>
    ::CreateSimplexNeighbors()
    {
    OutputMeshPointer output = this->GetOutput(0);

    // add neighbor vertices
    OutputPointsContainerPointer outputPointsContainer =  this->GetOutput(0)->GetPoints();
    OutputPointsContainerIterator points =  outputPointsContainer->Begin();

    unsigned long tp0,tp1,tp2;

    InputBoundaryAssignmentsContainerPointer cntlines = this->GetInput(0)->GetBoundaryAssignments( 1 );

    while ( points != outputPointsContainer->End() )
      {
      unsigned long idx = points.Index();
      InputBoundnaryAssignmentIdentifier key0(idx,0);
      InputBoundnaryAssignmentIdentifier key1(idx,1);
      InputBoundnaryAssignmentIdentifier key2(idx,2);

      tp0 = cntlines->GetElement( key0 );
      tp1 = cntlines->GetElement( key1 );
      tp2 = cntlines->GetElement( key2 );

      CreateEdgeForTrianglePair( idx, tp0 );
      CreateEdgeForTrianglePair( idx, tp1 );
      CreateEdgeForTrianglePair( idx, tp2 );

      points++;
      }
    }

  template <typename TInputMesh, typename TOutputMesh>
    void TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>
    ::CreateNewEdge(unsigned long currentCellId, unsigned int featureId, 
    unsigned long startPointId, unsigned long endPointId )
    {
    unsigned long boundaryId;

    InputMeshPointer input = this->GetInput(0);

    EdgeIdentifierType edge = std::make_pair(startPointId,endPointId);
    EdgeIdentifierType edgeInv = std::make_pair(endPointId,startPointId);

    if (!m_Edges->IndexExists(edge) && !m_Edges->IndexExists(edgeInv))
      {
      boundaryId = m_IdOffset+m_EdgeCellId;
      m_Edges->InsertElement(edge, boundaryId);
      NewInputMeshCellPointer.TakeOwnership( new LineType );
      NewInputMeshCellPointer->SetPointId(0, startPointId);
      NewInputMeshCellPointer->SetPointId(1, endPointId);
      input->SetCell( boundaryId, NewInputMeshCellPointer );
      m_EdgeCellId++;

      }
    else 
      {
      if (m_Edges->IndexExists(edge)) { boundaryId = m_Edges->GetElement(edge); }
      else { boundaryId = m_Edges->GetElement(edgeInv); }
      }
    input->SetBoundaryAssignment( 1,currentCellId, featureId , boundaryId);


    if (!m_EdgeNeighborList->IndexExists(boundaryId))
      {
      EdgeIdentifierType neighboringCells = std::make_pair(currentCellId, (unsigned long) NumericTraits<unsigned long>::max() );
      m_EdgeNeighborList->InsertElement(boundaryId, neighboringCells);
      }
    else 
      {
      EdgeIdentifierType neighboringCells = m_EdgeNeighborList->GetElement(boundaryId);

      if (neighboringCells.second == (unsigned long) NumericTraits<unsigned long>::max())
        {
        neighboringCells.second = currentCellId;
        }
      m_EdgeNeighborList->InsertElement(boundaryId, neighboringCells);
      }

    if (!m_VertexNeighborList->IndexExists(startPointId))
      {
      std::set<unsigned long> neighborEdges;
      neighborEdges.insert(boundaryId);
      m_VertexNeighborList->InsertElement(startPointId,neighborEdges);
      }
    else 
      {
      std::set<unsigned long> neighborEdges = m_VertexNeighborList->GetElement(startPointId);
      neighborEdges.insert(boundaryId);
      m_VertexNeighborList->InsertElement(startPointId,neighborEdges);
      }

    if (!m_VertexNeighborList->IndexExists(endPointId))
      {
      std::set<unsigned long> neighborEdges;
      neighborEdges.insert( boundaryId );
      m_VertexNeighborList->InsertElement( endPointId, neighborEdges );
      }
    else 
      {
      std::set<unsigned long> neighborEdges = m_VertexNeighborList->GetElement(endPointId);
      neighborEdges.insert(boundaryId);
      m_VertexNeighborList->InsertElement( endPointId, neighborEdges );
      }

    }



  /* PrintSelf. */
  template <typename TInputMesh, typename TOutputMesh>
    void
    TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>
    ::PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os,indent);
    os << indent << "ToDo: implement PrinSelf!!!";    
    }

  template <typename TInputMesh, typename TOutputMesh>
    void
    TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>
    ::CreateCells()
    {

    InputPointsContainerPointer pointsContainer =  this->GetInput(0)->GetPoints();
    InputPointsContainerIterator points =  pointsContainer->Begin();

    unsigned long idx;
    typedef itk::MapContainer<unsigned long, unsigned long> MapType;

    while( points != pointsContainer->End() ) {

      idx = points.Index();
      IndexSetType vertexNeighbors = m_VertexNeighborList->GetElement(idx);

      IndexSetType::iterator iterator1 = vertexNeighbors.begin();

      MapType::Pointer tmpMap = MapType::New();
      unsigned long startIdx = (unsigned long)NumericTraits<unsigned long>::max(),
        lastIdx = 0, 
        wrongIdx = 0;


      while ( lastIdx != startIdx )
        {
        if (startIdx == (unsigned long)NumericTraits<unsigned long>::max() )
          {
          EdgeIdentifierType neighboringCells = m_EdgeNeighborList->GetElement(*iterator1);
          startIdx = neighboringCells.first;
          tmpMap->InsertElement(neighboringCells.first,neighboringCells.second);

          lastIdx = neighboringCells.second;
          wrongIdx = neighboringCells.first;
          }
        else 
          {

          IndexSetType::iterator iterator2 = vertexNeighbors.begin();
          while ( iterator2 != vertexNeighbors.end() )
            {
            EdgeIdentifierType compare = m_EdgeNeighborList->GetElement(*iterator2);
            if (compare.first == lastIdx && compare.second != wrongIdx)
              {
              tmpMap->InsertElement(compare.first,compare.second);
              lastIdx = compare.second;
              wrongIdx = compare.first; 
              break;
              }
            else if (compare.second == lastIdx && compare.first != wrongIdx)
              {
              tmpMap->InsertElement(compare.second,compare.first);
              lastIdx = compare.first;
              wrongIdx = compare.second;    
              break;
              }
            iterator2++;
            }
          }
        iterator1++;
        }

      // create a new cell
      NewSimplexCellPointer.TakeOwnership( new OutputPolygonType );
      unsigned long vertexIdx = 0;
      unsigned long nextIdx = startIdx;
      unsigned long featureId = 0;

      unsigned long faceIndex = this->GetOutput()->AddFace( NewSimplexCellPointer );

      while ( tmpMap->IndexExists(nextIdx) ){
        NewSimplexCellPointer->SetPointId( vertexIdx++, nextIdx);
        unsigned long newIdx = tmpMap->GetElement(nextIdx);

        EdgeIdentifierType line = std::make_pair(nextIdx,newIdx);
        EdgeIdentifierType lineInv = std::make_pair(newIdx,nextIdx);

        unsigned long edgeIdx = 0;

        if (m_LineCellIndices->IndexExists(line) )
          {
          edgeIdx = m_LineCellIndices->GetElement(line);
          }
        else if (m_LineCellIndices->IndexExists(lineInv))
          {
          edgeIdx = m_LineCellIndices->GetElement(lineInv);        
          }
        else
          {
          std::cout << "error!!! " << std::endl;
          }
        this->GetOutput()->SetBoundaryAssignment( 1, faceIndex, featureId++, edgeIdx);

        if (newIdx == startIdx) { 
          break;
          }
        else { nextIdx = newIdx; }
      featureId++;
        }
    points++;
      }
    }

  template <typename TInputMesh, typename TOutputMesh>
    typename TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>::InputPointType 
    TriangleMeshToSimplexMeshFilter<TInputMesh, TOutputMesh>
    ::ComputeFaceCenter(unsigned long faceId)
    {
    InputMeshPointer input = this->GetInput(0);

    const unsigned long *tp;
    InputPointType v1, v2, v3;

    CellAutoPointer cellPointer;
    input->GetCell( faceId, cellPointer );
    tp = cellPointer->GetPointIds();
    input->GetPoint( tp[0],&v1 );
    input->GetPoint( tp[1],&v2 );
    input->GetPoint( tp[2],&v3 );

    InputPointType result;
    result[0] = ((double)(v1[0] + v2[0] + v3[0]))/3.0;
    result[1] = ((double)(v1[1] + v2[1] + v3[1]))/3.0;
    result[2] = ((double)(v1[2] + v2[2] + v3[2]))/3.0;

    return result;
    }

  } // end of namspace itk

#endif //_TRIANGLE_MESH_TO_SIMPLEX_MESH_FILTER_TXX

