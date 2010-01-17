/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadraticEdgeCell.h,v $
  Language:  C++
  Date:      $Date: 2007-05-18 14:31:12 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuadraticEdgeCell_h
#define __itkQuadraticEdgeCell_h

#include "itkCellInterface.h"
#include "itkVertexCell.h"

namespace itk
{

/** \class QuadraticEdgeCell
 * QuadraticEdgeCell represents a second order line segment for a Mesh.
 *
 * Template parameters for QuadraticEdgeCell:
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
class ITK_EXPORT QuadraticEdgeCell: public TCellInterface
{
public:
  /** Standard class typedefs. */
  itkCellCommonTypedefs(QuadraticEdgeCell);
  itkCellInheritedTypedefs(TCellInterface);
  
  /** Standard part of every itk Object. */
  itkTypeMacro(QuadraticEdgeCell, CellInterface);

  /** The type of boundary for this lines's vertices. */
  typedef VertexCell< TCellInterface >             VertexType;
  typedef typename VertexType::SelfAutoPointer     VertexAutoPointer;
    
  /** QuadraticEdge-specific topology numbers. */
  itkStaticConstMacro(NumberOfPoints, unsigned int, 3);
  itkStaticConstMacro(NumberOfVertices, unsigned int, 2);
  itkStaticConstMacro(CellDimension, unsigned int, 1);
  
  /** Implement the standard CellInterface. */
  virtual CellGeometry GetType(void) const 
  {return Superclass::QUADRATIC_EDGE_CELL;}
  virtual void MakeCopy( CellAutoPointer & ) const;
  virtual unsigned int GetDimension(void) const;
  virtual unsigned int GetNumberOfPoints(void) const;
  virtual CellFeatureCount GetNumberOfBoundaryFeatures(int dimension) const;
  virtual bool GetBoundaryFeature(int dimension, CellFeatureIdentifier,CellAutoPointer &);
  virtual void SetPointIds(PointIdConstIterator first);
  virtual void SetPointIds(PointIdConstIterator first,
                           PointIdConstIterator last);
  virtual void SetPointId(int localId, PointIdentifier);
  virtual PointIdIterator      PointIdsBegin(void);
  virtual PointIdConstIterator PointIdsBegin(void) const;
  virtual PointIdIterator      PointIdsEnd(void);
  virtual PointIdConstIterator PointIdsEnd(void) const; 
  
  /** QuadraticEdge-specific interface. */
  virtual CellFeatureCount GetNumberOfVertices(void) const;
  virtual bool GetVertex(CellFeatureIdentifier, VertexAutoPointer &);
  
  /** Visitor interface */
  itkCellVisitMacro(Superclass::QUADRATIC_EDGE_CELL);

  QuadraticEdgeCell()
    {
    for (unsigned int i = 0; i < itkGetStaticConstMacro(NumberOfPoints); i++)
      {
      m_PointIds[i] = NumericTraits<unsigned long>::max();
      }
    }
  ~QuadraticEdgeCell() {}

  /** Given the parametric coordinates of a point in the cell
   * returned the values of its ShapeFunctions */
  virtual void EvaluateShapeFunctions( 
                          const ParametricCoordArrayType & parametricCoordinates,
                                ShapeFunctionsArrayType  & weights) const;


protected:
  /** Store number of points needed for a line segment. */
  PointIdentifier m_PointIds[NumberOfPoints];

private:
  QuadraticEdgeCell(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkQuadraticEdgeCell.txx"
#endif

#endif
