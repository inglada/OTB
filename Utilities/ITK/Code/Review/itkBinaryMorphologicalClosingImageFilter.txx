/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryMorphologicalClosingImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-08 20:59:11 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBinaryMorphologicalClosingImageFilter_txx
#define __itkBinaryMorphologicalClosingImageFilter_txx

#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkProgressAccumulator.h"
#include "itkCropImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodIterator.h"
#include "itkProgressReporter.h"

namespace itk {

template<class TInputImage, class TOutputImage, class TKernel>
BinaryMorphologicalClosingImageFilter<TInputImage, TOutputImage, TKernel>
::BinaryMorphologicalClosingImageFilter()
{
  m_ForegroundValue = NumericTraits<InputPixelType>::max();
  m_SafeBorder = true;
}


template<class TInputImage, class TOutputImage, class TKernel>
void
BinaryMorphologicalClosingImageFilter<TInputImage, TOutputImage, TKernel>
::GenerateData()
{
  // Allocate the outputs
  this->AllocateOutputs();
  
  // let choose a background value. Background value should not be given by user
  // because closing is extensive so no background pixels will be added
  // it is just needed for internal erosion filter and constant padder
  InputPixelType backgroundValue = NumericTraits<InputPixelType>::Zero;
  if ( m_ForegroundValue == backgroundValue )
    {
    // current background value is already used for foreground value
    // choose another one
    backgroundValue = NumericTraits<InputPixelType>::max();
    }

  /** set up erosion and dilation methods */
  typename BinaryDilateImageFilter<TInputImage, TInputImage, TKernel>::Pointer
    dilate = BinaryDilateImageFilter<TInputImage, TInputImage, TKernel>::New();

  typename BinaryErodeImageFilter<TInputImage, TOutputImage, TKernel>::Pointer
    erode = BinaryErodeImageFilter<TInputImage, TOutputImage, TKernel>::New();

  // create the pipeline without input and output image
  dilate->ReleaseDataFlagOn();
  dilate->SetKernel( this->GetKernel() );
  dilate->SetDilateValue( m_ForegroundValue );

  erode->SetKernel( this->GetKernel() );
  erode->ReleaseDataFlagOn();
  erode->SetErodeValue( m_ForegroundValue );
  erode->SetBackgroundValue( backgroundValue );
  erode->SetInput( dilate->GetOutput() );

  // now we have 2 cases:
  // + SafeBorder is true so we need to create a bigger image use it as input
  //   and crop the image to the normal output image size
  // + SafeBorder is false; we just have to connect filters
  if ( m_SafeBorder )
    {
    typedef ConstantPadImageFilter<InputImageType, InputImageType> PadType;
    typename PadType::Pointer pad = PadType::New();
    pad->SetPadLowerBound( this->GetKernel().GetRadius().m_Size );
    pad->SetPadUpperBound( this->GetKernel().GetRadius().m_Size );
    pad->SetConstant( backgroundValue );
    pad->SetInput( this->GetInput() );

    dilate->SetInput( pad->GetOutput() );
    
    typedef CropImageFilter<TOutputImage, TOutputImage> CropType;
    typename CropType::Pointer crop = CropType::New();
    crop->SetInput( erode->GetOutput() );
    crop->SetUpperBoundaryCropSize( this->GetKernel().GetRadius() );
    crop->SetLowerBoundaryCropSize( this->GetKernel().GetRadius() );
    
    ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
    progress->SetMiniPipelineFilter(this);
    progress->RegisterInternalFilter(pad, .1f);
    progress->RegisterInternalFilter(erode, .35f);
    progress->RegisterInternalFilter(dilate, .35f);
    progress->RegisterInternalFilter(crop, .1f);
    
    crop->GraftOutput( this->GetOutput() );
    /** execute the minipipeline */
    crop->Update();
  
    /** graft the minipipeline output back into this filter's output */
    this->GraftOutput( crop->GetOutput() );
    }
  else
    {
    /** set up the minipipeline */
    ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
    progress->SetMiniPipelineFilter(this);
    progress->RegisterInternalFilter(erode, .45f);
    progress->RegisterInternalFilter(dilate, .45f);
    
    dilate->SetInput( this->GetInput() );
    erode->GraftOutput( this->GetOutput() );
  
    /** execute the minipipeline */
    erode->Update();
  
    /** graft the minipipeline output back into this filter's output */
    this->GraftOutput( erode->GetOutput() );
    }

  // finally copy background which should have been eroded
  //
  // iterator on input image
  ImageRegionConstIterator<InputImageType> inIt 
            = ImageRegionConstIterator<InputImageType>( this->GetInput(),
                    this->GetOutput()->GetRequestedRegion() );
  // iterator on output image
  ImageRegionIterator<OutputImageType> outIt
            = ImageRegionIterator<OutputImageType>( this->GetOutput(),
                    this->GetOutput()->GetRequestedRegion() );
  outIt.GoToBegin(); 
  inIt.GoToBegin(); 

  ProgressReporter progress2(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels(), 20, 0.9, 0.1);
  while( !outIt.IsAtEnd() )
    {
    if( outIt.Get() != m_ForegroundValue )
      {
      outIt.Set( static_cast<OutputPixelType>( inIt.Get() ) );
      }
    ++outIt;
    ++inIt;
    progress2.CompletedPixel();
    }
  
  // the end !
}

template<class TInputImage, class TOutputImage, class TKernel>
void
BinaryMorphologicalClosingImageFilter<TInputImage, TOutputImage, TKernel>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ForegroundValue: " << static_cast<typename NumericTraits<InputPixelType>::PrintType>(m_ForegroundValue) << std::endl;
  os << indent << "SafeBorder: " << m_SafeBorder << std::endl;
}

}// end namespace itk
#endif
