/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTriangleCell.h,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:11:01 $
  Version:   $Revision: 1.62 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTriangleCell_h
#define __itkTriangleCell_h

#include "itkCellInterface.h"
#include "itkLineCell.h"
#include "itkTriangleCellTopology.h"

#include <vector>

namespace itk
{

/** \class TriangleCell
 * TriangleCell represents a triangle for a Mesh.
 *
 * Template parameters for TriangleCell:
 *
 * TPixelType =
 *     The type associated with a point, cell, or boundary for use in storing
 *     its data.
 *
 * TCellTraits =
 *     Type information of mesh containing cell.
 *
 * \ingroup MeshObjects
 */

template < typename TCellInterface >
class ITK_EXPORT TriangleCell:
  public TCellInterface, private TriangleCellTopology
{
public:
  /** Standard class typedefs. */
  itkCellCommonTypedefs(TriangleCell);
  itkCellInheritedTypedefs(TCellInterface);

  /** Standard part of every itk Object. */
  itkTypeMacro(TriangleCell, CellInterface);
  
  /** The type of boundary for this triangle's vertices. */
  typedef VertexCell< TCellInterface >            VertexType;
  typedef typename VertexType::SelfAutoPointer    VertexAutoPointer;
  
  /** The type of boundary for this triangle's edges. */
  typedef LineCell< TCellInterface >              EdgeType;
  typedef typename EdgeType::SelfAutoPointer      EdgeAutoPointer;
    
  /** Triangle-specific topology numbers. */
  itkStaticConstMacro(NumberOfPoints, unsigned int, 3);
  itkStaticConstMacro(NumberOfVertices, unsigned int, 3);
  itkStaticConstMacro(NumberOfEdges, unsigned int, 3);
  itkStaticConstMacro(CellDimension, unsigned int, 2);
  
  /** Implement the standard CellInterface. */
  virtual CellGeometry GetType(void) const 
    {return Superclass::TRIANGLE_CELL;}
  virtual void MakeCopy( CellAutoPointer & ) const;
  virtual unsigned int GetDimension(void) const;
  virtual unsigned int GetNumberOfPoints(void) const;
  virtual CellFeatureCount GetNumberOfBoundaryFeatures(int dimension) const;
  virtual bool GetBoundaryFeature(int dimension, CellFeatureIdentifier,
    CellAutoPointer &);
  virtual void SetPointIds(PointIdConstIterator first);
  virtual void SetPointIds(PointIdConstIterator first,
                           PointIdConstIterator last);
  virtual void SetPointId(int localId, PointIdentifier);
  virtual PointIdIterator      PointIdsBegin(void);
  virtual PointIdConstIterator PointIdsBegin(void) const;
  virtual PointIdIterator      PointIdsEnd(void);
  virtual PointIdConstIterator PointIdsEnd(void) const; 
  
  /** Triangle-specific interface. */
  virtual CellFeatureCount GetNumberOfVertices(void) const;
  virtual CellFeatureCount GetNumberOfEdges(void) const;
  virtual bool GetVertex(CellFeatureIdentifier, VertexAutoPointer &);
  virtual bool GetEdge(CellFeatureIdentifier, EdgeAutoPointer &);
  
  virtual bool EvaluatePosition(CoordRepType*,
                                PointsContainer* ,
                                CoordRepType*,
                                CoordRepType [],
                                double *,
                                InterpolationWeightType*);

  /** Cell visitor interface. */
  itkCellVisitMacro(Superclass::TRIANGLE_CELL);


  /** \brief Compute Area to a TriangleCell given a PointsContainer.  */
  CoordRepType ComputeArea( PointsContainer* );
  PointType ComputeBarycenter( CoordRepType*,
                               PointsContainer* );

  PointType ComputeCenterOfGravity( PointsContainer* );
  PointType ComputeCircumCenter( PointsContainer* );

public:
  TriangleCell() :
    m_PointIds( NumberOfPoints, NumericTraits<PointIdentifier>::max() )
    {}
  ~TriangleCell() {}

protected:
  /** Store the number of points needed for a triangle. */
  std::vector< PointIdentifier > m_PointIds;

private:
  TriangleCell(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Computes the SQUARED distance between a point and a line segment defined
   * by two other points */
  double DistanceToLine(PointType x, PointType p1, PointType p2, 
                              double &t, CoordRepType *closestPoint);
  double DistanceToLine(PointType x, PointType p1, PointType p2, 
                              double &t, PointType &closestPoint);

};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTriangleCell.txx"
#endif

#endif
