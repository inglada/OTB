/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarRegionBasedLevelSetFunction.txx,v $
  Language:  C++
  Date:      $Date: 2009-07-25 18:52:39 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkScalarRegionBasedLevelSetFunction_txx
#define __itkScalarRegionBasedLevelSetFunction_txx

#include "itkScalarRegionBasedLevelSetFunction.h"

namespace itk {

// Computes the overlap multiplicative factors for the penalty term (sum) and
// the background intensity fitting terms in multiphase level-sets
template < class TInputImage, class TFeatureImage, class TSharedData >
typename ScalarRegionBasedLevelSetFunction< TInputImage, TFeatureImage, TSharedData >::ScalarValueType
ScalarRegionBasedLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::ComputeOverlapParameters( const FeatureIndexType& globalIndex, ScalarValueType& product )
{
// This conditional statement computes the amount of overlap s
// and the presence of background pr
  unsigned int fId = this->m_FunctionId;

  // accumulates the overlap across all functions
  ScalarValueType sum = 0;
  product = 1.;

  ListPixelType L;
  L = this->m_SharedData->m_NearestNeighborListImage->GetPixel( globalIndex );

  InputPixelType hVal;
  InputIndexType otherIndex;

  for ( ListPixelIterator it = L.begin(); it != L.end(); ++it )
    {
    if ( *it != fId )
      {
      otherIndex = this->m_SharedData->m_LevelSetDataPointerVector[*it]->GetIndex( globalIndex );
      hVal = this->m_SharedData->m_LevelSetDataPointerVector[*it]->m_HeavisideFunctionOfLevelSetImage->GetPixel( otherIndex );

      sum += (1 - hVal);
      product *= ( 1 - hVal );
      }
    }

  return sum;
}

/* Performs the narrow-band update of the Heaviside function for each voxel. The
characteristic function of each region is recomputed (note the shared
data which contains information from the other level sets). Using the
new H values, the previous c_i are updated. Used by only the sparse image
filter */
template < class TInputImage, class TFeatureImage, class TSharedData >
void
ScalarRegionBasedLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::UpdatePixel( const unsigned int& idx, NeighborhoodIterator< TInputImage >
&iterator, InputPixelType &newValue, bool & itkNotUsed(status) )
{
  unsigned int fId = this->m_FunctionId;

  // For each affected h val: h val = new hval (this will dirty some cvals)
  InputIndexType inputIndex = iterator.GetIndex( idx );
  FeatureIndexType globalIndex = this->m_SharedData->m_LevelSetDataPointerVector[fId]->GetFeatureIndex( inputIndex );

  FeaturePixelType featureVal = this->m_FeatureImage->GetPixel( inputIndex );

  ScalarValueType oldH = this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_HeavisideFunctionOfLevelSetImage->GetPixel( inputIndex );
  ScalarValueType newH = this->m_DomainFunction->Evaluate( - newValue );
  ScalarValueType change = newH - oldH;

  // update the foreground constant for current level-set function
  UpdateSharedDataInsideParameters( fId, featureVal, change );

  // Compute the product factor
  ListPixelType L = this->m_SharedData->m_NearestNeighborListImage->GetPixel( globalIndex );
  InputIndexType itInputIndex;
  ScalarValueType hVal;

  InputPixelType product = 1;
  for( ListPixelType::const_iterator it = L.begin(); it != L.end(); ++it )
    {
    if (*it != fId )
      {
      itInputIndex = this->m_SharedData->m_LevelSetDataPointerVector[*it]->GetIndex( globalIndex );
      hVal = this->m_SharedData->m_LevelSetDataPointerVector[*it]->m_HeavisideFunctionOfLevelSetImage->GetPixel( itInputIndex );
      product *= ( 1 - hVal );
      }
    }
  // Determine the change in the product factor
  ScalarValueType productChange = -(product * change);

  // update the background constant of all level-set functions
  for( ListPixelType::const_iterator it = L.begin(); it != L.end(); ++it )
    {
    UpdateSharedDataOutsideParameters( *it, featureVal, productChange );
    }

  this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_HeavisideFunctionOfLevelSetImage->SetPixel( inputIndex, newH );
}


} // end namespace itk
#endif
