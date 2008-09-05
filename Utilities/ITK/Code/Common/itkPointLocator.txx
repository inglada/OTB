/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPointLocator.txx,v $
  Language:  C++
  Date:      $Date: 2006-03-19 04:36:59 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkPointLocator_txx
#define _itkPointLocator_txx
#include "itkPointLocator.h"

namespace itk
{

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
void 
PointLocator<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::InitPointInsertion(PointsContainer *newPts, BoundingBoxPointer)
{
  // Check the input
  m_Points = newPts;
#if 0
  if ( newPts )
    {
    this->Points = newPts;
    }
  else
    {
    //throw an exception and return
    return;
    }
#endif
}

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
void 
PointLocator<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::InitIncrementalPointInsertion(PointsContainer *newPts, BoundingBoxPointer bbox)
{
  // Check the input
  m_Points = newPts;
#if 0
  if ( newPts )
    {
    this->Points = newPts;
    }
  else
    {
    //throw an exception and return
    return;
    }
#endif
}

#if 0
int i;
int maxDivs;
typedef vtkIdList *vtkIdListPtr;
float hmin;
int ndivs[3];
float level;

if ( this->HashTable )
{
  this->FreeSearchStructure();
}
if ( !newPts  )
{
  vtkErrorMacro(<<"Must define points for point insertion");
  return 0;
}
if (this->Points)
{
  this->Points->UnRegister(this);
}
this->Points = newPts;
this->Points->Register(this);

for (i=0; i<3; i++)
{
  this->Bounds[2*i] = bounds[2*i];
  this->Bounds[2*i+1] = bounds[2*i+1];
  if ( this->Bounds[2*i+1] <= this->Bounds[2*i] )
    {
    this->Bounds[2*i+1] = this->Bounds[2*i] + 1.0;
    }
}

if ( this->Automatic && (estNumPts > 0) )
{
  level = (float) estNumPts / this->NumberOfPointsPerBucket;
  level = ::ceil( vcl_pow((double)level,(double)0.33333333) );
  for (i=0; i<3; i++)
    {
    ndivs[i] = (int) level;
    }
} 
else 
{
  for (i=0; i<3; i++)
    {
    ndivs[i] = (int) this->Divisions[i];
    }
}

for (i=0; i<3; i++) 
{
  ndivs[i] = (ndivs[i] > 0 ? ndivs[i] : 1);
  this->Divisions[i] = ndivs[i];
}

this->NumberOfBuckets = ndivs[0]*ndivs[1]*ndivs[2];
this->HashTable = new vtkIdListPtr[this->NumberOfBuckets];
memset (this->HashTable, 0, this->NumberOfBuckets*
        sizeof(vtkIdListPtr));
//
//  Compute width of bucket in three directions
//
for (i=0; i<3; i++) 
{
  this->H[i] = (this->Bounds[2*i+1] - this->Bounds[2*i]) / ndivs[i] ;
}

this->InsertionTol2 = this->Tolerance * this->Tolerance;

for (maxDivs=0, hmin=VTK_LARGE_FLOAT, i=0; i<3; i++) 
{
  hmin = (this->H[i] < hmin ? this->H[i] : hmin);
  maxDivs = (maxDivs > this->Divisions[i] ? maxDivs : this->Divisions[i]);
}
this->InsertionLevel = ::ceil ((double) this->Tolerance / hmin);
this->InsertionLevel = (this->InsertionLevel > maxDivs ? maxDivs : this->InsertionLevel);
return 1;
#endif


/******************************************************************************
 * PROTECTED METHOD DEFINITIONS
 *****************************************************************************/

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
PointLocator<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::PointLocator():
  m_Points(NULL)
{
  m_Divisions = new unsigned long [PointDimension];
  m_NumberOfPointsPerBucket = 3;
}

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
PointLocator<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::~PointLocator()
{
  delete [] m_Divisions;
}

/**
 * Print out the bounding box.
 */
template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
void
PointLocator<TPointIdentifier, VPointDimension, TCoordRep, TPointsContainer >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Divisions: " << m_Divisions << "\n";
  os << indent << "NumberOfPointsPerBucket: " << m_NumberOfPointsPerBucket << "\n";
}

} // end namespace itk

#endif
