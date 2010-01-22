/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapOverlayImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelMapOverlayImageFilter_txx
#define __itkLabelMapOverlayImageFilter_txx

#include "itkLabelMapOverlayImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace itk {

template<class TInputImage, class TFeatureImage, class TOutputImage>
LabelMapOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::LabelMapOverlayImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_Opacity = 0.5;
}

template<class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}

template <class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  m_Barrier = Barrier::New();
  m_Barrier->Initialize( this->GetNumberOfThreads() );

  Superclass::BeforeThreadedGenerateData();

}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  OutputImageType * output = this->GetOutput();
  InputImageType * input = const_cast<InputImageType *>(this->GetInput());
  const FeatureImageType * input2 = this->GetFeatureImage();

  FunctorType function;
  function.SetBackgroundValue( input->GetBackgroundValue() );
  function.SetOpacity( m_Opacity );
  
  ImageRegionConstIterator< FeatureImageType > featureIt( input2, outputRegionForThread );
  ImageRegionIterator< OutputImageType > outputIt( output, outputRegionForThread );

  for ( featureIt.GoToBegin(), outputIt.GoToBegin();
        !featureIt.IsAtEnd();
        ++featureIt, ++outputIt )
    {
    outputIt.Set( function( featureIt.Get(), input->GetBackgroundValue() ) );
    }

  // wait for the other threads to complete that part
  m_Barrier->Wait();

  // and delegate to the superclass implementation to use the thread support for the label objects
  Superclass::ThreadedGenerateData( outputRegionForThread, threadId );
}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void
LabelMapOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  OutputImageType * output = this->GetOutput();
  InputImageType * input = const_cast<InputImageType *>(this->GetInput());
  const FeatureImageType * input2 = this->GetFeatureImage();

  FunctorType function;
  function.SetBackgroundValue( input->GetBackgroundValue() );
  function.SetOpacity( m_Opacity );

  const typename LabelObjectType::LabelType & label = labelObject->GetLabel();

  // the user want the mask to be the background of the label collection image
  typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
  typename InputImageType::LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    IndexType idx = lit->GetIndex();
    unsigned long length = lit->GetLength();
    for( unsigned int i=0; i<length; i++)
      {
        output->SetPixel( idx, function( input2->GetPixel(idx), label ) );
        idx[0]++;
      }
    }

}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void
LabelMapOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Opacity: " << m_Opacity << std::endl;
}


}// end namespace itk
#endif
