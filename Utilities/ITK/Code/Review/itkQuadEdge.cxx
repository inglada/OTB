/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdge.cxx,v $
  Language:  C++
  Date:      $Date: 2007-06-27 19:06:04 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkQuadEdge.h"

namespace itk
{

// ---------------------------------------------------------------------
QuadEdge
::QuadEdge()
{
  this->m_Onext = this;
  this->m_Rot   = NULL;
}


// ---------------------------------------------------------------------
QuadEdge
::~QuadEdge()
{
  this->m_Onext = NULL;
  this->m_Rot   = NULL;
}


// ---------------------------------------------------------------------
void 
QuadEdge
::Splice( Self* b )
{
  Self * aNext     = this->GetOnext();
  Self * bNext     = b->GetOnext();
  Self * alpha     = aNext->GetRot();
  Self * beta      = bNext->GetRot();
  Self * alphaNext = alpha->GetOnext();
  Self * betaNext  = beta->GetOnext();

  this->SetOnext( bNext );
  b->SetOnext( aNext );
  alpha->SetOnext( betaNext );
  beta->SetOnext( alphaNext );
}

// ---------------------------------------------------------------------
void 
QuadEdge
::SetOnext( Self * next  )
{
  this->m_Onext = next; 
}

// ---------------------------------------------------------------------
void 
QuadEdge
::SetRot( Self * rot  )
{
  this->m_Rot = rot; 
}

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetOnext()
{
  return this->m_Onext; 
}

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetRot()
{
  return this->m_Rot; 
}

// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetOnext() const
{
  return this->m_Onext; 
}

// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetRot() const
{
  return this->m_Rot; 
}

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetSym()
{

  Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetRot();
  if( p2 == NULL )
    {
    return NULL;
    }
  
  return p2;
} 

// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetSym() const
{

  const Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetRot();
  if( p2 == NULL )
    {
    return NULL;
    }


  return p2;
} 

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetLnext() 
{
  Self * p1 = this->GetInvRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  Self * p3 = p2->GetRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetLnext() const
{
  const Self * p1 = this->GetInvRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  const Self * p3 = p2->GetRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetRnext() 
{
  Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  Self * p3 = p2->GetInvRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetRnext() const
{
  const Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  const Self * p3 = p2->GetInvRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetDnext() 
{
  Self * p1 = this->GetSym();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  Self * p3 = p2->GetSym();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetDnext() const
{
  const Self * p1 = this->GetSym();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  const Self * p3 = p2->GetSym();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetOprev() 
{
  Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  Self * p3 = p2->GetRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetOprev() const
{
  const Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  const Self * p3 = p2->GetRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetLprev() 
{
  Self * p1 = this->GetOnext();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetSym();
  if( p2 == NULL )
    {
    return NULL;
    }

  return p2;
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetLprev() const
{
  const Self * p1 = this->GetOnext();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetSym();
  if( p2 == NULL )
    {
    return NULL;
    }

  return p2;
}


// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetRprev() 
{
  Self * p1 = this->GetSym();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  return p2;
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetRprev() const
{
  const Self * p1 = this->GetSym();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  return p2;
}


// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetDprev() 
{
  Self * p1 = this->GetInvRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  Self * p3 = p2->GetInvRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetDprev() const
{
  const Self * p1 = this->GetInvRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetOnext();
  if( p2 == NULL )
    {
    return NULL;
    }

  const Self * p3 = p2->GetInvRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}


// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetInvRot() 
{
  Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  Self * p2 = p1->GetRot();
  if( p2 == NULL )
    {
    return NULL;
    }

  Self * p3 = p2->GetRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetInvOnext() 
{
  return this->GetOprev(); 
}

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetInvLnext() 
{
  return this->GetLprev(); 
}

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetInvRnext() 
{
  return this->GetRprev(); 
}

// ---------------------------------------------------------------------
QuadEdge * 
QuadEdge
::GetInvDnext() 
{
  return this->GetDprev(); 
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetInvRot() const
{
  const Self * p1 = this->GetRot();
  if( p1 == NULL )
    {
    return NULL;
    }

  const Self * p2 = p1->GetRot();
  if( p2 == NULL )
    {
    return NULL;
    }

  const Self * p3 = p2->GetRot();
  if( p3 == NULL )
    {
    return NULL;
    }

  return p3;
}

// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetInvOnext() const
{
  return this->GetOprev(); 
}

// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetInvLnext() const
{
  return this->GetLprev(); 
}

// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetInvRnext() const
{
  return this->GetRprev(); 
}


// ---------------------------------------------------------------------
const QuadEdge * 
QuadEdge
::GetInvDnext() const
{
  return this->GetDprev(); 
}


// ---------------------------------------------------------------------
bool
QuadEdge
::IsHalfEdge() const
{
  const bool isHalfEdge = ( m_Onext == this ) || ( m_Rot == NULL );
  return isHalfEdge; 
}


// ---------------------------------------------------------------------
bool
QuadEdge
::IsIsolated() const
{
  const bool isIsolated = ( this == this->GetOnext() );
  return isIsolated; 
}


// ---------------------------------------------------------------------
bool 
QuadEdge
::IsEdgeInOnextRing( Self* testEdge ) const
{
  ConstIterator it = this->BeginOnext(); 
  while( it != this->EndOnext() )
    {
    if( it.Value() == NULL )
      {
      return false;
      }
    if( it.Value() == testEdge ) 
      {
      return true;
      }
    it++;  
    }
  return false;
}


// ---------------------------------------------------------------------
bool 
QuadEdge
::IsLnextGivenSizeCyclic( const int size ) const
{
  // Verify that when iterating size times with Lnext()
  // we end up on "this": this would prove that the size of Lnext()
  // ring is the given argument.
  const Self* iterated = this;

  for( int i = 0; i < size; i++ )
    {
    iterated = iterated->GetLnext();
    if( !iterated )
      {
      return false;
      }
    }
  return ( this == iterated );
}

// ---------------------------------------------------------------------
unsigned int 
QuadEdge
::GetOrder() const
{
  unsigned int order = 1; // count this edge
  const Self * it = this->GetOnext();
  while( it && it != this )
    {
    order++;
    if (it == it->GetOnext() )
      {
      // throw exception to avoid infinite loop
      }
    it = it->GetOnext();
    }
  return order;
}

} // end namespace itk
