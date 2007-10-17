/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVertexCell.h,v $
  Language:  C++
  Date:      $Date: 2007/05/18 16:53:48 $
  Version:   $Revision: 1.55 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVertexCell_h
#define __itkVertexCell_h

#include "itkCellInterface.h"
#include "itkNumericTraits.h"

namespace itk
{

/** \class VertexCell
 * VertexCell represents a single vertex for a Mesh.
 *
 * Template parameters for VertexCell:
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
class ITK_EXPORT VertexCell: public TCellInterface
{
public:
  /** Standard class typedefs. */
  itkCellCommonTypedefs(VertexCell);
  itkCellInheritedTypedefs(TCellInterface);

  /** Standard part of every itk Object. */
  itkTypeMacro(VertexCell, CellInterface);
 
  /** Vertex-specific topology numbers. */
  itkStaticConstMacro(NumberOfPoints, unsigned int, 1);
  itkStaticConstMacro(CellDimension, unsigned int, 0);
  
  /** Implement the standard CellInterface. */
  virtual CellGeometry GetType(void) const 
    {return Superclass::VERTEX_CELL;}
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
    
  /** Vertex-specific interface. */
  virtual void SetPointId(PointIdentifier);
  virtual PointIdentifier GetPointId(void);
  
  /** Cell visitor interface */
  itkCellVisitMacro(Superclass::VERTEX_CELL);

  /** Evaluate the position of a given point */
  virtual bool EvaluatePosition(CoordRepType*,
                                PointsContainer* ,
                                CoordRepType*,
                                CoordRepType [],
                                double *,
                                InterpolationWeightType*);
public:
  VertexCell()
    {
    for (unsigned int i = 0; i < itkGetStaticConstMacro(NumberOfPoints); i++)
      {
      m_PointIds[i] = NumericTraits<unsigned long>::max();
      }
    }
  ~VertexCell() {}

protected:
  /**
   * Store the number of points needed for a vertex.
   */
  PointIdentifier m_PointIds[NumberOfPoints];

private:
  VertexCell(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVertexCell.txx"
#endif

#endif
