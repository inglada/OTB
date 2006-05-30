/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegionOfInterestImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/02/21 22:25:24 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkRegionOfInterestImageFilter_txx
#define _itkRegionOfInterestImageFilter_txx

#include "itkRegionOfInterestImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"
#include "itkProgressReporter.h"
#include "itkImage.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
RegionOfInterestImageFilter<TInputImage,TOutputImage>
::RegionOfInterestImageFilter() 
{
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
RegionOfInterestImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "RegionOfInterest: " << m_RegionOfInterest << std::endl;
}



template <class TInputImage, class TOutputImage>
void 
RegionOfInterestImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointer to the input 
  typename Superclass::InputImagePointer  inputPtr = 
    const_cast< TInputImage * >( this->GetInput() );

  // request the region of interest
  inputPtr->SetRequestedRegion( m_RegionOfInterest );
}

template <class TInputImage, class TOutputImage>
void 
RegionOfInterestImageFilter<TInputImage,TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *output)
{
  // call the superclass' implementation of this method
  Superclass::EnlargeOutputRequestedRegion(output);
  
  // generate everything in the region of interest
  output->SetRequestedRegionToLargestPossibleRegion();
}




/** 
 * RegionOfInterestImageFilter can produce an image which is a different size
 * than its input image.  As such, RegionOfInterestImageFilter needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template <class TInputImage, class TOutputImage>
void 
RegionOfInterestImageFilter<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
  // do not call the superclass' implementation of this method since
  // this filter allows the input the output to be of different dimensions
 
  // get pointers to the input and output
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();

  if ( !outputPtr || !inputPtr)
    {
    return;
    }

  // Set the output image size to the same value as the region of interest.
  RegionType region;
  IndexType  start;
  start.Fill(0);

  region.SetSize( m_RegionOfInterest.GetSize() );
  region.SetIndex( start );
 
  // Copy Information without modification.
  outputPtr->CopyInformation( inputPtr );

  // Adjust output region
  outputPtr->SetLargestPossibleRegion(region);

  // Correct origin of the extracted region.
  IndexType roiStart( m_RegionOfInterest.GetIndex() );
  typename Superclass::OutputImageType::PointType  outputOrigin;
  typedef Image< ITK_TYPENAME TInputImage::PixelType,
    Superclass::InputImageDimension > ImageType;
  typename ImageType::ConstPointer imagePtr =
    dynamic_cast< const ImageType * >( inputPtr.GetPointer() );
  if ( imagePtr )
    {
    // Input image supports TransformIndexToContinuousPoint
    inputPtr->TransformIndexToPhysicalPoint( roiStart, outputOrigin);
    }
  else
    {
    // Generic type of image
    const typename Superclass::InputImageType::PointType&
      inputOrigin = inputPtr->GetOrigin();

    const typename Superclass::InputImageType::SpacingType&
      spacing = inputPtr->GetSpacing() ;
 
    for( unsigned int i=0; i<ImageDimension; i++)
      {
      outputOrigin[i] = inputOrigin[i] + roiStart[i] * spacing[i];
      }
    }
  
  outputPtr->SetOrigin( outputOrigin );

}



/** 
   * RegionOfInterestImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() 
   */
template <class TInputImage, class TOutputImage>
void 
RegionOfInterestImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread,
                       int threadId)
{
  itkDebugMacro(<<"Actually executing");

  // Get the input and output pointers
  typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();

  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  // Define the portion of the input to walk for this thread
  InputImageRegionType inputRegionForThread;
  inputRegionForThread.SetSize( outputRegionForThread.GetSize() );

  IndexType start;
  IndexType roiStart(    m_RegionOfInterest.GetIndex()    );
  IndexType threadStart( outputRegionForThread.GetIndex() );
  for(unsigned int i=0; i<ImageDimension; i++)
    {
    start[i] = roiStart[i] + threadStart[i];
    }

  inputRegionForThread.SetIndex( start );
  
  // Define the iterators.
  typedef ImageRegionIterator<TOutputImage> OutputIterator;
  typedef ImageRegionConstIterator<TInputImage> InputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);
  InputIterator inIt(inputPtr, inputRegionForThread);

  // walk the output region, and sample the input image
  while( !outIt.IsAtEnd() )
    {
    // copy the input pixel to the output
    outIt.Set( inIt.Get());
    ++outIt; 
    ++inIt; 
    progress.CompletedPixel();
    }
}

} // end namespace itk

#endif
