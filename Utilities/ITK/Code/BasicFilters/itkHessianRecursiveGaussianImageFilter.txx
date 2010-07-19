/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHessianRecursiveGaussianImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2010-04-23 07:09:41 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHessianRecursiveGaussianImageFilter_txx
#define __itkHessianRecursiveGaussianImageFilter_txx

#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkProgressAccumulator.h"

namespace itk
{


/**
 * Constructor
 */
template <typename TInputImage, typename TOutputImage >
HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::HessianRecursiveGaussianImageFilter()
{

  m_NormalizeAcrossScale = false;

  unsigned int numberOfSmoothingFilters = NumberOfSmoothingFilters;

  for( unsigned int i = 0; i<numberOfSmoothingFilters; i++ )
    {
    GaussianFilterPointer filter = GaussianFilterType::New();
    filter->SetOrder( GaussianFilterType::ZeroOrder );
    filter->SetNormalizeAcrossScale( m_NormalizeAcrossScale );
    filter->ReleaseDataFlagOn();
    m_SmoothingFilters.push_back( filter );
    }

  m_DerivativeFilterA = DerivativeFilterAType::New();
  m_DerivativeFilterB = DerivativeFilterBType::New();

  m_DerivativeFilterA->SetOrder( DerivativeFilterAType::FirstOrder );
  m_DerivativeFilterA->SetNormalizeAcrossScale( m_NormalizeAcrossScale );
  
  m_DerivativeFilterB->SetOrder( DerivativeFilterBType::FirstOrder );
  m_DerivativeFilterB->SetNormalizeAcrossScale( m_NormalizeAcrossScale );
  
  m_DerivativeFilterA->SetInput( this->GetInput() );
  m_DerivativeFilterB->SetInput( m_DerivativeFilterA->GetOutput() );

  // Deal with the 2D case.
  if( numberOfSmoothingFilters > 0 )
    {
    m_SmoothingFilters[0]->SetInput( m_DerivativeFilterB->GetOutput() );
    }

  for( unsigned int i = 1; i<numberOfSmoothingFilters; i++ )
    {
    m_SmoothingFilters[ i ]->SetInput( 
      m_SmoothingFilters[i-1]->GetOutput() );
    }
  
  m_ImageAdaptor = OutputImageAdaptorType::New();

  this->SetSigma( 1.0 );

}

/**
 * Set value of Sigma
 */
template <typename TInputImage, typename TOutputImage>
void 
HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::SetSigma( RealType sigma )
{

  unsigned int numberOfSmoothingFilters = NumberOfSmoothingFilters;

  for( unsigned int i = 0; i<numberOfSmoothingFilters; i++ )
    {
    m_SmoothingFilters[ i ]->SetSigma( sigma );
    }
  m_DerivativeFilterA->SetSigma( sigma );
  m_DerivativeFilterB->SetSigma( sigma );

  this->Modified();

}

/**
 * Set Normalize Across Scale Space
 */
template <typename TInputImage, typename TOutputImage>
void 
HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::SetNormalizeAcrossScale( bool normalize )
{

  m_NormalizeAcrossScale = normalize;

  m_DerivativeFilterA->SetNormalizeAcrossScale( normalize );
  m_DerivativeFilterB->SetNormalizeAcrossScale( normalize );

  this->Modified();

}


//
//
//
template <typename TInputImage, typename TOutputImage>
void
HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion() throw(InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method. this should
  // copy the output requested region to the input requested region
  Superclass::GenerateInputRequestedRegion();

  // This filter needs all of the input
  typename HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage>::InputImagePointer image = const_cast<InputImageType *>( this->GetInput() );
  if (image)
    {
    image->SetRequestedRegion( this->GetInput()->GetLargestPossibleRegion() );
    }
}


//
//
//
template <typename TInputImage, typename TOutputImage>
void
HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *output)
{
  TOutputImage *out = dynamic_cast<TOutputImage*>(output);

  if (out)
    {
    out->SetRequestedRegion( out->GetLargestPossibleRegion() );
    }
}

/**
 * Compute filter for Gaussian kernel
 */
template <typename TInputImage, typename TOutputImage >
void
HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage >
::GenerateData(void)
{

  itkDebugMacro(<< "HessianRecursiveGaussianImageFilter generating data ");


  // Create a process accumulator for tracking the progress of this
  // minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
 
  // Compute the contribution of each filter to the total progress.
  const double weight = 
     1.0 / ( ImageDimension * ( ImageDimension * ( ImageDimension+1 ) / 2 ));

  unsigned int numberOfSmoothingFilters = NumberOfSmoothingFilters;

  for( unsigned int i = 0; i<numberOfSmoothingFilters; i++ )
    {
    progress->RegisterInternalFilter( m_SmoothingFilters[i], weight );
    }
  progress->RegisterInternalFilter( m_DerivativeFilterA, weight );
  progress->RegisterInternalFilter( m_DerivativeFilterB, weight );


  const typename TInputImage::ConstPointer   inputImage( this->GetInput() );

  m_ImageAdaptor->SetImage( this->GetOutput() );

  m_ImageAdaptor->SetLargestPossibleRegion( 
    inputImage->GetLargestPossibleRegion() );

  m_ImageAdaptor->SetBufferedRegion( 
    inputImage->GetBufferedRegion() );

  m_ImageAdaptor->SetRequestedRegion( 
    inputImage->GetRequestedRegion() );

  m_ImageAdaptor->Allocate();

  m_DerivativeFilterA->SetInput( inputImage );
  m_DerivativeFilterB->SetInput( m_DerivativeFilterA->GetOutput() );

  unsigned int element = 0;

  for( unsigned int dima=0; dima < ImageDimension; dima++ )
    {
    for( unsigned int dimb=dima; dimb < ImageDimension; dimb++ )
      {
      // Manage the diagonal in a different way in order to avoid 
      // applying a double smoothing to this direction, and missing
      // to smooth one of the other directions.
      if( dimb == dima )
        {
        m_DerivativeFilterA->SetOrder( DerivativeFilterAType::SecondOrder );
        m_DerivativeFilterB->SetOrder( DerivativeFilterBType::ZeroOrder );

        unsigned int i=0; 
        unsigned int j=0;
        // find the direction for the first filter.
        while( j < ImageDimension ) 
          {
          if( j != dima ) 
            {
            m_DerivativeFilterB->SetDirection( j );
            j++;
            break;
            }
          j++;
          }
        // find the direction for all the other filters
        while( i < numberOfSmoothingFilters )
          {
          while( j < ImageDimension )
            {
            if( j != dima ) 
              {
              m_SmoothingFilters[ i ]->SetDirection( j );
              j++;
              break;
              }
            j++;
            }
          i++;
          }

        m_DerivativeFilterA->SetDirection( dima );
        }
      else
        {
        m_DerivativeFilterA->SetOrder( DerivativeFilterAType::FirstOrder );
        m_DerivativeFilterB->SetOrder( DerivativeFilterBType::FirstOrder );

        unsigned int i=0; 
        unsigned int j=0;
        while( i < numberOfSmoothingFilters )
          {
          while( j < ImageDimension )
            {
            if( j != dima && j != dimb ) 
              {
              m_SmoothingFilters[ i ]->SetDirection( j );
              j++;
              break;
              }
            j++;
            }
            i++;
          }

        m_DerivativeFilterA->SetDirection( dima );
        m_DerivativeFilterB->SetDirection( dimb );
 
        }
     
      typename RealImageType::Pointer derivativeImage; 

      // Deal with the 2D case.
      if( numberOfSmoothingFilters > 0 )
        {
        GaussianFilterPointer lastFilter = m_SmoothingFilters[ImageDimension-3];
        lastFilter->Update();
        derivativeImage = lastFilter->GetOutput(); 
        }
      else
        {
        m_DerivativeFilterB->Update();
        derivativeImage = m_DerivativeFilterB->GetOutput(); 
        }

      progress->ResetFilterProgressAndKeepAccumulatedProgress();

      // Copy the results to the corresponding component
      // on the output image of vectors
      m_ImageAdaptor->SelectNthElement( element++ );


      ImageRegionIteratorWithIndex< RealImageType > it( 
              derivativeImage, 
              derivativeImage->GetRequestedRegion() );

      ImageRegionIteratorWithIndex< OutputImageAdaptorType > ot( 
        m_ImageAdaptor, 
        m_ImageAdaptor->GetRequestedRegion() );
    
      const RealType spacingA = inputImage->GetSpacing()[ dima ];
      const RealType spacingB = inputImage->GetSpacing()[ dimb ];

      const RealType factor = spacingA * spacingB;

      it.GoToBegin();
      ot.GoToBegin();
      while( !it.IsAtEnd() )
        {
        ot.Set( it.Get() / factor );
        ++it;
        ++ot;
        }

      }
    }
  

}


template <typename TInputImage, typename TOutputImage>
void
HessianRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << "NormalizeAcrossScale: " << m_NormalizeAcrossScale << std::endl;
}


} // end namespace itk

#endif
