/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTetrahedronCell.h,v $
  Language:  C++
  Date:      $Date: 2007/05/18 14:31:13 $
  Version:   $Revision: 1.54 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTetrahedronCell_h
#define __itkTetrahedronCell_h

#include "itkCellInterface.h"
#include "itkTriangleCell.h"
#include "itkTetrahedronCellTopology.h"

namespace itk
{

/** \class TetrahedronCell
 * TetrahedronCell represents a tetrahedron for a Mesh.
 *
 * Template parameters for TetrahedronCell:
 *
 * TPixelType =
 *     The type associated with a point, cell, or boundary for use in storing
 *     its data.
 *
 * TCellTraits =
 *     Type information of mesh containing cell.
 * \ingroup MeshObjects
 */
template < typename TCellInterface >
class ITK_EXPORT TetrahedronCell: public TCellInterface, private TetrahedronCellTopology
{
public:
  /** Standard class typedefa. */
  itkCellCommonTypedefs(TetrahedronCell);
  itkCellInheritedTypedefs(TCellInterface);
  
  /** Standard part of every itk Object. */
  itkTypeMacro(TetrahedronCell, CellInterface);

  /** The type of boundary for this triangle's vertices. */
  typedef VertexCell< TCellInterface >            VertexType;
  typedef typename VertexType::SelfAutoPointer    VertexAutoPointer;
  
  /** The type of boundary for this triangle's edges. */
  typedef LineCell< TCellInterface >              EdgeType;
  typedef typename EdgeType::SelfAutoPointer      EdgeAutoPointer;
  
  /** The type of boundary for this hexahedron's faces. */
  typedef TriangleCell< TCellInterface >          FaceType;
  typedef typename FaceType::SelfAutoPointer      FaceAutoPointer;
 
  /** Tetrahedron-specific topology numbers. */
  itkStaticConstMacro(NumberOfPoints, unsigned int, 4);
  itkStaticConstMacro(NumberOfVertices, unsigned int, 4);
  itkStaticConstMacro(NumberOfEdges, unsigned int, 6);
  itkStaticConstMacro(NumberOfFaces, unsigned int, 4);
  itkStaticConstMacro(CellDimension, unsigned int, 3);

  /** Implement the standard CellInterface. */
  virtual CellGeometry GetType(void) const 
    {return Superclass::TETRAHEDRON_CELL;}
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
  
  /** Tetrahedron-specific interface. */
  virtual CellFeatureCount GetNumberOfVertices(void) const;
  virtual CellFeatureCount GetNumberOfEdges(void) const;
  virtual CellFeatureCount GetNumberOfFaces(void) const;
  virtual bool  GetVertex(CellFeatureIdentifier, VertexAutoPointer &);
  virtual bool  GetEdge(CellFeatureIdentifier, EdgeAutoPointer &);  
  virtual bool  GetFace(CellFeatureIdentifier, FaceAutoPointer &);  
  
  /** Visitor interface. */
  itkCellVisitMacro(Superclass::TETRAHEDRON_CELL);
  
  virtual bool EvaluatePosition(CoordRepType* ,
                                PointsContainer* ,
                                CoordRepType* ,
                                CoordRepType [],
                                double *,
                                InterpolationWeightType*);

 public:
  TetrahedronCell()
    {
    for (unsigned int i = 0; i < itkGetStaticConstMacro(NumberOfPoints); i++)
      {
      m_PointIds[i] = NumericTraits<unsigned long>::max();
      }
    }
  ~TetrahedronCell() {}

 protected:
  /** Store the number of points needed for a tetrahedron. */
  PointIdentifier m_PointIds[NumberOfPoints];
  
 private:
  TetrahedronCell(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  };

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTetrahedronCell.txx"
#endif

#endif
