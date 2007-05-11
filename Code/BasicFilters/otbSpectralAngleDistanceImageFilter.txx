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
#ifndef _otbSpectralAngleDistanceImageFilter_txx
#define _otbSpectralAngleDistanceImageFilter_txx

#include "otbSpectralAngleDistanceImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"
#include "otbMath.h" 

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
SpectralAngleDistanceImageFilter<TInputImage,TOutputImage>
::SpectralAngleDistanceImageFilter()
{
  m_ReferencePixel=0;
}
template <class TInputImage, class TOutputImage>
void
SpectralAngleDistanceImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,int threadId)
{
  
  if (m_ReferencePixel == 0)
    {
      itkExceptionMacro(<<"Reference pixel is not set!");
    }

  InputImageConstPointerType  inputPtr = this->GetInput();
  OutputImagePointerType outputPtr = this->GetOutput();

  
  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while(!inputIt.IsAtEnd() && !outputIt.IsAtEnd()) 
    {
      double dist=0.0;
      double scalarProd=0.0;
      double normProd=0.0;
      double normProd1=0.0;
      double normProd2=0.0;
      InputPixelType pixel = inputIt.Get();
      for (unsigned int i=0; i<pixel.Size(); i++)
	{
	  scalarProd += pixel[i]*m_ReferencePixel[i];
	  normProd1 += pixel[i]*pixel[i];
	  normProd2 += m_ReferencePixel[i]*m_ReferencePixel[i];
	}
      normProd = normProd1 * normProd2;

      if ( normProd == 0.0)
	{
	  dist = 0.0;
	}
      else
	{
	  dist = vcl_acos(scalarProd/vcl_sqrt(normProd));
	}
      // Spectral angle normalisation
      dist = dist/(M_PI/2);
   //square ponderation
   dist = vcl_sqrt(dist);
   outputIt.Set(static_cast<OutputPixelType>(dist));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
SpectralAngleDistanceImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
