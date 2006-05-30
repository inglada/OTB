/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOrthogonalSwath2DPathFilter.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:37 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _itkOrthogonalSwath2DPathFilter_txx
#define _itkOrthogonalSwath2DPathFilter_txx

#include "itkOrthogonalSwath2DPathFilter.h"
#include "vnl/vnl_math.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template <class TParametricPath, class TSwathMeritImage>
OrthogonalSwath2DPathFilter<TParametricPath, TSwathMeritImage>
::OrthogonalSwath2DPathFilter()
{
  SizeType size;
  
  // Initialize the member variables
  size[0]=0;
  size[1]=0;
  m_SwathSize = size;
  m_StepValues  = NULL;
  m_MeritValues = NULL;
  m_OptimumStepsValues = NULL;
  m_FinalOffsetValues = OrthogonalCorrectionTableType::New();
}


/**
 * Destructor
 */
template <class TParametricPath, class TSwathMeritImage>
OrthogonalSwath2DPathFilter<TParametricPath, TSwathMeritImage>
::~OrthogonalSwath2DPathFilter()
{
  if(m_StepValues)          delete [] m_StepValues;
  if(m_MeritValues)         delete [] m_MeritValues;
  if(m_OptimumStepsValues)  delete [] m_OptimumStepsValues;
}


/**
 * GenerateData Performs the reflection
 */
template <class TParametricPath, class TSwathMeritImage>
void
OrthogonalSwath2DPathFilter<TParametricPath, TSwathMeritImage>
::GenerateData( void )
{
  // Get a convenience pointer 
  ImageConstPointer swathMeritImage = this->GetImageInput();
  
  // Re-initialize the member variables
  m_SwathSize = swathMeritImage->GetLargestPossibleRegion().GetSize();
  if(m_StepValues)          delete [] m_StepValues;
  if(m_MeritValues)         delete [] m_MeritValues;
  if(m_OptimumStepsValues)  delete [] m_OptimumStepsValues;
  m_StepValues = new int[ m_SwathSize[0] * m_SwathSize[1] * m_SwathSize[1] ];
  m_MeritValues= new double[ m_SwathSize[0] * m_SwathSize[1] * m_SwathSize[1] ];
  m_OptimumStepsValues = new int[ m_SwathSize[0] ];
  m_FinalOffsetValues->Initialize();
  
  
  // Perform the remaining calculations; use dynamic programming
  
  // current swath column (all previous columns have been fully processed)
  unsigned int x;
  // current first row and last row of the swath.
  unsigned int F,L;
  // index used to access the processed swath image; filled in with x, F, & L
  IndexType index;
  
  // CalcFirstStep (x=0)
  // Enter the initial merit values
  index[0]=0;
  for(F=0;F<m_SwathSize[1];F++) for(L=0;L<m_SwathSize[1];L++)
    {
    if(F==L)
      {
      index[1]=F;
      MeritValue(F,L,0) = (double) swathMeritImage->GetPixel(index);
      StepValue( F,L,0) = F;
      }
    else
      {
      MeritValue(F,L,0) = NumericTraits<double>::NonpositiveMin();
      StepValue( F,L,0) = F;
      }
    }
  // end of double for-loop covering F & L
  
  

  // PrepForRemainingSteps
  for(F=0;F<m_SwathSize[1];F++) for(L=0;L<m_SwathSize[1];L++)
    {
    // find merit for x=1
    if( vnl_math_abs(F-L) <= 1 )
      {
      IndexType index2; // we need a second index here
      index[0]=0;
      index[1]=F;
      index2[0]=1;
      index2[1]=L;
      // Here we know in advance that Pixel(0,F) = Max(l=L-1..L+1){Merit(F,l,0)}
      MeritValue(F,L,1) = double( swathMeritImage->GetPixel(index)
                                + swathMeritImage->GetPixel(index2) );
      }
    else
      {
      MeritValue(F,L,1) = NumericTraits<double>::NonpositiveMin();
      }
    // Enter the final step values (x=SWATH_COLUMNS-1)
    StepValue(F,L,m_SwathSize[0]-1) = L;
    }
  // end of double for-loop covering F & L
  
  // CalcRestPath
  for(x=1;x<m_SwathSize[0]-1;x++)
  for(F=0;F<m_SwathSize[1];  F++)
  for(L=0;L<m_SwathSize[1];  L++)
    {
    int bestL = FindAndStoreBestErrorStep(x,F,L);
    index[0]=x+1;
    index[1]=L;
    MeritValue(F,L,x+1) = MeritValue(F,bestL,x) +
                          double( swathMeritImage->GetPixel(index) );
    }
  // end of tripple for-loop covering x & F & L
  
  
  // Find the best starting and ending points (F & L) for the path
  int bestF, bestL;
  double meritTemp, meritMax=NumericTraits<double>::NonpositiveMin();
  for(F=0;F<m_SwathSize[1];F++) for(L=0;L<m_SwathSize[1];L++)
    {
    if( vnl_math_abs(F-L) <= 1 ) // only accept closed paths
      {
      meritTemp = MeritValue( F, L, m_SwathSize[0]-1 );
      if( meritTemp > meritMax )
        {
        meritMax = meritTemp;
        bestF = F;
        bestL = L;
        }
      }
    }
  // end of double for-loop covering F & L
  
  // Fill in the optimum path error-step (orthogonal correction) values
  m_OptimumStepsValues[ m_SwathSize[0]-1 ] = bestL;
  for(x=m_SwathSize[0]-2; ; x--)
    {
    m_OptimumStepsValues[x] = StepValue(bestF, m_OptimumStepsValues[x+1], x);
    if( 0==x ) break;
    }
  
  // Convert from absolute indicies to +/- orthogonal offset values
  for(x=0;x<m_SwathSize[0];x++)
    {
    m_FinalOffsetValues->InsertElement( 
        x,  double( m_OptimumStepsValues[x] - int(m_SwathSize[1]/2) )  );
    }
  
  // setup the output path
  OutputPathPointer outputPtr = this->GetOutput(0);
  outputPtr->SetOriginalPath(this->GetPathInput());
  outputPtr->SetOrthogonalCorrectionTable(m_FinalOffsetValues);
}

template <class TParametricPath, class TSwathMeritImage>
void
OrthogonalSwath2DPathFilter<TParametricPath, TSwathMeritImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "StepValues:  " << m_StepValues << std::endl;
  os << indent << "MeritValues:  " << m_MeritValues << std::endl;
  os << indent << "OptimumStepsValues:  " << m_OptimumStepsValues << std::endl;
  os << indent << "FinalOffsetValues:  " << m_FinalOffsetValues << std::endl;
}

// The next three functions are private helper functions
template <class TParametricPath, class TSwathMeritImage>
unsigned int
OrthogonalSwath2DPathFilter<TParametricPath, TSwathMeritImage>
::FindAndStoreBestErrorStep(unsigned int x, unsigned int F, unsigned int L)
{
  unsigned int bestL; // L with largest merit of L and its 2 neighbors L-1 & L+1
  
  // Handle perimeter boundaries of the vert. gradient image
  if(L==0)
    {
    if( MeritValue(F,L+1,x) > MeritValue(F,L,x) )
      bestL = L+1;
    else
      bestL = L;
    }
  else if(L==m_SwathSize[1]-1)
    {
    if( MeritValue(F,L-1,x) > MeritValue(F,L,x) )
      bestL = L-1;
    else
      bestL = L;
    }
  else
    // We are now free to consider all 3 cases for bestL
    {
    if(      MeritValue(F,L+1,x) > MeritValue(F,L,x)
          && MeritValue(F,L+1,x) > MeritValue(F,L-1,x) )
      {
      bestL = L+1;
      }
    else if( MeritValue(F,L-1,x) > MeritValue(F,L,x)
          && MeritValue(F,L-1,x) > MeritValue(F,L+1,x) )
      {
      bestL = L-1;
      }
    else
      {
      bestL = L;
      }
    }
  StepValue(F,L,x) = bestL;
  return bestL;
}
template <class TParametricPath, class TSwathMeritImage>
inline int &
OrthogonalSwath2DPathFilter<TParametricPath, TSwathMeritImage>
::StepValue(int f, int l, int x)
{
  int rows=m_SwathSize[1];
  return m_StepValues[ (x*rows*rows) + (f*rows) + (l) ];
}
template <class TParametricPath, class TSwathMeritImage>
inline double &
OrthogonalSwath2DPathFilter<TParametricPath, TSwathMeritImage>
::MeritValue(int f, int l, int x)
{
  int rows=m_SwathSize[1];
  return m_MeritValues[ (x*rows*rows) + (f*rows) + (l) ];
}

} // end namespace itk

#endif
