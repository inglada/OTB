/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHexahedronCell.h,v $
  Language:  C++
  Date:      $Date: 2007/05/18 14:31:11 $
  Version:   $Revision: 1.49 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHexahedronCell_h
#define __itkHexahedronCell_h

#include "itkCellInterface.h"
#include "itkQuadrilateralCell.h"
#include "itkHexahedronCellTopology.h"

namespace itk
{

/** \class HexahedronCell
 * HexahedronCell represents a hexahedron for a Mesh.
 *
 * Template parameters for HexahedronCell:
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
class ITK_EXPORT HexahedronCell: public TCellInterface, private HexahedronCellTopology
{
public:
  /** Standard class typedefs. */
  itkCellCommonTypedefs(HexahedronCell);
  itkCellInheritedTypedefs(TCellInterface);

  /** Standard part of every itk Object. */
  itkTypeMacro(HexahedronCell, CellInterface);

  /** The type of boundary for this triangle's vertices. */
  typedef VertexCell< TCellInterface >            VertexType;
  typedef typename VertexType::SelfAutoPointer    VertexAutoPointer;
  
  /** The type of boundary for this triangle's edges. */
  typedef LineCell< TCellInterface >              EdgeType;
  typedef typename EdgeType::SelfAutoPointer      EdgeAutoPointer;
  
  /** The type of boundary for this hexahedron's faces. */
  typedef QuadrilateralCell< TCellInterface >     FaceType;
  typedef typename FaceType::SelfAutoPointer      FaceAutoPointer;
    
  /** Hexahedron-specific topology numbers. */
  enum { NumberOfPoints   =  8,
         NumberOfVertices =  8,
         NumberOfEdges    = 12,
         NumberOfFaces    =  6,
         CellDimension    =  3 };

  /** Implement the standard CellInterface. */
  virtual CellGeometry GetType(void) const 
    {return Superclass::HEXAHEDRON_CELL;}
  virtual void MakeCopy( CellAutoPointer & ) const;
  virtual unsigned int GetDimension(void) const;
  virtual unsigned int GetNumberOfPoints(void) const;
  virtual CellFeatureCount GetNumberOfBoundaryFeatures(int dimension) const;
  virtual bool GetBoundaryFeature(int dimension, CellFeatureIdentifier, CellAutoPointer &);
  virtual void SetPointIds(PointIdConstIterator first);
  virtual void SetPointIds(PointIdConstIterator first, PointIdConstIterator last);
  virtual void SetPointId(int localId, PointIdentifier);
  virtual PointIdIterator      PointIdsBegin(void);
  virtual PointIdConstIterator PointIdsBegin(void) const;
  virtual PointIdIterator      PointIdsEnd(void);
  virtual PointIdConstIterator PointIdsEnd(void) const; 
  
  /** Hexahedron-specific interface. */
  virtual CellFeatureCount GetNumberOfVertices(void) const;
  virtual CellFeatureCount GetNumberOfEdges(void) const;
  virtual CellFeatureCount GetNumberOfFaces(void) const;
  virtual bool GetVertex(CellFeatureIdentifier, VertexAutoPointer &);
  virtual bool GetEdge(CellFeatureIdentifier, EdgeAutoPointer &);  
  virtual bool GetFace(CellFeatureIdentifier, FaceAutoPointer &);  
  
  /** Evaluate the position inside the cell */
  virtual bool EvaluatePosition(CoordRepType* ,
                                PointsContainer* ,
                                CoordRepType* ,
                                CoordRepType [],
                                double *,
                                InterpolationWeightType*);

  /** Visitor interface */
  itkCellVisitMacro(Superclass::HEXAHEDRON_CELL);

protected:
  /** Store the number of points needed for a hexahedron. */
  PointIdentifier m_PointIds[NumberOfPoints];

  void InterpolationDerivs(CoordRepType pcoords[3], CoordRepType derivs[24]);
  void InterpolationFunctions(CoordRepType pcoords[3], InterpolationWeightType sf[8]);
  void EvaluateLocation(int& itkNotUsed(subId), PointsContainer* points, CoordRepType pcoords[3],
                                     CoordRepType x[3], InterpolationWeightType *weights);
  
public:
  HexahedronCell()
    {
    for (unsigned int i = 0; i < itkGetStaticConstMacro(NumberOfPoints); i++)
      {
      m_PointIds[i] = NumericTraits<unsigned long>::max();
      }
    }
  ~HexahedronCell() {}
  
private:
  HexahedronCell(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented  
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHexahedronCell.txx"
#endif

#endif
