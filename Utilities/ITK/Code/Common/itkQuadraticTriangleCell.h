/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadraticTriangleCell.h,v $
  Language:  C++
  Date:      $Date: 2007-05-18 14:31:12 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuadraticTriangleCell_h
#define __itkQuadraticTriangleCell_h

#include "itkCellInterface.h"
#include "itkQuadraticEdgeCell.h"
#include "itkQuadraticTriangleCellTopology.h"

namespace itk
{

/** \class QuadraticTriangleCell
 * QuadraticTriangleCell represents a second order triangular patch for a Mesh.
 *
 * Template parameters for QuadraticTriangleCell:
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
class QuadraticTriangleCell: public TCellInterface, private QuadraticTriangleCellTopology
{
public:
  /** Standard class typedefs. */
  itkCellCommonTypedefs(QuadraticTriangleCell);
  itkCellInheritedTypedefs(TCellInterface);

  /** Standard part of every itk Object. */
  itkTypeMacro(QuadraticTriangleCell, CellInterface);
  
  /** The type of boundary for this triangle's vertices. */
  typedef VertexCell< TCellInterface >          VertexType;
  typedef typename VertexType::SelfAutoPointer  VertexAutoPointer;
  
  /** The type of boundary for this triangle's edges. */
  typedef QuadraticEdgeCell< TCellInterface >   EdgeType;
  typedef typename EdgeType::SelfAutoPointer    EdgeAutoPointer;
    
  /** Triangle-specific topology numbers. */
  itkStaticConstMacro(NumberOfPoints, unsigned int, 6);
  itkStaticConstMacro(NumberOfVertices, unsigned int, 3);
  itkStaticConstMacro(NumberOfEdges, unsigned int, 3);
  itkStaticConstMacro(CellDimension, unsigned int, 2);
  
  /** Implement the standard CellInterface. */
  virtual CellGeometry GetType(void) const 
    {return Superclass::QUADRATIC_TRIANGLE_CELL;}
  virtual void MakeCopy( CellAutoPointer & ) const;
  virtual unsigned int GetDimension(void) const;
  virtual unsigned int GetNumberOfPoints(void) const;
  virtual CellFeatureCount GetNumberOfBoundaryFeatures(int dimension) const;
  virtual bool GetBoundaryFeature(int dimension, CellFeatureIdentifier, CellAutoPointer &);
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
  virtual bool  GetVertex(CellFeatureIdentifier, VertexAutoPointer &);
  virtual bool  GetEdge(CellFeatureIdentifier, EdgeAutoPointer &);
  
  /** Cell visitor interface. */
  itkCellVisitMacro(Superclass::QUADRATIC_TRIANGLE_CELL);


  /** Given the parametric coordinates of a point in the cell
   *  determine the value of its Shape Functions
   *  returned through an itkArray<InterpolationWeightType>).  */
  virtual void EvaluateShapeFunctions( 
                          const ParametricCoordArrayType & parametricCoordinates,
                                ShapeFunctionsArrayType  & weights) const;



 public:
  QuadraticTriangleCell()
    {
    for (unsigned int i = 0; i < itkGetStaticConstMacro(NumberOfPoints); i++)
      {
      m_PointIds[i] = NumericTraits<unsigned long>::max();
      }
    }
  ~QuadraticTriangleCell() {}

 protected:
  /** Store the number of points needed for a triangle. */
  PointIdentifier m_PointIds[NumberOfPoints];

 private:
  QuadraticTriangleCell(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkQuadraticTriangleCell.txx"
#endif

#endif
