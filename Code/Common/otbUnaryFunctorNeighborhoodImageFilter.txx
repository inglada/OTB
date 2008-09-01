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
#ifndef __otbUnaryFunctorNeighborhoodImageFilter_txx
#define __otbUnaryFunctorNeighborhoodImageFilter_txx

#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"
#include "otbMirrorBoundaryCondition.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction  >
UnaryFunctorNeighborhoodImageFilter<TInputImage,TOutputImage,TFunction>
::UnaryFunctorNeighborhoodImageFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();
  m_Radius = 1;
}
template <class TInputImage, class TOutputImage, class TFunction  >
void
UnaryFunctorNeighborhoodImageFilter<TInputImage,TOutputImage,TFunction>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr =
    const_cast< TInputImage * >( this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();
  
  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_Radius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage, class TOutputImage, class TFunction >
void
UnaryFunctorNeighborhoodImageFilter<TInputImage, TOutputImage, TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId)
{ 
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
  TInputImage * inputPtr = static_cast<TInputImage *>( ProcessObjectType::GetInput(0) );

  RadiusType r;
  r.Fill(m_Radius);

  OutputImagePointer outputPtr = this->GetOutput(); 

  itk::ImageRegionIterator<TOutputImage> outputIt;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  NeighborhoodIteratorType neighInputIt( r, inputPtr, outputRegionForThread );
  neighInputIt.OverrideBoundaryCondition( &nbc );
  neighInputIt.GoToBegin();
  
  outputIt = itk::ImageRegionIterator<TOutputImage> ( outputPtr,outputRegionForThread );
  outputIt.GoToBegin();
  
  while ( !outputIt.IsAtEnd() )
    {
      outputIt.Set( m_Functor( neighInputIt ) );
      
      ++neighInputIt;
      ++outputIt;
      
      progress.CompletedPixel();
    }
 }

} // end namespace otb

#endif
