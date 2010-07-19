/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshEulerOperatorFlipEdgeFunction.txx,v $
  Language:  C++
  Date:      $Date: 2009-02-07 17:19:59 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuadEdgeMeshEulerOperatorFlipEdgeFunction_txx
#define __itkQuadEdgeMeshEulerOperatorFlipEdgeFunction_txx

#include "itkQuadEdgeMeshEulerOperatorFlipEdgeFunction.h"
#include "itkQuadEdgeMeshEulerOperatorJoinFacetFunction.h"
#include "itkQuadEdgeMeshEulerOperatorSplitFacetFunction.h"

namespace itk
{

template < class TMesh, class TQEType >
QuadEdgeMeshEulerOperatorFlipEdgeFunction< TMesh, TQEType >::
QuadEdgeMeshEulerOperatorFlipEdgeFunction() : Superclass(),
  m_EdgeStatus( STANDARD_CONFIG )
{}

template < class TMesh, class TQEType >
void
QuadEdgeMeshEulerOperatorFlipEdgeFunction< TMesh, TQEType >::
PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "m_EdgeStatus: ";
  switch( m_EdgeStatus )
  {
    default:
    case STANDARD_CONFIG:
      os << "STANDARD_CONFIG" <<std::endl;
      break;
    case EDGE_NULL:
      os << "EDGE_NULL" <<std::endl;
      break;
    case MESH_NULL:
      os << "MESH_NULL" <<std::endl;
      break;
    case NON_INTERNAL_EDGE:
      os << "NON_INTERNAL_EDGE" <<std::endl;
      break;
    case NON_TRIANGULAR_RIGHT_FACE:
      os << "NON_TRIANGULAR_RIGHT_FACE" <<std::endl;
      break;
    case NON_TRIANGULAR_LEFT_FACE:
      os << "NON_TRIANGULAR_LEFT_FACE" <<std::endl;
      break;
    case EXISTING_OPPOSITE_EDGE: 
      os << "EXISTING_OPPOSITE_EDGE" <<std::endl;
      break;
    }
}

template < class TMesh, class TQEType >
void
QuadEdgeMeshEulerOperatorFlipEdgeFunction< TMesh, TQEType >::
CheckStatus( QEType* h )
{
#ifndef NDEBUG
  if( h == (QEType*)0 )
    {
    m_EdgeStatus = EDGE_NULL;
    return;
    }
   
  if( !this->m_Mesh )
    {
    m_EdgeStatus = MESH_NULL;
    return;
    }
#endif
   
  if( !h->IsInternal( ) )
    {
    m_EdgeStatus = NON_INTERNAL_EDGE;
    return;
    } 
     
  if( !h->IsLnextOfTriangle( ) )
    {
    m_EdgeStatus = NON_TRIANGULAR_LEFT_FACE;
    return;
    }
  if( !h->GetSym( )->IsLnextOfTriangle( ) )
    {
    m_EdgeStatus = NON_TRIANGULAR_RIGHT_FACE;
    return;
    }

  if( this->m_Mesh->FindEdge( h->GetOnext()->GetDestination(),
      h->GetSym()->GetOnext()->GetDestination() ) != 0 )
    {
    m_EdgeStatus = EXISTING_OPPOSITE_EDGE;
    return;
    }

  m_EdgeStatus = STANDARD_CONFIG;
}

template < class TMesh, class TQEType >
typename QuadEdgeMeshEulerOperatorFlipEdgeFunction< TMesh, TQEType >::OutputType
QuadEdgeMeshEulerOperatorFlipEdgeFunction< TMesh, TQEType >::
Evaluate( QEType* h )
{
  //
  //    X ---<-G---- X              X ---<-G---- X
  //    |          / |              | \          |
  //    |         /  |              |  \         |
  //    |        /   |              |   \        |
  //    |       /    |              |    \       |
  //    |      ^     |              |     \      |
  //    |     h      |              |   newEdge  |
  //    |    /       |              |       \    |
  //    |   /        |              |        \   |
  //    |  /         |              |         \  |
  //    | /          |              |          \ |
  //    X ---H->---- X              X ---H->---- X
  //
  CheckStatus( h );

  switch( m_EdgeStatus )
    {
    default:
    case STANDARD_CONFIG:
      return Process( h );
    case EDGE_NULL:
      itkDebugMacro( "No Edge to flip." );
      return( (QEType*) 0 );
    case MESH_NULL:
      itkDebugMacro( "No mesh present." );
      return( (QEType*) 0 );
    case NON_INTERNAL_EDGE:
      itkDebugMacro( "Can only flip internal edge." );
      return( (QEType*) 0 );
    case NON_TRIANGULAR_LEFT_FACE:
      itkDebugMacro( "Can only flip edge for triangles." );
      return( (QEType*) 0 );
    case NON_TRIANGULAR_RIGHT_FACE:
      itkDebugMacro( "Can only flip edge for triangles." );
      return( (QEType*) 0 );
    case EXISTING_OPPOSITE_EDGE:
      itkDebugMacro( "The opposite edge already exists." );
      return( (QEType*) 0 );
    }
}

template < class TMesh, class TQEType >
typename QuadEdgeMeshEulerOperatorFlipEdgeFunction< TMesh, TQEType >::OutputType
QuadEdgeMeshEulerOperatorFlipEdgeFunction< TMesh, TQEType >::
Process( QEType* h )
{
  // The following is not optimum, since we create a new face (with JoinFacet)
  // that is immediately deleted (with SplitFacet). Still we chose to write it
  // that way in the sake of maintenance simplicity (as long as JoinFacet and
  // SplitFacet are working, this operator does it job).
  typedef QuadEdgeMeshEulerOperatorJoinFacetFunction< MeshType, QEType >
    JoinFacet;
  typedef QuadEdgeMeshEulerOperatorSplitFacetFunction< MeshType, QEType>
    SplitFacet;
   
  QEType* G = h->GetLnext( );
  typename JoinFacet::Pointer joinFacet = JoinFacet::New( );
  joinFacet->SetInput( this->m_Mesh );
  QEType* H = joinFacet->Evaluate( h )->GetLnext( );
   
  typename SplitFacet::Pointer splitFacet = SplitFacet::New( );
  splitFacet->SetInput( this->m_Mesh );
   
  return( splitFacet->Evaluate( H, G ) );
}

} // namespace itk

#endif

// eof - $RCSfile: itkQuadEdgeMeshEulerOperatorFlipEdgeFunction.txx,v $
