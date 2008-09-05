/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransformToDeformationFieldSource.txx,v $
  Language:  C++
  Date:      $Date: 2008-07-14 17:09:06 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
/*======================================================================

This file is part of the elastix software.

Copyright (c) University Medical Center Utrecht. All rights reserved.
See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the above copyright notices for more information.

======================================================================*/
#ifndef __itkTransformToDeformationFieldSource_txx
#define __itkTransformToDeformationFieldSource_txx

#include "itkTransformToDeformationFieldSource.h"

#include "itkIdentityTransform.h"
#include "itkProgressReporter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"

namespace itk
{

/**
 * Constructor
 */
template <class TOutputImage, class TTransformPrecisionType>
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::TransformToDeformationFieldSource()
{
  this->m_OutputSpacing.Fill(1.0);
  this->m_OutputOrigin.Fill(0.0);
  this->m_OutputDirection.SetIdentity();

  this->m_OutputSize.Fill( 0 );
  this->m_OutputIndex.Fill( 0 );
  
  this->m_Transform = IdentityTransform<TTransformPrecisionType, ImageDimension>::New();

} // end Constructor


/**
 * Print out a description of self
 *
 * \todo Add details about this class
 */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::PrintSelf( std::ostream & os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
  
  os << indent << "OutputSize: " << this->m_OutputSize << std::endl;
  os << indent << "OutputIndex: " << this->m_OutputIndex << std::endl;
  os << indent << "OutputSpacing: " << this->m_OutputSpacing << std::endl;
  os << indent << "OutputOrigin: " << this->m_OutputOrigin << std::endl;
  os << indent << "OutputDirection: " << this->m_OutputDirection << std::endl;
  os << indent << "Transform: " << this->m_Transform.GetPointer() << std::endl;

} // end PrintSelf()


/**
 * Set the output image spacing.
 */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::SetOutputSpacing( const double* spacing )
{
  SpacingType s( spacing );
  this->SetOutputSpacing( s );

} // end SetOutputSpacing()


/**
 * Set the output image origin.
 */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::SetOutputOrigin( const double* origin )
{
  OriginType p( origin );
  this->SetOutputOrigin( p );

}

/** Helper method to set the output parameters based on this image */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::SetOutputParametersFromImage ( OutputImagePointer image )
{
  this->SetOutputOrigin( image->GetOrigin() );
  this->SetOutputSpacing( image->GetSpacing() );
  this->SetOutputDirection( image->GetDirection() );
  this->SetOutputSize( image->GetLargestPossibleRegion().GetSize() );
  this->SetOutputIndex( image->GetLargestPossibleRegion().GetIndex() );

} // end SetOutputParametersFromImage()


/** Helper method to set the output parameters based on this image */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::SetOutputParametersFromImage ( OutputImageConstPointer image )
{
  this->SetOutputOrigin( image->GetOrigin() );
  this->SetOutputSpacing( image->GetSpacing() );
  this->SetOutputDirection( image->GetDirection() );
  this->SetOutputSize( image->GetLargestPossibleRegion().GetSize() );
  this->SetOutputIndex( image->GetLargestPossibleRegion().GetIndex() );

} // end SetOutputParametersFromImage()


/**
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::BeforeThreadedGenerateData( void )
{
  if( !this->m_Transform )
    {
    itkExceptionMacro(<< "Transform not set");
    }

} // end BeforeThreadedGenerateData()


/**
 * ThreadedGenerateData
 */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::ThreadedGenerateData(
  const OutputImageRegionType & outputRegionForThread,
  int threadId )
{
  // Check whether we can use a fast path for resampling. Fast path
  // can be used if the transformation is linear. Transform respond
  // to the IsLinear() call.
  if ( this->m_Transform->IsLinear() )
    {
    this->LinearThreadedGenerateData( outputRegionForThread, threadId );
    return;
    }

  // Otherwise, we use the normal method where the transform is called
  // for computing the transformation of every point.
  this->NonlinearThreadedGenerateData( outputRegionForThread, threadId );
  
} // end ThreadedGenerateData()


template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::NonlinearThreadedGenerateData(
  const OutputImageRegionType & outputRegionForThread,
  int threadId )
{
  // Get the output pointer
  OutputImagePointer outputPtr = this->GetOutput();
 
  // Create an iterator that will walk the output region for this thread.
  typedef ImageRegionIteratorWithIndex<TOutputImage> OutputIteratorType;
  OutputIteratorType outIt( outputPtr, outputRegionForThread );

  // Define a few variables that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of output pixel
  PointType transformedPoint;    // Coordinates of transformed pixel
  PixelType deformation;         // the difference

  // Support for progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );
 
  // Walk the output region
  outIt.GoToBegin();
  while ( !outIt.IsAtEnd() )
    {
    // Determine the index of the current output pixel
    outputPtr->TransformIndexToPhysicalPoint( outIt.GetIndex(), outputPoint );

    // Compute corresponding input pixel position
    transformedPoint = this->m_Transform->TransformPoint( outputPoint );

    // Compute the deformation
    for ( unsigned int i = 0; i < ImageDimension; ++i )
      {
      deformation[ i ] = static_cast<PixelValueType>( transformedPoint[ i ] - outputPoint[ i ] );
      } 

    // Set it
    outIt.Set( deformation );

    // Update progress and iterator
    progress.CompletedPixel();
    ++outIt;
    }

} // end NonlinearThreadedGenerateData()


template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::LinearThreadedGenerateData(
  const OutputImageRegionType & outputRegionForThread,
  int threadId )
{
  // Get the output pointer
  OutputImagePointer      outputPtr = this->GetOutput();

  // Create an iterator that will walk the output region for this thread.
  typedef ImageLinearIteratorWithIndex<TOutputImage> OutputIteratorType;
  OutputIteratorType outIt( outputPtr, outputRegionForThread );
  outIt.SetDirection( 0 );

  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType transformedPoint;    // Coordinates of transformed pixel
  PixelType deformation;         // the difference
  
  IndexType index;

  // Support for progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );
    
  // Determine the position of the first pixel in the scanline
  outIt.GoToBegin();
  index = outIt.GetIndex();
  outputPtr->TransformIndexToPhysicalPoint( index, outputPoint );
  
  // Compute corresponding transformed pixel position
  transformedPoint = this->m_Transform->TransformPoint( outputPoint );
  
  // Compare with the ResampleImageFilter

  // Compute delta
  PointType outputPointNeighbour;
  PointType transformedPointNeighbour;
  typedef typename PointType::VectorType VectorType;
  VectorType delta;
  ++index[0];
  outputPtr->TransformIndexToPhysicalPoint( index, outputPointNeighbour );
  transformedPointNeighbour = this->m_Transform->TransformPoint( outputPointNeighbour );
  SpacingType sp; sp.Fill( 0.0 ); sp[ 0 ] = this->m_OutputSpacing[ 0 ];
  delta = transformedPointNeighbour - transformedPoint - sp;

  // loop over the vector image
  while ( !outIt.IsAtEnd() )
    {
    // Get current point
    index = outIt.GetIndex();
    outputPtr->TransformIndexToPhysicalPoint( index, outputPoint );

    // Compute transformed point
    transformedPoint = this->m_Transform->TransformPoint( outputPoint );

    while ( !outIt.IsAtEndOfLine() )
      {
      // Compute the deformation
      for ( unsigned int i = 0; i < ImageDimension; ++i )
        {
        deformation[ i ] = static_cast<PixelValueType>( transformedPoint[ i ] - outputPoint[ i ] );
        }

      // Set it
      outIt.Set( deformation );

      // Update stuff
      progress.CompletedPixel();
      ++outIt;
      transformedPoint += delta;
      }

    outIt.NextLine();
    }

} // end LinearThreadedGenerateData()


/** 
 * Inform pipeline of required output region
 */
template <class TOutputImage, class TTransformPrecisionType>
void 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::GenerateOutputInformation( void )
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointer to the output
  OutputImagePointer outputPtr = this->GetOutput();
  if ( !outputPtr )
    {
    return;
    }

  OutputImageRegionType region;
  region.SetSize( m_OutputSize );
  region.SetIndex( m_OutputIndex );
  outputPtr->SetLargestPossibleRegion( region );

  outputPtr->SetSpacing( m_OutputSpacing );
  outputPtr->SetOrigin( m_OutputOrigin );
  outputPtr->SetDirection( m_OutputDirection );

} // end GenerateOutputInformation()


/** 
 * Verify if any of the components has been modified.
 */
template <class TOutputImage, class TTransformPrecisionType>
unsigned long 
TransformToDeformationFieldSource<TOutputImage,TTransformPrecisionType>
::GetMTime( void ) const
{
  unsigned long latestTime = Object::GetMTime(); 

  if( this->m_Transform )
    {
    if( latestTime < this->m_Transform->GetMTime() )
      {
      latestTime = this->m_Transform->GetMTime();
      }
    }

  return latestTime;
} // end GetMTime()


} // end namespace itk

#endif // end #ifndef _itkTransformToDeformationFieldSource_txx
