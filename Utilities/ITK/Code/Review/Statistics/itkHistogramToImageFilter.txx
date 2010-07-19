/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkHistogramToImageFilter.txx,v $
Language:  C++
Date:      $Date: 2010-05-28 01:35:22 $
Version:   $Revision: 1.1 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHistogramToImageFilter_txx
#define __itkHistogramToImageFilter_txx

#include "itkHistogramToImageFilter.h"
#include <itkNumericTraits.h>
#include "itkProgressReporter.h"

namespace itk
{

/** Constructor */
template <class THistogram, unsigned int NDimension, class TFunction >
HistogramToImageFilter<THistogram, NDimension, TFunction>
::HistogramToImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_Size.Fill(0);
  m_Spacing.Fill(1.0);
  m_Origin.Fill(0.0);
}

/** Destructor */
template <class THistogram, unsigned int NDimension, class TFunction >
HistogramToImageFilter<THistogram, NDimension, TFunction>
::~HistogramToImageFilter()
{
}
  

/** Set the Input Histogram */
template <class THistogram, unsigned int NDimension, class TFunction >
void 
HistogramToImageFilter<THistogram, NDimension, TFunction>
::SetInput( const HistogramType *input)
{
  
  // Histograms are not dataobjects, so need to decorate it to push it down
  // the pipeline 
  typename InputHistogramObjectType::Pointer histogramObject = 
    InputHistogramObjectType::New();
  histogramObject->Set( const_cast< HistogramType * >(input) );
  
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0,  histogramObject  );
}


/** Set the Input Histogram if already decorated */
template <class THistogram, unsigned int NDimension, class TFunction >
void 
HistogramToImageFilter<THistogram, NDimension, TFunction>
::SetInput( const InputHistogramObjectType *inputObject)
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0, 
                     const_cast< InputHistogramObjectType * >( inputObject ) );
}

template <class THistogram, unsigned int NDimension, class TFunction >
const typename HistogramToImageFilter< THistogram, NDimension, TFunction >::InputHistogramObjectType * 
HistogramToImageFilter<THistogram, NDimension, TFunction>
::GetInput(void) 
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<const InputHistogramObjectType * >
    (this->ProcessObject::GetInput(0) );
}


template <class THistogram, unsigned int NDimension, class TFunction >
void 
HistogramToImageFilter<THistogram, NDimension, TFunction>
::SetSpacing(const double* spacing)
{
  SpacingType s(spacing);
  this->SetSpacing( s );
}


template <class THistogram, unsigned int NDimension, class TFunction >
void 
HistogramToImageFilter<THistogram, NDimension, TFunction>
::SetOrigin(const double* origin)
{
  PointType p(origin);
  SetOrigin(p);
}


template <class THistogram, unsigned int NDimension, class TFunction >
void 
HistogramToImageFilter<THistogram, NDimension, TFunction>
::SetTotalFrequency( unsigned long n )
{
  if( n < 1 ) 
    {
    itkExceptionMacro("Total frequency in the histogram must be at least 1.");
    }
  
  if( n == this->GetFunctor().GetTotalFrequency() )
    {
    return;
    }
  else
    {
    this->GetFunctor().SetTotalFrequency( n );
    this->Modified();
    }
}


template <class THistogram, unsigned int NDimension, class TFunction >
void
HistogramToImageFilter<THistogram, NDimension, TFunction>
::GenerateOutputInformation()
{
  // Get the input and output pointers 
  // Get from decorator
  const HistogramType *inputHistogram = this->GetInput()->Get();
  OutputImageType     *outputImage    = this->GetOutput();

  
  // Set the image size to the number of bins along each dimension.
  for( unsigned int i=0; i< ImageDimension; i++)
    { 
    m_Size[i]    = inputHistogram->GetSize(i);
    m_Origin[i]  = inputHistogram->GetBinMin(i,0);
    m_Spacing[i] = inputHistogram->GetBinMin(i,1) - m_Origin[i];
    }

  // Set output image params and Allocate image
  typename OutputImageType::RegionType region;
  region.SetSize( m_Size );

  outputImage->SetRegions( region );
  outputImage->SetSpacing( m_Spacing );   // set spacing
  outputImage->SetOrigin(  m_Origin  );   // and origin
}

//----------------------------------------------------------------------------

/** Update */
template <class THistogram, unsigned int NDimension, class TFunction >
void
HistogramToImageFilter<THistogram, NDimension, TFunction>
::GenerateData(void)
{
  itkDebugMacro(<< "HistogramToImageFilter::Update() called");

  this->AllocateOutputs();

  // Get the input and output pointers 
  // Get from decorator
  const HistogramType *inputHistogram = this->GetInput()->Get();
  OutputImageType     *outputImage    = this->GetOutput();
 
  // Set the TotalFrequency in the functor
  this->SetTotalFrequency( static_cast< unsigned long >(
        inputHistogram->GetTotalFrequency() ));
 
  ProgressReporter progress( this, 0,
    outputImage->GetRequestedRegion().GetNumberOfPixels() );

  typedef typename HistogramType::ConstIterator           HistogramIterator;
  typedef typename HistogramType::AbsoluteFrequencyType   AbsoluteFrequencyType;

  HistogramIterator hitr = inputHistogram->Begin();

  // Fill image with frequencies from Histogram
  ImageIteratorType iter( outputImage, outputImage->GetRequestedRegion() );

  while( !iter.IsAtEnd() )
    {
    const AbsoluteFrequencyType & value = hitr.GetFrequency();

    iter.Set( m_Functor( static_cast< unsigned long >( value ) ) );

    ++iter;
    ++hitr;

    progress.CompletedPixel();
    }

} // end update function  


template <class THistogram, unsigned int NDimension, class TFunction >
void 
HistogramToImageFilter<THistogram, NDimension, TFunction>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size : " << m_Size << std::endl;
  os << indent << "Origin: " << m_Origin << std::endl;
  os << indent << "Spacing: " << m_Spacing << std::endl;
  os << indent << "Sum of frequencies of measurement vectors of the histogram: " <<
        m_Functor.GetTotalFrequency() << std::endl;
}


} // end namespace itk

#endif
