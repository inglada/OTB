/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshLineCell.h,v $
  Language:  C++
  Date:      $Date: 2007/07/26 06:30:26 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __itkQuadEdgeMeshLineCell_h
#define __itkQuadEdgeMeshLineCell_h

#include "itkAutoPointer.h"
#include "itkGeometricalQuadEdge.h"

namespace itk
{

/**
 * \class QuadEdgeMeshLineCell
 *
 * \brief Class that connects the QuadEdgeMesh with the Mesh
 *
 * \param TCellInterface Basic type for the itk*Cell. 
 *        This usually comes from the MeshTraits.
 *
 * \author  Eric Boix, Alex Gouaillard, Leonardo Florez  
 *
 */
template< class TCellInterface >
class QuadEdgeMeshLineCell 
  : public TCellInterface, public TCellInterface::CellTraits::QuadEdgeType
{
public:
  /** Standard class typedefs. */
  // itkCellCommonTypedefs
  typedef QuadEdgeMeshLineCell      Self;
  typedef AutoPointer< const Self > ConstSelfAutoPointer;
  typedef AutoPointer< Self >       SelfAutoPointer;
  typedef Self *                    RawPointer;
  typedef const Self *              ConstRawPointer;

  // itkCellInheritedTypedefs
  typedef TCellInterface                                Superclass;
  typedef typename Superclass::PixelType                PixelType;
  typedef typename Superclass::CellType                 CellType;
  typedef typename Superclass::CellAutoPointer          CellAutoPointer;
  typedef typename Superclass::CellConstAutoPointer     CellConstAutoPointer;
  typedef typename Superclass::CellRawPointer           CellRawPointer;
  typedef typename Superclass::CellConstRawPointer      CellConstRawPointer;
  typedef typename Superclass::CellTraits               CellTraits;
  typedef typename Superclass::CoordRepType             CoordRepType;
  typedef typename Superclass::InterpolationWeightType  InterpolationWeightType;
  typedef typename Superclass::PointIdentifier          PointIdentifier;
  typedef typename Superclass::CellIdentifier           CellIdentifier;
  typedef typename Superclass::CellFeatureIdentifier    CellFeatureIdentifier;
  typedef typename Superclass::CellFeatureIdentifier    CellFeatureCount;
  typedef typename Superclass::PointType                PointType;
  typedef typename Superclass::PointsContainer          PointsContainer;
  typedef typename Superclass::UsingCellsContainer      UsingCellsContainer;
  typedef typename Superclass::CellGeometry             CellGeometry;
  typedef typename Superclass::ParametricCoordArrayType ParametricCoordArrayType;
  typedef typename Superclass::ShapeFunctionsArrayType  ShapeFunctionsArrayType;
  itkStaticConstMacro( PointDimension, unsigned int, Superclass::PointDimension );
  itkStaticConstMacro( CellDimension, unsigned int, 2 );

  /** Multivisitor type. */
  typedef typename CellType::MultiVisitor MultiVisitor;
  
  //** */
  typedef typename CellTraits::PointIdIterator              PointIdIterator;
  typedef typename CellTraits::PointIdConstIterator         PointIdConstIterator;
  typedef typename CellTraits::PointIdInternalIterator      PointIdInternalIterator;
  typedef typename CellTraits::PointIdInternalConstIterator PointIdInternalConstIterator;

  /** QE types. */
  typedef typename CellTraits::QuadEdgeType              QEType;
  typedef typename QEType::OriginRefType                 VertexRefType;
  typedef typename QEType::DualOriginRefType             FaceRefType;
  typedef typename QEType::PrimalDataType                PrimalDataType;
  typedef typename QEType::DualDataType                  DualDataType;
  typedef typename QEType::DualType                      QEDual;

public:
  /** Standard part of every itk Object. */
  itkTypeMacro( QuadEdgeMeshLineCell, TCellInterface );

  // accessor to the new QEGeom link that replaces now inheritance.
  QEType* GetQEGeom( ) const { return( m_QuadEdgeGeom ); };

public:
  /** Object memory management methods. */
  QuadEdgeMeshLineCell( );
  virtual ~QuadEdgeMeshLineCell();

  /** Accessors for m_Identifier. */
  void SetIdent( CellIdentifier cid );
  
  CellIdentifier GetIdent();

  /** Implement the standard CellInterface. */
  SelfAutoPointer New();

  /** TCellInterface abstract methods definition. */
  virtual void Accept( unsigned long cellId, MultiVisitor* mv );
  virtual CellGeometry GetType() const;

  /** Topology related methods. */
  static int GetTopologyId( );
  virtual unsigned int GetDimension() const;
  virtual unsigned int GetNumberOfPoints() const;
  
  virtual CellFeatureCount GetNumberOfBoundaryFeatures( int dimension ) const;
  virtual bool GetBoundaryFeature( int dimension,
                                   CellFeatureIdentifier cellId,
                                   CellAutoPointer& cell );

  /** Useless methods. */
  virtual void MakeCopy( CellAutoPointer& cell ) const
    {
    cell.TakeOwnership( new Self );
    cell->SetPointId( 0, this->GetQEGeom( )->GetOrigin( ) );
    cell->SetPointId( 1, this->GetQEGeom( )->GetDestination( ) );
    }

  /** ITK Cell API - Iterator-related methods.
   *  The Set methods will work, not the Get.
   *  Hopefully never used ...
   */
  virtual void SetPointIds( PointIdConstIterator first );
  virtual void SetPointIds( PointIdConstIterator first,
                            PointIdConstIterator last );
  virtual void SetPointId( int localId, PointIdentifier pId );

  virtual PointIdIterator PointIdsBegin(){return (PointIdIterator)0; }
  virtual PointIdIterator PointIdsEnd(){return (PointIdIterator)0; }

  virtual PointIdConstIterator GetPointIds() const {return (PointIdIterator)0; }
  virtual PointIdConstIterator PointIdsBegin() const {return (PointIdIterator)0; }
  virtual PointIdConstIterator PointIdsEnd() const {return (PointIdIterator)0; }

  /** QuadEdge internal flavor of cell API **/
  virtual void InternalSetPointIds( PointIdInternalConstIterator first );
  virtual void InternalSetPointIds( PointIdInternalConstIterator first,
                            PointIdInternalConstIterator last );

  virtual PointIdInternalIterator InternalPointIdsBegin();
  virtual PointIdInternalIterator InternalPointIdsEnd();

  virtual PointIdInternalConstIterator InternalGetPointIds() const;
  virtual PointIdInternalConstIterator InternalPointIdsBegin() const;
  virtual PointIdInternalConstIterator InternalPointIdsEnd() const;

private:
  QuadEdgeMeshLineCell( const Self& );  //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  /**
   * In order to have constant time access at the itk level instead of 
   * of doing a search in the Mesh::Cell container.
   */
  CellIdentifier m_Identifier;
  QEType*        m_QuadEdgeGeom;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkQuadEdgeMeshLineCell.txx"
#endif 

#endif 
