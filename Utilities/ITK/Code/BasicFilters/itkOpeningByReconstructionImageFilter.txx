/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOpeningByReconstructionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/12/07 22:20:16 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOpeningByReconstructionImageFilter_txx
#define __itkOpeningByReconstructionImageFilter_txx

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkOpeningByReconstructionImageFilter.h"
#include "itkGrayscaleErodeImageFilter.h"
#include "itkReconstructionByDilationImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template <class TInputImage, class TOutputImage, class TKernel>
OpeningByReconstructionImageFilter<TInputImage, TOutputImage, TKernel>
::OpeningByReconstructionImageFilter()
  : m_Kernel()
{
  m_FullyConnected = false;
  m_PreserveIntensities = false;
}

template <class TInputImage, class TOutputImage, class TKernel>
void 
OpeningByReconstructionImageFilter<TInputImage, TOutputImage, TKernel>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TKernel>
void 
OpeningByReconstructionImageFilter<TInputImage, TOutputImage, TKernel>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TKernel>
void 
OpeningByReconstructionImageFilter<TInputImage, TOutputImage, TKernel>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  // Delegate to an erode filter.
  typename GrayscaleErodeImageFilter<TInputImage, TInputImage, TKernel>::Pointer
    erode = GrayscaleErodeImageFilter<TInputImage, TInputImage, TKernel>::New();

  erode->SetInput( this->GetInput() );
  erode->SetKernel( this->m_Kernel );

  // Delegate to a dilate filter.
  typename ReconstructionByDilationImageFilter<TInputImage, TInputImage>::Pointer
  dilate = ReconstructionByDilationImageFilter<TInputImage, TInputImage>::New();

  dilate->SetMarkerImage( erode->GetOutput() );
  dilate->SetMaskImage( this->GetInput() );
  dilate->SetFullyConnected( m_FullyConnected );


  progress->RegisterInternalFilter(erode, 0.5f);
  progress->RegisterInternalFilter(dilate, 0.25f);

  if (m_PreserveIntensities)
    {
    dilate->Update();
    typename TInputImage::Pointer tempImage = TInputImage::New();
    tempImage->SetRegions (erode->GetOutput()->GetBufferedRegion());
    tempImage->Allocate();

    ImageRegionConstIterator<TInputImage> inputIt(this->GetInput(),
                                                  erode->GetOutput()->GetBufferedRegion());
    ImageRegionConstIterator<TInputImage> erodeIt(erode->GetOutput(),
                                                  erode->GetOutput()->GetBufferedRegion());
    ImageRegionConstIterator<TInputImage> dilateIt(dilate->GetOutput(),
                                                   erode->GetOutput()->GetBufferedRegion());
    ImageRegionIterator<TInputImage> tempIt(tempImage,
                                            erode->GetOutput()->GetBufferedRegion());
    while (!erodeIt.IsAtEnd())
      {
      if (erodeIt.Get() == dilateIt.Get())
        {
        tempIt.Set(inputIt.Get());
        }
      else
        {
        tempIt.Set(NumericTraits<InputImagePixelType>::NonpositiveMin());
        }
      ++erodeIt;
      ++dilateIt;
      ++tempIt;
      ++inputIt;
      }

    typename ReconstructionByDilationImageFilter<TInputImage, TInputImage>::Pointer
      dilateAgain = ReconstructionByDilationImageFilter<TInputImage, TInputImage>::New();
    dilateAgain->SetMaskImage (this->GetInput());
    dilateAgain->SetMarkerImage (tempImage);
    dilateAgain->SetFullyConnected( m_FullyConnected );
    dilateAgain->GraftOutput( this->GetOutput() );
    progress->RegisterInternalFilter(dilateAgain, 0.25f);
    dilateAgain->Update();
    this->GraftOutput( dilateAgain->GetOutput() );
    }
  else
    {
    dilate->GraftOutput( this->GetOutput() );
    dilate->Update();
    this->GraftOutput( dilate->GetOutput() );
    }
}

template<class TInputImage, class TOutputImage, class TKernel>
void
OpeningByReconstructionImageFilter<TInputImage, TOutputImage, TKernel>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Kernel: " << m_Kernel << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "PreserveIntensities: "  << m_PreserveIntensities << std::endl;
}

}// end namespace itk
#endif
