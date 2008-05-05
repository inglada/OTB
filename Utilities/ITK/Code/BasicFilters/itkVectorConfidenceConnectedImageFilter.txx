/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorConfidenceConnectedImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-01-18 20:07:32 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorConfidenceConnectedImageFilter_txx_
#define __itkVectorConfidenceConnectedImageFilter_txx_

#include "itkVectorConfidenceConnectedImageFilter.h"
#include "itkExceptionObject.h"
#include "itkImageRegionIterator.h"
#include "itkVectorMeanImageFunction.h"
#include "itkCovarianceImageFunction.h"
#include "itkBinaryThresholdImageFunction.h"
#include "itkFloodFilledImageFunctionConditionalIterator.h"
#include "itkFloodFilledImageFunctionConditionalConstIterator.h"
#include "itkNumericTraits.h"
#include "itkNumericTraitsRGBPixel.h"
#include "itkProgressReporter.h"

namespace itk
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
VectorConfidenceConnectedImageFilter<TInputImage, TOutputImage>
::VectorConfidenceConnectedImageFilter()
{
  m_Multiplier = 2.5;
  m_NumberOfIterations = 4;
  m_Seeds.clear();
  m_InitialNeighborhoodRadius = 1;
  m_ReplaceValue = NumericTraits<OutputImagePixelType>::One;
  m_ThresholdFunction = DistanceThresholdFunctionType::New();
}

/**
 * Standard PrintSelf method.
 */
template <class TInputImage, class TOutputImage>
void
VectorConfidenceConnectedImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Number of iterations: " << m_NumberOfIterations
     << std::endl;
  os << indent << "Multiplier for confidence interval: " << m_Multiplier
     << std::endl;
  os << indent << "ReplaceValue: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_ReplaceValue)
     << std::endl;
  os << indent << "InitialNeighborhoodRadius: " << m_InitialNeighborhoodRadius 
     << std::endl;
     
}

template <class TInputImage, class TOutputImage>
void 
VectorConfidenceConnectedImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() )
    {
    InputImagePointer input =
      const_cast< TInputImage * >( this->GetInput() );
    input->SetRequestedRegionToLargestPossibleRegion();
    }
}

template <class TInputImage, class TOutputImage>
void 
VectorConfidenceConnectedImageFilter<TInputImage,TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *output)
{
  Superclass::EnlargeOutputRequestedRegion(output);
  output->SetRequestedRegionToLargestPossibleRegion();
}

template <class TInputImage, class TOutputImage>
void 
VectorConfidenceConnectedImageFilter<TInputImage,TOutputImage>
::GenerateData()
{
  typedef typename InputImageType::PixelType   InputPixelType;

  typedef BinaryThresholdImageFunction<OutputImageType>               SecondFunctionType;
  typedef FloodFilledImageFunctionConditionalIterator<OutputImageType, DistanceThresholdFunctionType> IteratorType;
  typedef FloodFilledImageFunctionConditionalConstIterator<InputImageType, SecondFunctionType> SecondIteratorType;

  unsigned int loop;
  
  typename Superclass::InputImageConstPointer inputImage  = this->GetInput();
  typename Superclass::OutputImagePointer     outputImage = this->GetOutput();

  // Zero the output
  OutputImageRegionType region = outputImage->GetRequestedRegion();
  outputImage->SetBufferedRegion( region );
  outputImage->Allocate();
  outputImage->FillBuffer ( NumericTraits<OutputImagePixelType>::Zero );

  // Compute the statistics of the seed point
  typedef VectorMeanImageFunction<InputImageType> VectorMeanImageFunctionType;
  typename VectorMeanImageFunctionType::Pointer meanFunction
                                          = VectorMeanImageFunctionType::New();

  meanFunction->SetInputImage( inputImage );
  meanFunction->SetNeighborhoodRadius( m_InitialNeighborhoodRadius );
  typedef CovarianceImageFunction<InputImageType> CovarianceImageFunctionType;
  typename CovarianceImageFunctionType::Pointer varianceFunction
                                          = CovarianceImageFunctionType::New();
  varianceFunction->SetInputImage( inputImage );
  varianceFunction->SetNeighborhoodRadius( m_InitialNeighborhoodRadius );
  
  // Set up the image function used for connectivity
  m_ThresholdFunction->SetInputImage ( inputImage );

  CovarianceMatrixType covariance;
  MeanVectorType       mean;

  typedef typename InputPixelType::ValueType                      ComponentPixelType;
  typedef typename NumericTraits< ComponentPixelType >::RealType  ComponentRealType;

  const unsigned int dimension = InputPixelType::Dimension;

  covariance = CovarianceMatrixType( dimension, dimension );
  mean       = MeanVectorType( dimension );

  covariance.fill( NumericTraits<ComponentRealType>::Zero );
  mean.fill(  NumericTraits<ComponentRealType>::Zero );

  typedef typename VectorMeanImageFunctionType::OutputType MeanFunctionVectorType;
  typedef typename CovarianceImageFunctionType::OutputType CovarianceFunctionMatrixType;

  typename SeedsContainerType::const_iterator si = m_Seeds.begin();
  typename SeedsContainerType::const_iterator li = m_Seeds.end();
  while( si != li )
    {
    const MeanFunctionVectorType       meanContribution       = meanFunction->EvaluateAtIndex( *si );
    const CovarianceFunctionMatrixType covarianceContribution = varianceFunction->EvaluateAtIndex( *si );
    for(unsigned int ii=0; ii < dimension; ii++)
      {
      mean[ ii ]     += meanContribution[ ii ];
      for(unsigned int jj=0; jj < dimension; jj++)
        {
        covariance[ ii ][ jj ] += covarianceContribution[ ii ][ jj ];
        }
      }
    si++;
    }
  for(unsigned int ik=0; ik < dimension; ik++)
    {
    mean[ ik ]     /= m_Seeds.size();
    for(unsigned int jk=0; jk < dimension; jk++)
      {
      covariance[ ik ][ jk ] /= m_Seeds.size();
      }
    }

  m_ThresholdFunction->SetMean( mean );
  m_ThresholdFunction->SetCovariance( covariance );

  m_ThresholdFunction->SetThreshold( m_Multiplier );

  itkDebugMacro(<< "\nMultiplier originally = " << m_Multiplier );


  // Make sure that the multiplier is large enough to include the seed points themselves.
  // This is a pragmatic fix, but a questionable practice because it means that the actual
  // region may be grown using a multiplier different from the one specified by the user.
  si = m_Seeds.begin();
  li = m_Seeds.end();
  while( si != li )
    {
    const double distance = 
      m_ThresholdFunction->EvaluateDistanceAtIndex( *si );
    if( distance >  m_Multiplier )
      {
      m_Multiplier = distance;
      }
    si++;
    }

  // Finally setup the eventually modified multiplier. That is actually the threshold itself.
  m_ThresholdFunction->SetThreshold( m_Multiplier );

  itkDebugMacro(<< "\nMultiplier after verifying seeds inclusion = " << m_Multiplier );



  // Segment the image, the iterator walks the output image (so Set()
  // writes into the output image), starting at the seed point.  As
  // the iterator walks, if the corresponding pixel in the input image
  // (accessed via the "m_ThresholdFunction" assigned to the iterator) is within
  // the [lower, upper] bounds prescribed, the pixel is added to the
  // output segmentation and its neighbors become candidates for the
  // iterator to walk.
  IteratorType it = IteratorType ( outputImage, m_ThresholdFunction, m_Seeds );
  it.GoToBegin();
  while( !it.IsAtEnd())
    {
    it.Set(m_ReplaceValue);
    ++it;
    }

  ProgressReporter progress(this, 0, region.GetNumberOfPixels() * m_NumberOfIterations );

  for (loop = 0; loop < m_NumberOfIterations; ++loop)
    {
    // Now that we have an initial segmentation, let's recalculate the
    // statistics.  Since we have already labelled the output, we visit
    // pixels in the input image that have been set in the output image.
    // Essentially, we flip the iterator around, so we walk the input
    // image (so Get() will get pixel values from the input) and constrain
    // iterator such it only visits pixels that were set in the output.
    typename SecondFunctionType::Pointer secondFunction = SecondFunctionType::New();
    secondFunction->SetInputImage ( outputImage );
    secondFunction->ThresholdBetween( m_ReplaceValue, m_ReplaceValue );

    covariance = CovarianceMatrixType( dimension, dimension );
    mean       = MeanVectorType( dimension );

    covariance.fill( NumericTraits<ComponentRealType>::Zero );
    mean.fill(  NumericTraits<ComponentRealType>::Zero );

    unsigned long num = 0;
    
    SecondIteratorType sit
      = SecondIteratorType ( inputImage, secondFunction, m_Seeds );
    sit.GoToBegin();
    while( !sit.IsAtEnd())
      {
      const InputPixelType pixelValue = sit.Get();
      for(unsigned int i=0; i<dimension; i++)
        {
        const ComponentRealType pixelValueI = static_cast<ComponentRealType>( pixelValue[i] );
        covariance[i][i] += pixelValueI * pixelValueI;
        mean[i]          += pixelValueI;
        for(unsigned int j=i+1; j<dimension; j++)
          {
          const ComponentRealType pixelValueJ = static_cast<ComponentRealType>( pixelValue[j] );
          const ComponentRealType product = pixelValueI * pixelValueJ;
          covariance[i][j] += product;
          covariance[j][i] += product;
          }
        }
      ++num;
      ++sit;
      }
    for(unsigned int ii=0; ii<dimension; ii++)
      {
      mean[ii] /= static_cast<double>(num);
      for(unsigned int jj=0; jj<dimension; jj++)
        {
        covariance[ii][jj] /= static_cast<double>(num);
        }
      }

    for(unsigned int ik=0; ik<dimension; ik++)
      {
      for(unsigned int jk=0; jk<dimension; jk++)
        {
        covariance[ik][jk] -= mean[ik] * mean[jk];
        }
      }

    m_ThresholdFunction->SetMean( mean );
    m_ThresholdFunction->SetCovariance( covariance );
    
    
    // Rerun the segmentation, the iterator walks the output image,
    // starting at the seed point.  As the iterator walks, if the
    // corresponding pixel in the input image (accessed via the
    // "m_ThresholdFunction" assigned to the iterator) is within the [lower,
    // upper] bounds prescribed, the pixel is added to the output
    // segmentation and its neighbors become candidates for the
    // iterator to walk.
    outputImage->FillBuffer ( NumericTraits<OutputImagePixelType>::Zero );
    IteratorType thirdIt = IteratorType ( outputImage, m_ThresholdFunction, m_Seeds );
    thirdIt.GoToBegin();
    try
      {
      while( !thirdIt.IsAtEnd())
        {
        thirdIt.Set(m_ReplaceValue);
        ++thirdIt;
        progress.CompletedPixel();  // potential exception thrown here
        }

      } 
    catch( ProcessAborted & )
      {
      break; // interrupt the iterations loop
      }
    
    }  // end iteration loop

  if( this->GetAbortGenerateData() )
    {
    ProcessAborted e(__FILE__,__LINE__);
    e.SetDescription("Process aborted.");
    e.SetLocation(ITK_LOCATION);
    throw e;
    }
}


template <class TInputImage, class TOutputImage>
const typename 
VectorConfidenceConnectedImageFilter<TInputImage,TOutputImage>::CovarianceMatrixType &
VectorConfidenceConnectedImageFilter<TInputImage,TOutputImage>
::GetCovariance() const
{
  return m_ThresholdFunction->GetCovariance();
}




template <class TInputImage, class TOutputImage>
const typename 
VectorConfidenceConnectedImageFilter<TInputImage,TOutputImage>::MeanVectorType &
VectorConfidenceConnectedImageFilter<TInputImage,TOutputImage>
::GetMean() const
{
  return m_ThresholdFunction->GetMean();
}


} // end namespace itk

#endif
