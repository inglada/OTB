/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPolarimetricSynthesisFilter_txx
#define __otbPolarimetricSynthesisFilter_txx

#include "otbPolarimetricSynthesisFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction >
PolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::PolarimetricSynthesisFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();
}

 /** PolarimetricSynthesisFilter
   *
   *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template <class TInputImage, class TOutputImage, class TFunction>
void 
PolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
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

  // Set the output image largest possible region.  Use a RegionCopier
  // so that the input and output images can be different dimensions.
  OutputImageRegionType outputLargestPossibleRegion;
  this->CallCopyInputRegionToOutputRegion(outputLargestPossibleRegion,
                                          inputPtr->GetLargestPossibleRegion());
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

  // Set the output spacing and origin
  const itk::ImageBase<Superclass::InputImageDimension> *phyData;

  phyData
    = dynamic_cast<const itk::ImageBase<Superclass::InputImageDimension>*>(this->GetInput());

  if (phyData)
    {
    // Copy what we can from the image from spacing and origin of the input
    // This logic needs to be augmented with logic that select which
    // dimensions to copy
    unsigned int i, j;
    const typename InputImageType::SpacingType&
      inputSpacing = inputPtr->GetSpacing();
    const typename InputImageType::PointType&
      inputOrigin = inputPtr->GetOrigin();
    const typename InputImageType::DirectionType&
      inputDirection = inputPtr->GetDirection();

    typename OutputImageType::SpacingType outputSpacing;
    typename OutputImageType::PointType outputOrigin;
    typename OutputImageType::DirectionType outputDirection;

    // copy the input to the output and fill the rest of the
    // output with zeros.
    for (i=0; i < Superclass::InputImageDimension; ++i)
      {
      outputSpacing[i] = inputSpacing[i];
      outputOrigin[i] = inputOrigin[i];
      for (j=0; j < Superclass::OutputImageDimension; j++)
        {
        if (j < Superclass::InputImageDimension)
          {
          outputDirection[j][i] = inputDirection[j][i];
          }
        else
          {
          outputDirection[j][i] = 0.0;          
          }
        }
      }
    for (; i < Superclass::OutputImageDimension; ++i)
      {
      outputSpacing[i] = 1.0;
      outputOrigin[i] = 0.0;
      for (j=0; j < Superclass::OutputImageDimension; j++)
        {
        if (j == i)
          {
          outputDirection[j][i] = 1.0;
          }
        else
          {
          outputDirection[j][i] = 0.0;          
          }
        }
      }

    // set the spacing and origin
    outputPtr->SetSpacing( outputSpacing );
    outputPtr->SetOrigin( outputOrigin );
    outputPtr->SetDirection( outputDirection );
    outputPtr->SetNumberOfComponentsPerPixel( // propagate vector length info
        inputPtr->GetNumberOfComponentsPerPixel());
    }
  else
    {
    // pointer could not be cast back down
    itkExceptionMacro(<< "otb::MultiChannelRAndNIRVegetationIndexImageFilter::GenerateOutputInformation "
                      << "cannot cast input to "
                      << typeid(itk::ImageBase<Superclass::InputImageDimension>*).name() );
    }
}


/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
PolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{
  InputImagePointer  inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);
  
  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while( !inputIt.IsAtEnd() ) 
    {
    outputIt.Set( m_Functor( inputIt.Get()[0], inputIt.Get()[1]) );
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}

template <class TInputImage, class TOutputImage, class TFunction  >
void
PolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Phi E: "<<m_PhiE<<std::endl;
  os << indent << "Khi E: "<<m_KhiE<<std::endl;
  os << indent << "Phi R: "<<m_PhiR<<std::endl;
  os << indent << "Khi R: "<<m_KhiR<<std::endl;    
}

} // end namespace otb

#endif
