/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVotingBinaryHoleFillingImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/16 17:24:32 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkVotingBinaryHoleFillingImageFilter_txx
#define _itkVotingBinaryHoleFillingImageFilter_txx
#include "itkVotingBinaryHoleFillingImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

#include <vector>
#include <algorithm>

namespace itk
{

template <class TInputImage, class TOutputImage>
VotingBinaryHoleFillingImageFilter<TInputImage, TOutputImage>
::VotingBinaryHoleFillingImageFilter()
{
  this->SetSurvivalThreshold(0);
  this->m_MajorityThreshold = 1;
  this->m_NumberOfPixelsChanged = 0;
}


template< class TInputImage, class TOutputImage>
void
VotingBinaryHoleFillingImageFilter< TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  unsigned int threshold = 1;
  
  InputSizeType radius = this->GetRadius();

  for(unsigned int i=0; i<InputImageDimension; i++)
    {
    threshold *= (2*radius[i] + 1 );
    }

  // remove central pixel and take 50%
  threshold = static_cast<unsigned int>( (threshold - 1 ) / 2.0 ); 

  // add the majority threshold.
  threshold += this->GetMajorityThreshold();

  this->SetBirthThreshold( threshold );
  this->SetSurvivalThreshold(0);

  this->m_NumberOfPixelsChanged = 0;

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  this->m_Count.SetSize(numberOfThreads);
  for(unsigned int i=0; i<numberOfThreads; i++)
    {
    this->m_Count[i] = 0;
    }
}


template< class TInputImage, class TOutputImage>
void
VotingBinaryHoleFillingImageFilter< TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  
  ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

  ConstNeighborhoodIterator<InputImageType> bit;
  ImageRegionIterator<OutputImageType> it;
  
  // Allocate output
  typename OutputImageType::Pointer output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();
  
  // Find the data-set boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, this->GetRadius() );

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  const InputPixelType backgroundValue = this->GetBackgroundValue();
  const InputPixelType foregroundValue = this->GetForegroundValue();
  const unsigned int   birthThreshold  = this->GetBirthThreshold();

  unsigned int numberOfPixelsChanged = 0;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    { 
    bit = ConstNeighborhoodIterator<InputImageType>(this->GetRadius(), input, *fit);
    it  = ImageRegionIterator<OutputImageType>(output, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    
    unsigned int neighborhoodSize = bit.Size();

    while ( ! bit.IsAtEnd() )
      {
      const InputPixelType inpixel = bit.GetCenterPixel();

      if( inpixel == backgroundValue )
        {
        // count the pixels ON in the neighborhood
        unsigned int count = 0;
        for (unsigned int i = 0; i < neighborhoodSize; ++i)
          {
          InputPixelType value = bit.GetPixel(i);
          if( value == foregroundValue )
            {
            count++;
            }
          }

        if( count >= birthThreshold )
          {
          it.Set( static_cast<OutputPixelType>( foregroundValue ) );
          numberOfPixelsChanged++;
          }
        else 
          {
          it.Set( static_cast<OutputPixelType>( backgroundValue ) );
          }
        } 
      else
        {
        it.Set( static_cast<OutputPixelType>( foregroundValue ) );
        }
      ++bit;
      ++it;
      progress.CompletedPixel();
      }
    }
  this->m_Count[threadId] = numberOfPixelsChanged;
}



template< class TInputImage, class TOutputImage>
void
VotingBinaryHoleFillingImageFilter< TInputImage, TOutputImage>
::AfterThreadedGenerateData()
{
  this->m_NumberOfPixelsChanged = 0;
  unsigned int numberOfThreads = this->GetNumberOfThreads();
  this->m_Count.SetSize(numberOfThreads);
  for(unsigned int t=0; t<numberOfThreads; t++)
    {
    this->m_NumberOfPixelsChanged += this->m_Count[t];
    }
}



/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
VotingBinaryHoleFillingImageFilter<TInputImage, TOutput>
::PrintSelf(
  std::ostream& os, 
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Majority Threshold           : " << m_MajorityThreshold << std::endl;
  os << indent << "Number of Pixels Changed     : " << m_NumberOfPixelsChanged << std::endl;
}

} // end namespace itk

#endif
