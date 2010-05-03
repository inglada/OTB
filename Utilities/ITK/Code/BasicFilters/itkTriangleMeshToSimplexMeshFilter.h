/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkTriangleMeshToSimplexMeshFilter.h,v $
Language:  C++
Date:      $Date: 2009-09-17 11:14:56 $
Version:   $Revision: 1.7 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTriangleMeshToSimplexMeshFilter_h
#define __itkTriangleMeshToSimplexMeshFilter_h

#include <itkMesh.h>
#include <itkLineCell.h>
#include <itkVertexCell.h>
#include <itkMapContainer.h>
#include "itkSimplexMesh.h"
#include "itkSimplexMeshGeometry.h"
#include "itkMeshToMeshFilter.h"
#include "itkVectorContainer.h"
#include "itkNumericTraits.h"

#include <itkPolygonCell.h>
namespace itk
{
/**
 * \class TriangleMeshToSimplexMeshFilter
 * \brief This filter converts a triangle mesh into a 2-simplex mesh.
 * 
 * The triangle cell centers build the points of the dual simplex mesh
 * each center is connected with the center of each neighboring triangle.
 * This creates the simplex structure. 
 *
 * Finally the neighbors of the points are reordered counter-clockwise
 * for geometry computation.
 *
 * \author Thomas Boettger. Division Medical and Biological Informatics, German Cancer Research Center, Heidelberg.
 * 
 */
template <class TInputMesh, class TOutputMesh>
class ITK_EXPORT TriangleMeshToSimplexMeshFilter : public MeshToMeshFilter<TInputMesh, TOutputMesh>
{
public:
  /** Standard "Self" typedef. */
  typedef TriangleMeshToSimplexMeshFilter  Self;

  /** Standard "Superclass" typedef. */
  typedef MeshToMeshFilter<TInputMesh, TOutputMesh> Superclass;

  /** Smart pointer typedef support */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method of creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TriangleMeshToSimplexMeshFilter,MeshToMeshFilter);

  typedef TInputMesh                     InputMeshType;
  typedef typename TInputMesh::Pointer   InputMeshPointer;
  typedef typename TInputMesh::PointType InputPointType;

  typedef typename TInputMesh::BoundaryAssignmentsContainerPointer InputBoundaryAssignmentsContainerPointer;

  typedef typename TInputMesh::PointsContainer          InputPointsContainer;
  typedef typename InputPointsContainer::Pointer        InputPointsContainerPointer;
  typedef typename InputPointsContainer::Iterator       InputPointsContainerIterator;
  typedef typename InputPointsContainer::ConstIterator  InputPointsContainerConstIterator;

  typedef typename TOutputMesh::Pointer                   OutputMeshPointer;
  typedef typename TOutputMesh::PointType                 OutputPointType;
  typedef typename TOutputMesh::PixelType                 OutputPixelType;
  typedef typename TOutputMesh::PointsContainer           OutputPointsContainer;
  typedef typename OutputPointsContainer::Pointer         OutputPointsContainerPointer;
  typedef typename TOutputMesh::PointsContainer::Iterator OutputPointsContainerIterator;

  typedef typename TInputMesh::BoundaryAssignmentIdentifier InputBoundnaryAssignmentIdentifier;

  typedef typename TInputMesh::CellType             InputCellType;
  typedef typename InputCellType::CellAutoPointer   InputCellAutoPointer;
  typedef typename TInputMesh::CellAutoPointer      CellAutoPointer;
  typedef          itk::LineCell<InputCellType>     LineType;
  typedef          itk::PolygonCell<InputCellType>  PolygonType;
  typedef          itk::TriangleCell<InputCellType> TriangleType;
  typedef          itk::VertexCell<InputCellType>   VertexType;

  typedef          std::pair<unsigned long,unsigned long> EdgeIdentifierType;
  typedef          std::set<unsigned long>                IndexSetType;

  typedef          itk::MapContainer<unsigned long, EdgeIdentifierType> EdgeNeighborListType;
  typedef          itk::MapContainer<EdgeIdentifierType, unsigned long> LineCellIndexType;

  typedef          itk::MapContainer<unsigned long, IndexSetType>       VertexNeighborListType;
    typedef          itk::MapContainer<EdgeIdentifierType,unsigned long>  EdgeMapType;
  typedef typename EdgeMapType::Pointer                                 EdgeMapPointer;

  typedef          itk::VectorContainer<unsigned long,unsigned long>    IdVectorType;
  typedef typename IdVectorType::Pointer                                IdVectorPointer;

  typedef typename TOutputMesh::CellType             OutputCellType;
  typedef typename TOutputMesh::CellAutoPointer      OutputCellAutoPointer;
  typedef          itk::LineCell<OutputCellType>     OutputLineType;
  typedef          itk::PolygonCell<OutputCellType>  OutputPolygonType;

protected:

  TriangleMeshToSimplexMeshFilter();
  ~TriangleMeshToSimplexMeshFilter();
  TriangleMeshToSimplexMeshFilter(const Self&) {}

  void operator=(const Self&) {}

  void PrintSelf(std::ostream& os, Indent indent) const;

  /** 
   * Override from ProcessObject
   */
  virtual void GenerateData();

  /** 
   * Initializes all necessary datastructures
   */
  void Initialize();

  /** 
   * Method inserts the new computed simplex points into the output mesh
   */
  void CreateSimplexPoints();

  /** 
   * Method creates a new edge, which from the centers of 
   * two neighboring triangles of the input mesh over the 
   * edge the both triangles have in common.
   */
  void CreateEdgeForTrianglePair( unsigned long pointIndex, unsigned long boundaryId);

  /** 
   * Constructs the neighborhood relations for all simplex mesh points
   * It also reorders the neighbors for easy normals computation
   */
  void CreateSimplexNeighbors();

  /**
   * This method creates all the cells of the dual simplex mesh
   */
  void CreateCells();

  /**
   * \brief add edge cells to the input mesh
   */
  void CreateNewEdge(unsigned long currentCellId, unsigned int featureId, 
    unsigned long startPointId, unsigned long endPointId);

  /**
   *  Computes the center of a face
   */
  InputPointType ComputeFaceCenter(unsigned long faceId);

  /**
   * \brief stores all faces (triangles) of the input mesh
   */
  IndexSetType* m_FaceSet;

  /**
   * \brief stores all edges of the input mesh. 
   * 
   * the key is the index of the edge cell, and the element is the 
   * index pair of the two points of the edge.
   */
  EdgeMapPointer m_Edges;

  /**
   * \brief stores the two neighboring cells (faces) of an edge
   */
  EdgeNeighborListType::Pointer m_EdgeNeighborList;

  /**
   * \brief stores all edges starting from a vertex
   */
  VertexNeighborListType::Pointer m_VertexNeighborList;

  /**
   * stores line indices for conversion algorithm
   */
  LineCellIndexType::Pointer m_LineCellIndices;

  /**
   * offset for ids of new simplex polygon cells
   */
  unsigned long m_CellIdxOffset;

  /**
   * offset for point ids
   */
  unsigned long m_IdOffset;

  /**
   * offset for edge cell ids
   */
  unsigned long m_EdgeCellId;

  /**
   * stores algorithmic data
   */
  IdVectorPointer m_HandledEdgeIds;

  /** 
   * autopointer definition for creation of new cells in the input mesh
   */
  InputCellAutoPointer m_NewInputMeshCellPointer;

  /** 
   * autopointer definition for creation of new simplex cells
   */
  OutputCellAutoPointer  m_NewSimplexCellPointer;
};

} //end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTriangleMeshToSimplexMeshFilter.txx"
#endif

#endif
