/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMesh.txx,v $
  Language:  C++
  Date:      $Date: 2008-07-12 22:29:28 $
  Version:   $Revision: 1.50 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkQuadEdgeMesh_txx
#define __itkQuadEdgeMesh_txx
#include "itkQuadEdgeMesh.h"
#include <vcl_limits.h>
#include <vector>

namespace itk
{

template< typename TPixel, unsigned int VDimension, typename TTraits >
const typename QuadEdgeMesh< TPixel, VDimension, TTraits >::PointIdentifier
QuadEdgeMesh< TPixel, VDimension, TTraits >::m_NoPoint =
vcl_numeric_limits< PointIdentifier >::max();

template< typename TPixel, unsigned int VDimension, typename TTraits >
const typename QuadEdgeMesh< TPixel, VDimension, TTraits >::CellIdentifier
QuadEdgeMesh< TPixel, VDimension, TTraits >::m_NoFace =
vcl_numeric_limits< CellIdentifier >::max();

/**
 * Restore the mesh to its initial state. Useful for data pipeline updates
 * without memory re-allocation.
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::Initialize()
{
  itkDebugMacro("Mesh Initialize method ");
  Clear();
  Superclass::Initialize();
}

/**
 * Clear all this mesh by deleting all contained edges which as
 * a side effect deletes adjacent faces
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::Clear()
{
  if( this->GetEdgeCells() )
    {
    CellsContainerIterator cellIterator = this->GetEdgeCells( )->Begin( );
    while( !this->GetEdgeCells( )->empty( ) )
      {
      EdgeCellType* edgeToDelete = 
                      dynamic_cast< EdgeCellType* >( cellIterator.Value( ) );
      this->LightWeightDeleteEdge( edgeToDelete );
      cellIterator = this->GetEdgeCells( )->Begin( );
      }
    }

  // Clear the points potentialy left behind by LightWeightDeleteEdge():
  if( this->GetPoints( ) )
    {
    this->GetPoints( )->clear( );
    }
  this->ClearFreePointAndCellIndexesLists( ); // to start at index 0

}

template< typename TPixel, unsigned int VDimension, typename TTraits >
void
QuadEdgeMesh< TPixel, VDimension, TTraits >
::Graft( const DataObject* data )
{
  this->Superclass::Graft( data );
  const Self* mesh = 0;

  try
    {
    mesh = dynamic_cast< const Self* >( data );
    }
  catch( ... )
    {
    // mesh could not be cast back down
    itkExceptionMacro(<< "itk::QuadEdgeMesh::CopyInformation() cannot cast "
                      << typeid(data).name() << " to "
                      << typeid(Self*).name() );
    }

  if ( !mesh )
    {
    // pointer could not be cast back down
    itkExceptionMacro(<< "itk::QuadEdgeMesh::CopyInformation() cannot cast "
                      << typeid(data).name() << " to "
                      << typeid(Self*).name() );
    }

  this->m_FreePointIndexes = mesh->m_FreePointIndexes;
  this->m_FreeCellIndexes = mesh->m_FreeCellIndexes;
}

/**
 * \brief The one and only method to modify the edge connectivity.
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::PointIdentifier
QuadEdgeMesh< TPixel, VDimension, TTraits >
::Splice( QEPrimal* a, QEPrimal* b )
{
  bool SplitingOrigin = a->IsInOnextRing( b );
  PointIdentifier resultingOriginId;

  if( SplitingOrigin )
    {
    // see TODO's entry dated 2006-01-24
    /* We consider the following situation which depicts the Onext()
    * ring around the point Origin (which is both a->GetOrigin() and
    * b->GetOrigin():
    *
    *              \         /
    *               \       /
    *               e3     e2              counter-clockwise
    *                 \   /                Onext() order.
    *                  \ /
    *      -----b------Org------a----
    *                  / \
    *                 /   \
    *                /     \
    *               /       \
    *              /         \
    *            e5           e6
    *            /             \
    *           /               \
    *
    * The result of this method is then:
    *
    *         \         /
    *          \       /
    *          e3     e2
    *            \   /
    *             \ /
    * ----b------newOrg
    *
    *                  Org------a-----
    *                  / \
    *                 /   \
    *                /     \
    *               /       \
    *              /         \
    *            e5           e7
    *            /             \
    *           /               \
    */

    // Handle connectivity at QEQuadEdge level:
    a->Splice( b );

    ////////// Handle the geometrical references:
    // Make sure the Origin's edge entry doesn't point to an entry edge
    // that isn't any more in the Onext ring:
    PointIdentifier orgId = a->GetOrigin();
    PointType org = this->GetPoint( orgId );
    org.SetEdge( a );
    this->SetPoint( orgId, org );

    // Create a newOrigin point by duplicating the geometry of Origin...
    PointType newOrigin  = org;
    newOrigin.SetEdge( b );
    PointIdentifier newOriginId = this->AddPoint( newOrigin );

    // ...and inform Onext ring of b that their Origin() have changed:
    typename QEPrimal::IteratorGeom it;
    for( it = b->BeginGeomOnext(); it != b->EndGeomOnext(); it++ )
      {
      it.Value()->SetOrigin( newOriginId );
      }
    resultingOriginId = newOriginId;
    }
  else
    {
    // see TODO's entry dated 2006-01-24
    /* We consider the following situation which depicts the Onext()
    * rings around the point Origin = a->GetOrigin() and
    * oldOrigin = b->GetOrigin():
    *
    *         \         /
    *          \       /
    *          e3     e2
    *            \   /
    *             \ /
    * ----b------oldOrg
    *
    *                  Org------a-----
    *                  / \
    *                 /   \
    *                /     \
    *               /       \
    *              /         \
    *            e5           e7
    *            /             \
    *           /               \
    *
    *
    * The result of this method is then:
    *
    *              \         /
    *               \       /
    *               e3     e2              counter-clockwise
    *                 \   /                Onext() order.
    *                  \ /
    *      -----b------Org------a----
    *                  / \
    *                 /   \
    *                /     \
    *               /       \
    *              /         \
    *            e5           e6
    *            /             \
    *           /               \
    *
    * Note: in this case we must handle the geometry first and
    *       then the connectivity.
    */
  
    // Since this is the geometrical version of Splice() we
    // have additional geometrical information that we should use
    // to check the correctness of the situation.
  
    /////////////////////////////////////////////////////////////
    // First, consider the vertices: Origin and oldOrigin must be different.
    PointIdentifier oldOriginId = b->GetOrigin();
    PointIdentifier orgId = a->GetOrigin();
  
    if( oldOriginId == orgId )
      {
      itkDebugMacro( "Trying to fuse the same point!" );
      return( m_NoPoint );
      }
  
    /** \todo Compare the geometry of the two points and accept
     * splicing when their geometry matches. We could fix
     * an epsilon threshold distance above which the two points
     * are considered distinct.
     */
    PointType oldOrigin = this->GetPoint( oldOriginId );
    PointType org = this->GetPoint( orgId );
  
    /////////////////////////////////////////////////////////////
    /* We are done with the vertices and we might need to consider the
    * possible initial adjacent face[s]. We shall accept to proceed
    * with Splicing if and only if the following conditions are met:
    * [1] a and b both share the SAME Left face,
    * [2] a and b and in the same Lnext() ring,
    * [3] a and b are not too close followers in the Lnext() ring
    *    [this is to avoid to create a face with only two edges which
    *     is equivalent to two different edges adjacent to the same two
    *     vertices].
    *
    *                   V ---<-b---- V
    *                  /              \
    *                 /                \
    *                /              a.Lnext().Lnext()
    *               /                    \
    *              /        Face          \
    *             V                        V
    *              \     a.Splice(b)      /
    *               \     is OK          /
    *                \               a.Lnext()
    *                 \                /
    *                  \              /
    *                   V ----a->--- V
    *
    * Basically, we accept to proceed with spliting if there is a
    * single face on the left and this face is at least an hexagone
    * and the vertices we wish to splice are at least two vertices aside.
    */
  
    FaceRefType aLeftFace = a->GetLeft();
    FaceRefType bLeftFace = b->GetLeft();
  
    bool MustReconstructFace = false;
    if( ( aLeftFace == m_NoFace && bLeftFace != m_NoFace )
        || ( aLeftFace != m_NoFace && bLeftFace == m_NoFace ) )
      {
      itkDebugMacro("Face on one side but not the other. Cancel.");
      return( m_NoPoint );
      }
  
    if( aLeftFace != m_NoFace && bLeftFace != m_NoFace )
      {
        if(  ( aLeftFace == bLeftFace )
          && ( a->GetLnext( ) != b ) 
          && ( a->GetLnext()->GetLnext() != b )
          && ( b->GetLnext() != a )
          && ( b->GetLnext()->GetLnext() != a )
          && ( a->IsInLnextRing( b ) )
          && ( b->IsInLnextRing( a ) ) )
        {
        this->DeleteFace( aLeftFace );
        MustReconstructFace = true;
        }
      else
        {
        itkDebugMacro( "Face is not at least and hexagon." );
        return( m_NoPoint );
        }
      }
  
    // Notice that when aLeftFace == m_NoFace and bLeftFace == m_NoFace
    // we simply proceed... (with MustReconstructFace initialy set to
    // false.
  
    ///////////////////////////////////////////////////////////////
    // Handle connectivity at QEQuadEdge level:
    a->Splice( b );
  
    ///////////////////////////////////////////////////////////////
    // Back to dealing with the geometrical references. First
    // make sure the oldOrigin's edge entry won't be used any more:
    oldOrigin.SetEdge( (QEPrimal*)0 );
    this->SetPoint( oldOriginId, oldOrigin );
  
    // We need to inform the edges ranging from a->Onext() to b that
    // their Origin() have changed. Let's over do it (read, be lazy) and
    // inform the full Onext() ring:
    typename QEPrimal::IteratorGeom it;
    for( it = a->BeginGeomOnext(); it != a->EndGeomOnext(); it++ )
      {
      it.Value()->SetOrigin( orgId );
      }
    resultingOriginId = oldOriginId;
  
    ///////////////////////////////////////////////////////////////
    // Now that we are done with the handling of the geometry of
    // vertices proceed with the geometry of the faces. When we
    // are spliting a face (through Splicing) we must construct two
    // new faces:
    if( MustReconstructFace )
      {
      this->AddFace( a );
      this->AddFace( b );
      }
    }

  this->Modified();
  return resultingOriginId;
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void QuadEdgeMesh< TPixel, VDimension, TTraits >
::SetCell( CellIdentifier cId, CellAutoPointer& cell )
{
  (void)cId;

  // NOTE ALEX: should add some checking to be sure everything went fine
  EdgeCellType* qe;
  PolygonCellType* pe;

  // The QuadEdgeMeshCellTypes first
  if( ( qe = dynamic_cast< EdgeCellType* >( cell.GetPointer() ) ) )
    {
    // NOTE ALEX: here
    this->AddEdge( qe->GetQEGeom( )->GetOrigin( ),
                   qe->GetQEGeom( )->GetDestination( ) );
    cell.ReleaseOwnership( );
    delete qe;
    }
  else if( ( pe = dynamic_cast< PolygonCellType* >( cell.GetPointer() ) ) )
    {
    PointIdList points;
    PointIdInternalIterator pit = pe->InternalPointIdsBegin();
    while( pit != pe->InternalPointIdsEnd() )
      {
      points.push_back( *pit );
      pit++;
      }
    // NOTE ALEX: here
    this->AddFaceWithSecurePointList( points );
    cell.ReleaseOwnership( );
    delete pe;
    }
  else // non-QE cell, i.e. original itk cells for example
    {
    PointIdentifier numPoint = cell->GetNumberOfPoints( );
    PointIdIterator pointId = cell->PointIdsBegin();
    PointIdIterator endId = cell->PointIdsEnd();
    // Edge
    if( numPoint == 2 )
      {
      if( ( pointId ) && ( endId ) && ( pointId != endId ) )
        {
        PointIdIterator temp = pointId++;
        // NOTE ALEX: here
        this->AddEdge( *pointId, *temp );
        }
      }
    // polygons
    else if( cell->GetDimension( ) == 2 )
      {
      PointIdList points;
      while( pointId != endId )
        {
        points.push_back( *pointId );
        pointId++;
        }
      // NOTE ALEX: here
      this->AddFace( points );
      }
    cell.ReleaseOwnership( );
    delete ( cell.GetPointer( ) );
    }
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::PointIdentifier
QuadEdgeMesh< TPixel, VDimension, TTraits >
::FindFirstUnusedPointIndex()
{
  PointIdentifier pid = 0;
  PointIdentifier maxpid = this->GetNumberOfPoints( );
  if( ! m_FreePointIndexes.empty( ) )
    {
    // find the first valid free ID
    do
      {
      pid = m_FreePointIndexes.front();
      if( pid < maxpid ) 
        {
        m_FreePointIndexes.pop();
        return( pid );
        }
      else
        {
        m_FreePointIndexes.pop();
        }
      }
    while( !m_FreePointIndexes.empty( ) );
    }

  if( m_FreePointIndexes.empty() )
    {
    pid = this->GetNumberOfPoints();
    if( pid != 0 )
      {
      PointsContainerConstIterator last = this->GetPoints()->End();
      last--;
      pid = last.Index() + 1;
      }
    }
  return( pid );
}


template< typename TPixel, unsigned int VDimension, typename TTraits >
void
QuadEdgeMesh< TPixel, VDimension, TTraits >
::SqueezePointsIds( )
{
  PointsContainerPointer points = this->GetPoints();
  PointsContainerConstIterator last = points->End();
  --last;

  PointIdentifier FilledPointID;
  QEType* EdgeRingEntry;
  QEType* EdgeRingIter;

  // if there is empty slots in PointCont
  while( ( m_FreePointIndexes.size() != 0 )
      && ( last.Index() >= this->GetNumberOfPoints() ) )
    {
    
    // duplicate last point into the empty slot and pop the id from freeID list
    FilledPointID = AddPoint( GetPoint( last.Index( ) ) );
    
    // make sure that all the edges/faces now refer to the new ID
    EdgeRingEntry = GetPoint( last.Index( ) ).GetEdge( );
    if( EdgeRingEntry )
      {
      EdgeRingIter  = EdgeRingEntry;
      do {
        EdgeRingIter->SetOrigin( FilledPointID );
        EdgeRingIter = EdgeRingIter->GetOnext( );
        }
      while( EdgeRingIter != EdgeRingEntry );
      }
    // Delete the point directly from the container
    points->DeleteIndex( last.Index( ) );
    last = points->End();
    --last;
    }
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::PointIdentifier
QuadEdgeMesh< TPixel, VDimension, TTraits >
::AddPoint( const PointType& p )
{
  PointIdentifier pid = this->FindFirstUnusedPointIndex();
  this->SetPoint( pid, p );
  return( pid );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void QuadEdgeMesh< TPixel, VDimension, TTraits >
::DeletePoint( const PointIdentifier& pid )
{
  PointType pointToDelete = this->GetPoint( pid);  
  if( pointToDelete.GetEdge() )
    {
    itkDebugMacro("Point is not isolated.");
    return;
    }

  this->GetPoints()->DeleteIndex( pid );
  m_FreePointIndexes.push( pid );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::PointType
QuadEdgeMesh< TPixel, VDimension, TTraits >
::GetPoint( const PointIdentifier& pid ) const
{
  return( this->GetPoints()->GetElement( pid ) );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::VectorType
QuadEdgeMesh< TPixel, VDimension, TTraits >
::GetVector( const PointIdentifier& pid ) const
{
  return( this->GetPoint( pid ).GetVectorFromOrigin( ) );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::CellIdentifier
QuadEdgeMesh< TPixel, VDimension, TTraits >
::FindFirstUnusedCellIndex()
{
  CellIdentifier cid;

  if( m_FreeCellIndexes.size() == 0 )
    {

    cid = this->GetNumberOfCells();

    if( cid != 0 )
      {
      CellsContainerIterator last = this->GetCells()->End();
      last--;
      cid = last.Index() + 1;
      }
    }
  else
    {
    cid = m_FreeCellIndexes.front();
    m_FreeCellIndexes.pop();
    }

  return( cid );
}

/**
 *\brief  Construct a new edge ending at points with identifiers given
 *        as arguments.
 * @param  orgPid first endpoint (origin) of the edge to Add.
 * @param destPid second endpoint (destination) of the edge to Add.
 * @sa \ref GeometricalQuadEdge::InsertAfterNextBorderEdgeWithUnsetLeft
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::AddEdge( const PointIdentifier& orgPid, const PointIdentifier& destPid )
{
  // Make sure the points are different
  if( orgPid == destPid )
    {
    itkDebugMacro( "Creating an edge between the same point." );
    return( (QEPrimal*)0 );
    }

  // Make sure the points are allready in the QuadEdgeMesh container:
  if( !( this->GetPoints()->IndexExists( orgPid ) ) ||
      !( this->GetPoints()->IndexExists( destPid ) ) )
    {
    itkDebugMacro( "One of the points not in the PointSet." );
    return( (QEPrimal*)0 );
    }

  // Make sure the edge is not allready in the container
  QEPrimal* e = this->FindEdge( orgPid, destPid );
  if( e != (QEPrimal*)0 )
    {
    itkDebugMacro("Edge already in QuadEdgeMesh.");
    return e;
    }

  // Check if the points have room to receive a new edge
  QEPrimal*  eOrigin     = this->GetPoint(  orgPid ).GetEdge();

  if( eOrigin )
    {
    if( eOrigin->IsOriginInternal() )
      {
      itkDebugMacro("No room for a new edge in the Origin() ring.");
      return( (QEPrimal*)0 );
      }
    }

  QEPrimal* eDestination = this->GetPoint( destPid ).GetEdge();

  if( eDestination )
    {
    if( eDestination->IsOriginInternal() )
      {
      itkDebugMacro("No room for a new edge in the Destination() ring.");
      return( (QEPrimal*)0 );
      }
    }

  return AddEdgeWithSecurePointList( orgPid, destPid );
}

template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::AddEdgeWithSecurePointList( const PointIdentifier& orgPid, const PointIdentifier& destPid )
{
  QEPrimal*  eOrigin     = this->GetPoint(  orgPid ).GetEdge();
  QEPrimal* eDestination = this->GetPoint( destPid ).GetEdge();
  
  // Ok, there's room and the points exist
  EdgeCellType* newEdge = new EdgeCellType( );
  QEPrimal* newEdgeGeom = newEdge->GetQEGeom( );

  newEdgeGeom->SetOrigin (  orgPid );
  newEdgeGeom->SetDestination( destPid );

  if( !eOrigin )
    {
    PointType pOrigin = this->GetPoint( orgPid );
    pOrigin.SetEdge( newEdgeGeom );
    this->SetPoint( orgPid, pOrigin );
    }
  else
    {
    eOrigin->InsertAfterNextBorderEdgeWithUnsetLeft( newEdgeGeom );
    }

  if( !eDestination )
    {
    PointType pDestination = this->GetPoint( destPid );
    pDestination.SetEdge( newEdgeGeom->GetSym() );
    this->SetPoint( destPid, pDestination );
    }
  else
    {
    eDestination->InsertAfterNextBorderEdgeWithUnsetLeft(
                                                        newEdgeGeom->GetSym() );
    }

  // Add it to the container
  this->PushOnContainer( newEdge ); 

  return( newEdgeGeom );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void
QuadEdgeMesh< TPixel, VDimension, TTraits >
::PushOnContainer( EdgeCellType* newEdge )
{
  CellIdentifier eid = 0;
  if(this->GetEdgeCells()->size() > 0)
    {
    CellsContainerConstIterator last = this->GetEdgeCells()->End();
    --last;
    eid = last.Index() + 1;
    }
  newEdge->SetIdent( eid );
  CellAutoPointer pEdge;
  pEdge.TakeOwnership( newEdge );
  this->SetEdgeCell( eid, pEdge );
  m_NumberOfEdges++;
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::DeleteEdge( const PointIdentifier& orgPid, const PointIdentifier& destPid )
{
  // Check if the edge exists
  QEPrimal* e = this->FindEdge( orgPid, destPid );

  if( e == (QEPrimal*)0 )
    {
    itkDebugMacro("Edge missing in mesh.");
    return;
    }

  this->DeleteEdge( e );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::DeleteEdge( QEPrimal* e )
{
  const PointIdentifier& orgPid  = e->GetOrigin();
  const PointIdentifier& destPid = e->GetDestination();

  // Check if the Origin point's edge ring entry should be changed
  PointType pOrigin = this->GetPoint( orgPid );
  if( pOrigin.GetEdge() == e )
    {
    if( !e->IsOriginDisconnected() )
      {
      pOrigin.SetEdge( e->GetOprev() );
      }
    else
      {
      pOrigin.SetEdge( (QEPrimal*)0 );
      }

    this->SetPoint( orgPid, pOrigin );
    }

  // Same for the Destination point
  PointType pDestination = this->GetPoint( destPid );

  if( pDestination.GetEdge() == e->GetSym() )
    {
    if( !e->IsDestinationDisconnected() )
      {
      pDestination.SetEdge( e->GetLnext() );
      }
    else
      {
      pDestination.SetEdge( (QEPrimal*)0 );
      }

    this->SetPoint( destPid, pDestination );
    }

  // This container serves to avoid the MS .net bug when
  // one wants to delete a map element using a map::iterator.
  // Normally, if we delete a map element using an iterator,
  // it should keep the iterator validity but .net doesn't
  // like it, so we delay the cell deletion to a later loop.
  typedef std::vector< CellIdentifier > DeleteCellsCont;
  DeleteCellsCont cellsToDelete;

  // Delete all references to 'e' in the cell container
  CellsContainerIterator cit = this->GetCells()->Begin();
  while( cit != this->GetCells()->End() )
    {
    PolygonCellType* pcell = dynamic_cast< PolygonCellType* >(cit.Value());
    bool toDelete = false;
    if( pcell != (PolygonCellType*)0 )
      {
      QEPrimal* edge = pcell->GetEdgeRingEntry();
      typename QEPrimal::IteratorGeom it = edge->BeginGeomLnext();

      while( it != edge->EndGeomLnext() && !toDelete )
        {
        toDelete = ( ( it.Value() == e ) ||
                     ( it.Value()->GetSym() == e ) );
        it++;
        }

      if( toDelete )
        {
        --m_NumberOfFaces;
        // handle QE level, i.e. for the polygon, just unset the faces
        it = edge->BeginGeomLnext();
        while( it != edge->EndGeomLnext() )
          {
          it.Value()->UnsetLeft();
          it++;
          }
        }
      }

    // if the current face is to be deleted, 
    // put it in the second container
    // and keep the Id for next cell insertion
    if( toDelete )
      {
      cellsToDelete.push_back( cit.Index() );
      m_FreeCellIndexes.push( cit.Index() );
      }
    cit++;
    }

  // we checked all the cells i nthe container
  // now delete the elements in the map
  typename DeleteCellsCont::iterator dit = cellsToDelete.begin();
  while( dit != cellsToDelete.end() )
    {
    const CellType * cellToBeDeleted = this->GetCells()->GetElement( *dit );
    delete cellToBeDeleted;
    this->GetCells()->DeleteIndex( *dit );
    dit++;
    }
  
  // now delete the edge in the edge container
  this->GetEdgeCells()->DeleteIndex( e->GetIdent() );
  --m_NumberOfEdges;

  // Now, disconnect it and let the garbage collector do the rest
  this->Modified();
}

/**
 * Delete the incoming edge and all LOCAL references to this edge.
 * By local we mean the ones we can reasonably be aware of i.e.
 * the adjacent faces (that we also delete) and the adjacent points
 * (when the incoming edge is their Onext ring entry).
 * This is to be opposed to \ref DeleteEdge that searches for ALL
 * references to the incoming edge (which is a much heavier process
 * because one as to make an exhaustive search in the CellContainer).
 * \note: when deleting the adjacent faces we also handle the
 *        suppression of the references to those faces in the Lnext()
 *        and Rnext() rings.
 * \warning Nothing is done to remove the potential isolated points
 *        left by this edge deletion (the caller might want to recycle
 *        them). Hence it is the caller's responsability to manage the
 *        clean-up of adjacent points (when necessary).
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::LightWeightDeleteEdge( EdgeCellType* edgeCell )
{
 
  if( !edgeCell )
    {
    return;
    }

  QEPrimal* e = edgeCell->GetQEGeom( );
 
  if ( !e)
    {
    return;
    }
  const PointIdentifier& orgPid  = e->GetOrigin();
  const PointIdentifier& destPid = e->GetDestination();
  if( orgPid != e->m_NoPoint &&  destPid != e->m_NoPoint)
    {
    // ------------------------------------------------------------------
    // First make sure the points are not pointing to the edge we are
    // trying to delete.

    // Check if the Origin point's edge ring entry is the edge we are
    // trying to delete. When this is the case shift the Origin edge entry
    // to another edge and when no other edge is available leave it
    // to NULL.
    PointType pOrigin = this->GetPoint( orgPid );

    if( pOrigin.GetEdge() == e )
      {
      if( !e->IsOriginDisconnected() )
        {
        pOrigin.SetEdge( e->GetOprev() );
        }
      else
        {
        pOrigin.SetEdge( (QEPrimal*)0 );
        }
      
      this->SetPoint( orgPid, pOrigin );
      }

    // Same thing for the Destination point:
    PointType pDestination = this->GetPoint( destPid );

    if( pDestination.GetEdge() == e->GetSym() )
      {
      if( !e->IsDestinationDisconnected() )
        {
        pDestination.SetEdge( e->GetLnext() );
        }
      else
        {
        pDestination.SetEdge( (QEPrimal*)0 );
        }

      this->SetPoint( destPid, pDestination );
      }
    // ------------------------------------------------------------------
    // Second we need to destroy the adjacent faces (both GetLeft()
    // and GetRight() when they exist) because their very definition
    // makes reference to the edge we are trying to delete:
    if( e->IsLeftSet() )
      {
      this->DeleteFace( e->GetLeft() );
      }

    if( e->IsRightSet() )
      {
      this->DeleteFace( e->GetRight() );
      }
      
    /////////////////////////////////////////////////////////////////
    // Third we need to remove from the container the EdgeCell
    // representing the edge we are trying to destroy at the itk
    // level.
    this->GetEdgeCells()->DeleteIndex( edgeCell->GetIdent() );
    edgeCell->SetIdent( 0 );

    // Eventually, we disconnect (at the QuadEdge level) the edge we
    // are trying to delete and we delete it.
    e->Disconnect();
    }

  --m_NumberOfEdges;
  delete edgeCell;
  this->Modified();
}

template< typename TPixel, unsigned int VDimension, typename TTraits >
void 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::LightWeightDeleteEdge( QEPrimal* e )
{
  if (!e)
    {
    return;
    }
  const PointIdentifier& orgPid  = e->GetOrigin();
  if( orgPid == e->m_NoPoint )
    {
    // org not set
    return;
    }
    
  const PointIdentifier& destPid = e->GetDestination();
  if( destPid == e->m_NoPoint )
    {
    // dest not set
    return;
    }

  CellIdentifier LineIdent = e->GetIdent( );
  if( LineIdent != m_NoPoint )
    { 
    EdgeCellType* edgeCell = dynamic_cast< EdgeCellType* >(this->GetEdgeCells( )->GetElement( LineIdent ));
    this->LightWeightDeleteEdge( edgeCell );
    }
  else
    {
    itkDebugMacro( "Edge Not found. LineIdent not set?" );
    return;
    }

}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void
QuadEdgeMesh< TPixel, VDimension, TTraits >
::DeleteFace( FaceRefType faceToDelete )
{
  if( !this->GetCells()->IndexExists( faceToDelete ) )
    {
    itkDebugMacro( "No such face in container" );
    return;
    }

  PolygonCellType* cellToDelete = dynamic_cast< PolygonCellType* >
                     ( this->GetCells()->GetElement( faceToDelete ) );
  if( !cellToDelete )
    {
    itkDebugMacro( "This Id does not correspond to a face (should be an edge)" );
    return;
    }

  // Iterate on the edges adjacent to face and remove references to
  // to this face:
  QEPrimal* e = cellToDelete->GetEdgeRingEntry();

  if( faceToDelete != e->GetLeft() )
    {
    e = e->GetSym();
    }

  if( faceToDelete != e->GetLeft() )
    {
    itkDebugMacro("Neither e nor e->Sym() are the correct face");
    return;
    }

  typename QEPrimal::IteratorGeom it;

  for( it = e->BeginGeomLnext(); it != e->EndGeomLnext(); it++ )
    {
    it.Value()->UnsetLeft();
    }

  this->GetCells()->DeleteIndex( faceToDelete );
  delete cellToDelete;
  
  --m_NumberOfFaces;

  this->Modified();
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::GetEdge() const
{
  if( this->GetEdgeCells()->size() == 0 ) return( (QEPrimal*)0 );

  CellsContainerIterator cit = this->GetEdgeCells()->Begin();
  EdgeCellType* e = dynamic_cast< EdgeCellType* >( cit.Value( ) );
  return( e->GetQEGeom( ) );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::GetEdge( const CellIdentifier& eid ) const
{
   
  if( !this->GetEdgeCells()->IndexExists( eid ) )
    {
    itkDebugMacro( "No such edge in container" );
    return( (QEPrimal*)0 );
    }

  EdgeCellType* e = dynamic_cast< EdgeCellType* >(
                                    this->GetEdgeCells( )->GetElement( eid ) );
  return( e->GetQEGeom( ) );

}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::FindEdge( const PointIdentifier& pid0 ) const
{
  PointType p = this->GetPoint( pid0 );
  return( p.GetEdge() ); 
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::FindEdge( const PointIdentifier& pid0, const PointIdentifier& pid1 ) const
{
  QEPrimal * initialEdge = this->GetPoint( pid0 ).GetEdge( );
  if( initialEdge )
    {
    typename QEPrimal::IteratorGeom it = initialEdge->BeginGeomOnext();
    while( it != initialEdge->EndGeomOnext() )
      {
      if(  it.Value()->GetDestination() == pid1 )
        {
        return( dynamic_cast< QEPrimal* >( it.Value() ) );
        }
      ++it;
      }
    }
  return( static_cast< QEPrimal * >(NULL) );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::EdgeCellType*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::FindEdgeCell( const PointIdentifier& pid0,const PointIdentifier& pid1 ) const
{
  EdgeCellType* result = (EdgeCellType*)0;
  QEPrimal* EdgeGeom = FindEdge( pid0, pid1);
  if( EdgeGeom != (QEPrimal*)0 )
    {
    CellIdentifier LineIdent = EdgeGeom->GetIdent( );
    if( LineIdent != m_NoPoint )
      {
      result = dynamic_cast< EdgeCellType* >(
                                this->GetEdgeCells()->GetElement( LineIdent ) );
      }
    }
  return( result );
}

 
/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::AddFace( const PointIdList & points )
{
  // Check that there are no duplicate points
  for(unsigned int i=0; i < points.size(); i++)
    {
    typename PointIdList::const_iterator itr = points.begin();
    typename PointIdList::const_iterator end = points.end();
    typename PointIdList::size_type count = 0;
    const PointIdentifier pointId = points[i];
    while( itr != end )
      {
      if( *itr == pointId )
        {
        ++count;
        }
      ++itr;
      }
    if( count != 1 )
      {
      itkDebugMacro("Point "<<i<<" is duplicated");
      return( (QEPrimal*)NULL );
      }
    }

  // Check that all points exist
  for(unsigned int i=0; i < points.size(); i++)
    {
    if( !this->GetPoints()->IndexExists( points[i] ) )
      {
      itkDebugMacro("Point "<<i<<" is missing in the mesh");
      return (QEPrimal*) NULL;
      }
    }

  // Check if existing edges have no face on the left.
  for(unsigned int i=0; i < points.size(); i++)
    {
    PointIdentifier pid0 = points[i];
    PointIdentifier pid1 = points[ (i+1) % points.size() ];

    QEPrimal* edge = this->FindEdge( pid0, pid1 );

    if( edge )
      {
      if( edge->IsLeftSet() )
        {
          itkDebugMacro("Edge [" << i << " " << ((i+1) % points.size())
              <<" has a left face.");
              return (QEPrimal*) NULL;
        }
      }
    }

    return AddFaceWithSecurePointList( points );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::AddFaceWithSecurePointList( const PointIdList& points )
{
  typedef std::vector< QEPrimal* >  QEList;
  QEList FaceQEList;

  // Now create edges as needed.
  for(unsigned int i=0; i < points.size(); i++)
    {
    PointIdentifier pid0 = points[i];
    PointIdentifier pid1 = points[ (i+1) % points.size() ];
    QEPrimal* edge = this->FindEdge( pid0, pid1 );

    if( !edge )
      {
      QEPrimal* entry = this->AddEdgeWithSecurePointList( pid0, pid1 );
      if( entry == (QEPrimal*)0 )
        {
        return( entry );
        }
      FaceQEList.push_back( entry);
      }
    else
      {
      FaceQEList.push_back( edge );
      }
    }

  // Reorder all Onext rings
  QEPrimal* e1;
  QEPrimal* e0 = FaceQEList[points.size()-1];
  for(unsigned int i=0; i < points.size(); i++)
    {
    e1 = e0->GetSym();
    e0 = FaceQEList[i];

    e0->ReorderOnextRingBeforeAddFace( e1 );
    }

  // all edges are ready to receive a face on the left
  QEPrimal* entry = FaceQEList[0];

  if( !entry )
    {
    itkDebugMacro("entry == NULL");
    return (QEPrimal*) NULL;
    }

  this->AddFace(entry);

  return( entry );
}

/**
 * We here make the strong assumption that the caller was wise enough
 * to build/handle the connectivity at the QE level. This method
 * simply creates a new PolygonCell and assigns it as the left face
 * of all edges in the Lnext ring of the incoming argument.
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
void
QuadEdgeMesh< TPixel, VDimension, TTraits >
::AddFace( QEPrimal* entry )
{
  // Create the cell and add it to the container
  PolygonCellType* faceCell = new PolygonCellType( entry );
  CellIdentifier fid = this->FindFirstUnusedCellIndex();
  faceCell->SetIdent( fid );

  // Associate the above generated CellIndex as the default FaceRefType
  // of the new face [ i.e. use the itk level CellIdentifier as the
  // GeometricalQuadEdge::m_Origin of dual edges (edges of type QEDual) ].
  typename QEPrimal::IteratorGeom it;
  for( it = entry->BeginGeomLnext(); it != entry->EndGeomLnext(); it++ )
    {
    it.Value()->SetLeft( fid );
    }

  ++m_NumberOfFaces;
  CellAutoPointer face;
  face.TakeOwnership( faceCell );
  this->Superclass::SetCell( fid, face );
}

/**
 * Add a triangle face to this QuadEdgeMesh.
 * @param aPid \ref PointIdentifier of first point
 * @param bPid \ref PointIdentifier of second point
 * @param cPid \ref PointIdentifier of third point
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::QEPrimal*
QuadEdgeMesh< TPixel, VDimension, TTraits >
::AddFaceTriangle( 
  const PointIdentifier& aPid,
  const PointIdentifier& bPid,
  const PointIdentifier& cPid )
{
  PointIdList points;
  points.push_back( aPid );
  points.push_back( bPid );
  points.push_back( cPid );
  return this->AddFace( points );
}

/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
QuadEdgeMesh< TPixel, VDimension, TTraits >
::QuadEdgeMesh(): m_NumberOfFaces(0), m_NumberOfEdges(0)
{
  m_EdgeCellsContainer = CellsContainer::New();
}


template< typename TPixel, unsigned int VDimension, typename TTraits >
QuadEdgeMesh< TPixel, VDimension, TTraits >
::~QuadEdgeMesh()
{
  if( m_EdgeCellsContainer->GetReferenceCount() == 1 )
    {
    CellsContainerIterator EdgeCell = m_EdgeCellsContainer->Begin();
    CellsContainerIterator EdgeEnd  = m_EdgeCellsContainer->End();
    while( EdgeCell != EdgeEnd )
      {
      const CellType * EdgeCellToBeDeleted = EdgeCell->Value( );
      delete EdgeCellToBeDeleted;
      ++EdgeCell;
      }
    m_EdgeCellsContainer->Initialize();
    }
}


/**
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
typename QuadEdgeMesh< TPixel, VDimension, TTraits >::CoordRepType
QuadEdgeMesh< TPixel, VDimension, TTraits >
::ComputeEdgeLength( QEPrimal* e )
{
  const PointType org  = this->GetPoint( e->GetOrigin()  );
  const PointType dest = this->GetPoint( e->GetDestination() );

  const CoordRepType length = org.EuclideanDistanceTo( dest );
  
  return( length );
}

/**
 * \brief Compute the total number of USED points. This differs from
 * \ref Mesh::GetNumberOfPoints() that will return the total number of
 * points including the ones that have no entry in the edge ring.
 *
 * \note This method is an optional utility of the class: its
 * understanding is not usefull at first contact with the class.
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
unsigned long 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::ComputeNumberOfPoints() const
{
  const PointsContainer* points = this->GetPoints();

  if( ! points )
    {
    itkDebugMacro("No point container");
    return( 0 );
    }

  unsigned long numberOfPoints = 0;
  PointsContainerConstIterator pointIterator = points->Begin();

  while( pointIterator != points->End() )
    {
    if( pointIterator.Value().GetEdge() )
      {
      numberOfPoints++;
      }
    pointIterator++;
    }

  return( numberOfPoints );
}

/**
 * \brief Compute the total number of faces.
 *
 * \note This method is an optional utility of the class: its
 * understanding is not usefull at first contact with the class.
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
unsigned long 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::ComputeNumberOfFaces() const
{
  unsigned long numberOfFaces = 0;
  CellsContainerConstIterator cellIterator = this->GetCells()->Begin();
  CellsContainerConstIterator cellEnd      = this->GetCells()->End();

  PointIdentifier NumOfPoints;
  while( cellIterator != cellEnd )
    {
    NumOfPoints = cellIterator.Value()->GetNumberOfPoints();
    if( NumOfPoints > 2 )
      {
      numberOfFaces++;
      }
    ++cellIterator;
    }

  return( numberOfFaces );
}

/**
 * \brief Compute the total number of edges.
 *
 * \note This method is an optional utility of the class: it's
 *       understanding is not usefull at first contact with the class.
 */
template< typename TPixel, unsigned int VDimension, typename TTraits >
unsigned long 
QuadEdgeMesh< TPixel, VDimension, TTraits >
::ComputeNumberOfEdges() const
{
  unsigned long numberOfEdges = this->GetEdgeCells()->size();

  return( numberOfEdges );
}

} // namespace itk

#endif 
