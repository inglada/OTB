/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshLineCell.txx,v $
  Language:  C++
  Date:      $Date: 2007/08/25 22:28:11 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkQuadEdgeMeshLineCell_txx
#define __itkQuadEdgeMeshLineCell_txx

#include "itkCellInterfaceVisitor.h"
#include "itkQuadEdgeMeshLineCell.h"

namespace itk
{

// ---------------------------------------------------------------------
template< class TCellInterface >
QuadEdgeMeshLineCell< TCellInterface >
::QuadEdgeMeshLineCell( )
{
  m_Identifier = 0;
  m_QuadEdgeGeom = new QEType;

  QEType* e2 = new QEType;
  QEDual* e1 = new QEDual;
  QEDual* e3 = new QEDual;
  this->m_QuadEdgeGeom->SetRot( e1 );
  e1->SetRot( e2 );
  e2->SetRot( e3 );
  e3->SetRot( this->m_QuadEdgeGeom );
  this->m_QuadEdgeGeom->SetOnext( this->m_QuadEdgeGeom );
  e1->SetOnext( e3 );
  e2->SetOnext( e2 );
  e3->SetOnext( e1 );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
QuadEdgeMeshLineCell< TCellInterface >
::~QuadEdgeMeshLineCell()
{
  // ALEX: for performance issues,
  // we will assume the user calls Disconnect beforehand
  // or else it is the mesh destructor, and we can proceed.
  // if( !m_QuadEdgeGeom->IsDisconnected( ) )
  //  {
  //  m_QuadEdgeGeom->Disconnect( );
  //  }

  bool FoundNullPointer = false;
  if( m_QuadEdgeGeom )
    {
    if( m_QuadEdgeGeom->GetRot( ) )
      {
      if( m_QuadEdgeGeom->GetRot( )->GetRot( ) )
        {
        if( m_QuadEdgeGeom->GetRot( )->GetRot( )->GetRot( ) )
          {
          delete m_QuadEdgeGeom->GetRot( )->GetRot( )->GetRot( ); //e3
          delete m_QuadEdgeGeom->GetRot( )->GetRot( );            //e2
          delete m_QuadEdgeGeom->GetRot( );                       //e1
          delete m_QuadEdgeGeom;
          }
        else
          {
          FoundNullPointer = true;
          }
        }
      else
        {
        FoundNullPointer = true;
        }
      }
    else
      {
      FoundNullPointer = true;
      }
    }
  else
    {
    FoundNullPointer = true;
    }

  if( FoundNullPointer )
    {
    //Throw exception here
    }
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::SelfAutoPointer
QuadEdgeMeshLineCell< TCellInterface >
::New()
{
  SelfAutoPointer ptr( new Self );
  ptr.TakeOwnership();
  return( ptr );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
void
QuadEdgeMeshLineCell< TCellInterface >
::Accept( unsigned long cellId, MultiVisitor* mv )
{
  typedef CellInterfaceVisitor< PixelType, CellTraits > IntVis;
  typename IntVis::Pointer v = mv->GetVisitor( this->GetType() );
  if( v )
    {
    v->VisitFromCell( cellId, this );
    }
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::CellFeatureCount
QuadEdgeMeshLineCell< TCellInterface >
::GetNumberOfBoundaryFeatures( int dimension ) const
{
  if( dimension == 0 )
    {
    return  2;
    }

  if( dimension == 1 )
    {
    return 1;
    }

  return 0;
}

// ---------------------------------------------------------------------
template< class TCellInterface >
bool
QuadEdgeMeshLineCell< TCellInterface >
::GetBoundaryFeature( int dimension, CellFeatureIdentifier cellId,
                      CellAutoPointer& cell )
{
  // TODO : FIXME
  (void)dimension;
  (void)cellId;
  (void)cell;
  return( false );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
void
QuadEdgeMeshLineCell< TCellInterface >
::SetPointIds( PointIdConstIterator first )
{
  PointIdConstIterator i = first;
  this->GetQEGeom( )->SetOrigin( *i );
  i++;
  this->GetQEGeom( )->SetDestination( *i );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
void
QuadEdgeMeshLineCell< TCellInterface >
::InternalSetPointIds( PointIdInternalConstIterator first )
{
  PointIdInternalConstIterator i = first;
  this->GetQEGeom( )->SetOrigin( *i );
  i++;
  this->GetQEGeom( )->SetDestination( *i );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
void
QuadEdgeMeshLineCell< TCellInterface >
::SetPointIds( PointIdConstIterator first,
               PointIdConstIterator last )
{
  (void)last;
  this->GetQEGeom( )->SetOrigin( *first );
  first++;
  this->GetQEGeom( )->SetDestination( *first );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
void
QuadEdgeMeshLineCell< TCellInterface >
::InternalSetPointIds( PointIdInternalConstIterator first,
               PointIdInternalConstIterator last )
{
  (void)last;
  this->GetQEGeom( )->SetOrigin( *first );
  first++;
  this->GetQEGeom( )->SetDestination( *first );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
void
QuadEdgeMeshLineCell< TCellInterface >
::SetPointId( int localId, PointIdentifier pId )
{
  if( localId == 0 )      this->GetQEGeom( )->SetOrigin( pId );
  else if( localId == 1 ) this->GetQEGeom( )->SetDestination( pId );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::PointIdInternalIterator
QuadEdgeMeshLineCell< TCellInterface >
::InternalPointIdsBegin()
{
  return( PointIdInternalIterator( this->m_QuadEdgeGeom,
                                   PointIdInternalIterator::OperatorSym, true ) );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::PointIdInternalIterator
QuadEdgeMeshLineCell< TCellInterface >
::InternalPointIdsEnd()
{
  return( PointIdInternalIterator( this->m_QuadEdgeGeom,
                                   PointIdInternalIterator::OperatorSym, false ) );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::PointIdInternalConstIterator
QuadEdgeMeshLineCell< TCellInterface >
::InternalGetPointIds() const
{
  return( PointIdInternalConstIterator( this->m_QuadEdgeGeom,
                                        PointIdInternalConstIterator::OperatorSym, true ) );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::PointIdInternalConstIterator
QuadEdgeMeshLineCell< TCellInterface >
::InternalPointIdsBegin() const
{
  return( PointIdInternalConstIterator( this->m_QuadEdgeGeom,
                                        PointIdInternalConstIterator::OperatorSym, true ) );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::PointIdInternalConstIterator
QuadEdgeMeshLineCell< TCellInterface >
::InternalPointIdsEnd() const
{
  return( PointIdInternalConstIterator( this->m_QuadEdgeGeom,
                                        PointIdInternalConstIterator::OperatorSym, false ) );
}


// ---------------------------------------------------------------------
template< class TCellInterface >
void
QuadEdgeMeshLineCell< TCellInterface >
::SetIdent( CellIdentifier cid )
{
  this->m_Identifier = cid;
  this->m_QuadEdgeGeom->SetIdent( cid );
  this->m_QuadEdgeGeom->GetSym( )->SetIdent( cid );
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::CellIdentifier
QuadEdgeMeshLineCell< TCellInterface >
::GetIdent()
{
  return this->m_Identifier;
}

// ---------------------------------------------------------------------
template< class TCellInterface >
typename QuadEdgeMeshLineCell< TCellInterface >::CellGeometry
QuadEdgeMeshLineCell< TCellInterface >
::GetType() const
{
  return  Superclass::LINE_CELL;
}

// ---------------------------------------------------------------------
template< class TCellInterface >
int
QuadEdgeMeshLineCell< TCellInterface >
::GetTopologyId()
{
  return Superclass::LINE_CELL;
}

// ---------------------------------------------------------------------
template< class TCellInterface >
unsigned int
QuadEdgeMeshLineCell< TCellInterface >
::GetDimension() const
{
  return Self::CellDimension;
}

// ---------------------------------------------------------------------
template< class TCellInterface >
unsigned int
QuadEdgeMeshLineCell< TCellInterface >
::GetNumberOfPoints() const
{
  return 2;
}

} // end namespace itk

#endif
