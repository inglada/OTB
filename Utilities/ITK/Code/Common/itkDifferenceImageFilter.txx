/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDifferenceImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-07 09:09:39 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDifferenceImageFilter_txx
#define __itkDifferenceImageFilter_txx

#include "itkDifferenceImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace itk
{

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
DifferenceImageFilter<TInputImage, TOutputImage>
::DifferenceImageFilter()
{
  // We require two inputs to execute.
  this->SetNumberOfRequiredInputs(2);
  
  // Set the default DifferenceThreshold.
  m_DifferenceThreshold = NumericTraits<OutputPixelType>::Zero;
  
  // Set the default ToleranceRadius.
  m_ToleranceRadius = 0;
  
  // Initialize statistics about difference image.
  m_MeanDifference = NumericTraits<RealType>::Zero;
  m_TotalDifference = NumericTraits<AccumulateType>::Zero;
  m_NumberOfPixelsWithDifferences = 0;
  m_IgnoreBoundaryPixels = false;
}

//----------------------------------------------------------------------------
template<class TInputImage, class TOutputImage>
void 
DifferenceImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "ToleranceRadius: " << m_ToleranceRadius << "\n";  
  os << indent << "DifferenceThreshold: " << m_DifferenceThreshold << "\n";
  os << indent << "MeanDifference: " << m_MeanDifference << "\n";
  os << indent << "TotalDifference: " << m_TotalDifference << "\n";
  os << indent << "NumberOfPixelsWithDifferences: " 
               << m_NumberOfPixelsWithDifferences << "\n";
  os << indent << "IgnoreBoundaryPixels: " 
               << m_IgnoreBoundaryPixels << "\n";
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void 
DifferenceImageFilter<TInputImage, TOutputImage>
::SetValidInput(const InputImageType* validImage)
{
  // The valid image should be input 0.
  this->SetInput(0, validImage);
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void 
DifferenceImageFilter<TInputImage, TOutputImage>
::SetTestInput(const InputImageType* testImage)
{
  // The test image should be input 1.
  this->SetInput(1, testImage);
}

//----------------------------------------------------------------------------
template<class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  int numberOfThreads = this->GetNumberOfThreads();

  // Initialize statistics about difference image.
  m_MeanDifference = NumericTraits<RealType>::Zero;
  m_TotalDifference = NumericTraits<AccumulateType>::Zero;
  m_NumberOfPixelsWithDifferences = 0;
  
  // Resize the thread temporaries
  m_ThreadDifferenceSum.SetSize(numberOfThreads);
  m_ThreadNumberOfPixels.SetSize(numberOfThreads);
  
  // Initialize the temporaries
  m_ThreadDifferenceSum.Fill(NumericTraits<AccumulateType>::Zero);
  m_ThreadNumberOfPixels.Fill(0);
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType &threadRegion, int threadId)
{
  typedef ConstNeighborhoodIterator<InputImageType>   SmartIterator;
  typedef ImageRegionConstIterator<InputImageType>    InputIterator;
  typedef ImageRegionIterator<OutputImageType>        OutputIterator;
  typedef NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> 
                                                      FacesCalculator;
  typedef typename FacesCalculator::RadiusType        RadiusType;
  typedef typename FacesCalculator::FaceListType      FaceListType;
  typedef typename FaceListType::iterator             FaceListIterator;
  typedef typename InputImageType::PixelType          InputPixelType;
 
  // Prepare standard boundary condition.
  ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

  // Get a pointer to each image.
  const InputImageType* validImage = this->GetInput(0);
  const InputImageType* testImage = this->GetInput(1);
  OutputImageType* outputPtr = this->GetOutput();
  
  // Create a radius of pixels.
  RadiusType radius;
  if(m_ToleranceRadius > 0)
    {
    radius.Fill(m_ToleranceRadius);
    }
  else
    {
    radius.Fill(0);
    }
  
  // Find the data-set boundary faces.
  FacesCalculator boundaryCalculator;
  FaceListType faceList = boundaryCalculator(testImage, threadRegion, radius);
  
  // Support progress methods/callbacks.
  ProgressReporter progress(this, threadId, threadRegion.GetNumberOfPixels());
  
  // Process the internal face and each of the boundary faces.
  for (FaceListIterator face = faceList.begin(); face != faceList.end(); ++face)
    { 
    SmartIterator test(radius, testImage, *face); // Iterate over test image.
    InputIterator valid(validImage, *face);       // Iterate over valid image.
    OutputIterator out(outputPtr, *face);         // Iterate over output image.
    if( !test.GetNeedToUseBoundaryCondition() || !m_IgnoreBoundaryPixels )
      {
      test.OverrideBoundaryCondition(&nbc);
    
      for(valid.GoToBegin(), test.GoToBegin(), out.GoToBegin();
          !valid.IsAtEnd();
          ++valid, ++test, ++out)
        {
        // Get the current valid pixel.
        InputPixelType t = valid.Get();
        
        //  Assume a good match - so test center pixel first, for speed
        RealType difference = static_cast<RealType>(t) - test.GetCenterPixel();
        if(NumericTraits<RealType>::IsNegative(difference))
          {
          difference = -difference;
          }
        OutputPixelType minimumDifference = static_cast<OutputPixelType>(difference);
  
        // If center pixel isn't good enough, then test the neighborhood
        if(minimumDifference > m_DifferenceThreshold)
          {
          unsigned int neighborhoodSize = test.Size();
          // Find the closest-valued pixel in the neighborhood of the test
          // image.
          for (unsigned int i=0; i < neighborhoodSize; ++i)
            {
            // Use the RealType for the difference to make sure we get the
            // sign.
            RealType differenceReal = static_cast<RealType>(t) - test.GetPixel(i);
            if(NumericTraits<RealType>::IsNegative(differenceReal))
              {
              differenceReal = -differenceReal;
              }
            OutputPixelType d = static_cast<OutputPixelType>(differenceReal);
            if(d < minimumDifference)
              {
              minimumDifference = d;
              if(minimumDifference <= m_DifferenceThreshold)
                {
                break;
                }
              }
            }
          }
          
        // Check if difference is above threshold.
        if(minimumDifference > m_DifferenceThreshold)
          {
          // Store the minimum difference value in the output image.
          out.Set(minimumDifference);
          
          // Update difference image statistics.
          m_ThreadDifferenceSum[threadId] += minimumDifference;
          m_ThreadNumberOfPixels[threadId]++;
          }
        else
          {
          // Difference is below threshold.
          out.Set(NumericTraits<OutputPixelType>::Zero);
          }
        
        // Update progress.
        progress.CompletedPixel();
        }
      }
    else
      {
      for(out.GoToBegin(); !out.IsAtEnd(); ++out)
        {
        out.Set(NumericTraits<OutputPixelType>::Zero);
        progress.CompletedPixel();
        }
      }
    }
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::AfterThreadedGenerateData()
{
  // Set statistics about difference image.
  int numberOfThreads = this->GetNumberOfThreads();
  for(int i=0; i < numberOfThreads; ++i)
    {
    m_TotalDifference += m_ThreadDifferenceSum[i];
    m_NumberOfPixelsWithDifferences += m_ThreadNumberOfPixels[i];
    }
  
  // Get the total number of pixels processed in the region.
  // This is different from the m_TotalNumberOfPixels which
  // is the number of pixels that actually have differences
  // above the intensity threshold.
  OutputImageRegionType region = this->GetOutput()->GetRequestedRegion();
  AccumulateType numberOfPixels = region.GetNumberOfPixels();
  
  // Calculate the mean difference.
  m_MeanDifference = m_TotalDifference / numberOfPixels;
}

} // end namespace itk

#endif
