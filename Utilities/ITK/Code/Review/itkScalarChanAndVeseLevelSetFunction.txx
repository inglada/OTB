/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarChanAndVeseLevelSetFunction.txx,v $
  Language:  C++
  Date:      $Date: 2009-06-12 09:44:02 $
  Version:   $Revision: 1.21 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkScalarChanAndVeseLevelSetFunction_txx
#define __itkScalarChanAndVeseLevelSetFunction_txx

#include "itkScalarChanAndVeseLevelSetFunction.h"

namespace itk {

// Computes the foreground constant and background constant value. For the Dense 
// image filter, this is called prior to the start of every iteration. For the
// sparse filter, this is only called one during initialization.
template < class TInputImage, class TFeatureImage, class TSharedData >
void
ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::UpdateSharedDataParameters()
{
  unsigned int fId = this->m_FunctionId;

  if ( this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedNumberOfPixelsInsideLevelSet > vnl_math::eps )
    {
    this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_ForegroundConstantValues =
      this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedSumOfPixelValuesInsideLevelSet /
      this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedNumberOfPixelsInsideLevelSet;
    }
  else
    {
    this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_ForegroundConstantValues = 0;
    }

  if ( this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedNumberOfPixelsOutsideLevelSet > vnl_math::eps )
    {
    this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_BackgroundConstantValues =
      this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedSumOfPixelValuesOutsideLevelSet /
      this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedNumberOfPixelsOutsideLevelSet;
    }
  else
    {
    this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_BackgroundConstantValues = 0;
    }

}

// update the foreground constant for pixel updates
// Called only when sparse filters are used to prevent iteration through the
// entire image
template < class TInputImage, class TFeatureImage, class TSharedData >
void
ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::UpdateSharedDataInsideParameters( const unsigned int& iId,
    const FeaturePixelType& iVal, const ScalarValueType& iChange )
{
  // update the foreground constant calculation of the current level-set function
  this->m_SharedData->m_LevelSetDataPointerVector[iId]->m_WeightedNumberOfPixelsInsideLevelSet += iChange;
  this->m_SharedData->m_LevelSetDataPointerVector[iId]->m_WeightedSumOfPixelValuesInsideLevelSet += iVal * iChange;
}

// update the background constant for pixel updates
// Called only when sparse filters are used to prevent iteration through the
// entire image
template < class TInputImage, class TFeatureImage, class TSharedData >
void
ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::UpdateSharedDataOutsideParameters( const unsigned int& iId,
    const FeaturePixelType& iVal, const ScalarValueType& iChange )
{
  this->m_SharedData->m_LevelSetDataPointerVector[iId]->m_WeightedNumberOfPixelsOutsideLevelSet += iChange;
  this->m_SharedData->m_LevelSetDataPointerVector[iId]->m_WeightedSumOfPixelValuesOutsideLevelSet += iVal * iChange;
}

/* Calculates the numerator and denominator for c_i for each region. As part of
the optimization, it is called once at the beginning of the code, and then the
cNum and cDen are updated during the evolution without iterating through the
entire image. */
template < class TInputImage, class TFeatureImage, class TSharedData >
void
ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::ComputeParameters()
{
  unsigned int fId = this->m_FunctionId;

  this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedNumberOfPixelsInsideLevelSet = 0;
  this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedSumOfPixelValuesInsideLevelSet = 0;
  this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_ForegroundConstantValues = 0;
  this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedNumberOfPixelsOutsideLevelSet = 0;
  this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedSumOfPixelValuesOutsideLevelSet = 0;
  this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_BackgroundConstantValues = 0;

  ConstFeatureIteratorType fIt( this->m_FeatureImage, this->m_FeatureImage->GetLargestPossibleRegion() );

  FeaturePixelType featureVal;
  FeatureIndexType globalIndex;
  InputIndexType itInputIndex, inputIndex;
  InputPixelType hVal;
  ListPixelType L;

  fIt.GoToBegin();

  while( !fIt.IsAtEnd() )
    {
    featureVal = fIt.Get();
    inputIndex = fIt.GetIndex();
    InputPixelType prod = 1.;

    globalIndex = this->m_SharedData->m_LevelSetDataPointerVector[fId]->GetFeatureIndex( inputIndex );

    L = this->m_SharedData->m_NearestNeighborListImage->GetPixel( globalIndex );

    for( ListPixelConstIterator it = L.begin(); it != L.end(); ++it )
      {
      itInputIndex = this->m_SharedData->m_LevelSetDataPointerVector[*it]->GetIndex( globalIndex );
      hVal = this->m_SharedData->m_LevelSetDataPointerVector[*it]->m_HeavisideFunctionOfLevelSetImage->GetPixel( itInputIndex );
      prod *= ( 1. - hVal );

      if (*it == fId)
        {
        this->m_SharedData->m_LevelSetDataPointerVector[*it]->m_WeightedSumOfPixelValuesInsideLevelSet += featureVal * hVal;
        this->m_SharedData->m_LevelSetDataPointerVector[*it]->m_WeightedNumberOfPixelsInsideLevelSet += hVal;
        }
      }

    this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedSumOfPixelValuesOutsideLevelSet += featureVal * prod;
    this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_WeightedNumberOfPixelsOutsideLevelSet += prod;

    ++fIt;
    }
}


template < class TInputImage, class TFeatureImage, class TSharedData >
typename ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::ScalarValueType
ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >::
ComputeInternalTerm( const FeaturePixelType& iValue,
  const FeatureIndexType& itkNotUsed(iIdx) )
{
  const unsigned int fId = this->m_FunctionId;
  const ScalarValueType cVals = this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_ForegroundConstantValues;
  const ScalarValueType t = ( iValue - cVals );
  return t * t;
}


template < class TInputImage, class TFeatureImage, class TSharedData >
typename ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::ScalarValueType
ScalarChanAndVeseLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
::ComputeExternalTerm( const FeaturePixelType& iValue,
    const FeatureIndexType& itkNotUsed(iIdx) )
{
  const unsigned int fId = this->m_FunctionId;
  const ScalarValueType cBgrnd = this->m_SharedData->m_LevelSetDataPointerVector[fId]->m_BackgroundConstantValues; // background
  const ScalarValueType t = ( iValue - cBgrnd );

  return t * t;
}


} // end namespace itk
#endif
