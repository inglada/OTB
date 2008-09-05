/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkResampleImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-01-04 12:54:09 $
  Version:   $Revision: 1.61 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkResampleImageFilter_txx
#define _itkResampleImageFilter_txx

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
#include "itkOptResampleImageFilter.txx"
#else


#include "itkResampleImageFilter.h"
#include "itkObjectFactory.h"
#include "itkIdentityTransform.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkProgressReporter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkSpecialCoordinatesImage.h"

namespace itk
{

/**
 * Initialize new instance
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
ResampleImageFilter<TInputImage, TOutputImage,TInterpolatorPrecisionType>
::ResampleImageFilter()
{
  m_OutputSpacing.Fill(1.0);
  m_OutputOrigin.Fill(0.0);
  m_OutputDirection.SetIdentity();

  m_UseReferenceImage = false;

  m_Size.Fill( 0 );
  m_OutputStartIndex.Fill( 0 );
  
  m_Transform = IdentityTransform<TInterpolatorPrecisionType, ImageDimension>::New();
  m_Interpolator = LinearInterpolateImageFunction<InputImageType, TInterpolatorPrecisionType>::New();
  m_DefaultPixelValue = 0;
}


/**
 * Print out a description of self
 *
 * \todo Add details about this class
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage, TOutputImage,TInterpolatorPrecisionType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "DefaultPixelValue: "
     << static_cast<typename NumericTraits<PixelType>::PrintType>(m_DefaultPixelValue)
     << std::endl;
  os << indent << "Size: " << m_Size << std::endl;
  os << indent << "OutputStartIndex: " << m_OutputStartIndex << std::endl;
  os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;
  os << indent << "OutputOrigin: " << m_OutputOrigin << std::endl;
  os << indent << "OutputDirection: " << m_OutputDirection << std::endl;
  os << indent << "Transform: " << m_Transform.GetPointer() << std::endl;
  os << indent << "Interpolator: " << m_Interpolator.GetPointer() << std::endl;
  os << indent << "UseReferenceImage: " << (m_UseReferenceImage ? "On" : "Off") << std::endl;
  return;
}



/**
 * Set the output image spacing.
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::SetOutputSpacing(
  const double* spacing)
{
  SpacingType s(spacing);
  this->SetOutputSpacing( s );
}


/**
 * Set the output image origin.
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::SetOutputOrigin(
  const double* origin)
{
  OriginPointType p(origin);
  this->SetOutputOrigin( p );
}


/**
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::BeforeThreadedGenerateData()
{
  if( !m_Transform )
    {
    itkExceptionMacro(<< "Transform not set");
    }

  if( !m_Interpolator )
    {
    itkExceptionMacro(<< "Interpolator not set");
    }

  // Connect input image to interpolator
  m_Interpolator->SetInputImage( this->GetInput() );

}

/**
 * Set up state of filter after multi-threading.
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::AfterThreadedGenerateData()
{
  // Disconnect input image from the interpolator
  m_Interpolator->SetInputImage( NULL );

}

/**
 * ThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::ThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread,
  int threadId)
{
  // Check whether the input or the output is a
  // SpecialCoordinatesImage.  If either are, then we cannot use the
  // fast path since index mapping will definately not be linear.
  typedef SpecialCoordinatesImage<PixelType, ImageDimension> OutputSpecialCoordinatesImageType;
  typedef SpecialCoordinatesImage<InputPixelType, InputImageDimension> InputSpecialCoordinatesImageType;

  // Our friend the SGI needs these declarations to avoid unresolved
  // linker errors.
#ifdef __sgi
  InputSpecialCoordinatesImageType::Pointer foo =
    InputSpecialCoordinatesImageType::New();
  OutputSpecialCoordinatesImageType::Pointer bar =
    OutputSpecialCoordinatesImageType::New();
#endif
  if (dynamic_cast<const InputSpecialCoordinatesImageType *>(this->GetInput())
      || dynamic_cast<const OutputSpecialCoordinatesImageType *>(this->GetOutput()))
    {
    this->NonlinearThreadedGenerateData(outputRegionForThread, threadId);
    return;
    }
  
  // Check whether we can use a fast path for resampling. Fast path
  // can be used if the transformation is linear. Transform respond
  // to the IsLinear() call.
  if( m_Transform->IsLinear() )
    {
    this->LinearThreadedGenerateData(outputRegionForThread, threadId);
    return;
    }

  // Otherwise, we use the normal method where the transform is called
  // for computing the transformation of every point.
  this->NonlinearThreadedGenerateData(outputRegionForThread, threadId);
  
}


template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::NonlinearThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread,
  int threadId)
{
  // Get the output pointers
  OutputImagePointer      outputPtr = this->GetOutput();

  // Get ths input pointers
  InputImageConstPointer inputPtr=this->GetInput();

  // Create an iterator that will walk the output region for this thread.
  typedef ImageRegionIteratorWithIndex<TOutputImage> OutputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);

  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType inputPoint;          // Coordinates of current input pixel

  typedef ContinuousIndex<TInterpolatorPrecisionType, ImageDimension> ContinuousIndexType;
  ContinuousIndexType inputIndex;

  // Support for progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
        
  typedef typename InterpolatorType::OutputType OutputType;

  // Min/max values of the output pixel type AND these values
  // represented as the output type of the interpolator
  const PixelType minValue =  NumericTraits<PixelType >::NonpositiveMin();
  const PixelType maxValue =  NumericTraits<PixelType >::max();

  const OutputType minOutputValue = static_cast<OutputType>(minValue);
  const OutputType maxOutputValue = static_cast<OutputType>(maxValue);
  
  // Walk the output region
  outIt.GoToBegin();


  // This fix works for images up to approximately 2^25 pixels in
  // any dimension.  If the image is larger than this, this constant
  // needs to be made lower.
  double precisionConstant = 1<<(NumericTraits<double>::digits>>1);

  while ( !outIt.IsAtEnd() )
    {
    // Determine the index of the current output pixel
    outputPtr->TransformIndexToPhysicalPoint( outIt.GetIndex(), outputPoint );

    // Compute corresponding input pixel position
    inputPoint = m_Transform->TransformPoint(outputPoint);
    inputPtr->TransformPhysicalPointToContinuousIndex(inputPoint, inputIndex);

    // The inputIndex is precise to many decimal points, but this precision
    // involves some error in the last bits.  
    // Sometimes, when an index should be inside of the image, the
    // index will be slightly
    // greater than the largest index in the image, like 255.00000000002
    // for a image of size 256.  This can cause an empty row to show up
    // at the bottom of the image.
    // Therefore, the following routine uses a
    // precisionConstant that specifies the number of relevant bits,
    // and the value is truncated to this precision.
    for (int i=0; i < ImageDimension; ++i)
      {
      double roundedInputIndex = vcl_floor(inputIndex[i]);
      double inputIndexFrac = inputIndex[i] - roundedInputIndex;
      double newInputIndexFrac = vcl_floor(precisionConstant * inputIndexFrac)/precisionConstant;
      inputIndex[i] = roundedInputIndex + newInputIndexFrac;
      }    
    
    // Evaluate input at right position and copy to the output
    if( m_Interpolator->IsInsideBuffer(inputIndex) )
      {
      PixelType pixval;
      const OutputType value
        = m_Interpolator->EvaluateAtContinuousIndex(inputIndex);
      if( value < minOutputValue )
        {
        pixval = minValue;
        }
      else if( value > maxOutputValue )
        {
        pixval = maxValue;
        }
      else 
        {
        pixval = static_cast<PixelType>( value );
        }
      outIt.Set( pixval );      
      }
    else
      {
      outIt.Set(m_DefaultPixelValue); // default background value
      }

    progress.CompletedPixel();
    ++outIt;
    }

  return;
}

template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::LinearThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread,
  int threadId)
{
  // Get the output pointers
  OutputImagePointer      outputPtr = this->GetOutput();

  // Get ths input pointers
  InputImageConstPointer inputPtr=this->GetInput();

  // Create an iterator that will walk the output region for this thread.
  typedef ImageLinearIteratorWithIndex<TOutputImage> OutputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);
  outIt.SetDirection( 0 );

  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType inputPoint;          // Coordinates of current input pixel
  PointType tmpOutputPoint;
  PointType tmpInputPoint;

  typedef ContinuousIndex<TInterpolatorPrecisionType, ImageDimension> ContinuousIndexType;
  ContinuousIndexType inputIndex;
  ContinuousIndexType tmpInputIndex;
  
  typedef typename PointType::VectorType VectorType;
  VectorType delta;          // delta in input continuous index coordinate frame
  IndexType index;

  // Support for progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
        
  typedef typename InterpolatorType::OutputType OutputType;

  // Cache information from the superclass
  PixelType defaultValue = this->GetDefaultPixelValue();

  // Min/max values of the output pixel type AND these values
  // represented as the output type of the interpolator
  const PixelType minValue =  NumericTraits<PixelType >::NonpositiveMin();
  const PixelType maxValue =  NumericTraits<PixelType >::max();

  const OutputType minOutputValue = static_cast<OutputType>(minValue);
  const OutputType maxOutputValue = static_cast<OutputType>(maxValue);
  
  
  // Determine the position of the first pixel in the scanline
  index = outIt.GetIndex();
  outputPtr->TransformIndexToPhysicalPoint( index, outputPoint );
  

  // Compute corresponding input pixel position
  inputPoint = m_Transform->TransformPoint(outputPoint);
  inputPtr->TransformPhysicalPointToContinuousIndex(inputPoint, inputIndex);
  
  // As we walk across a scan line in the output image, we trace
  // an oriented/scaled/translated line in the input image.  Cache
  // the delta along this line in continuous index space of the input
  // image. This allows us to use vector addition to model the
  // transformation.
  //
  // To find delta, we take two pixels adjacent in a scanline 
  // and determine the continuous indices of these pixels when 
  // mapped to the input coordinate frame. We use the difference
  // between these two continuous indices as the delta to apply
  // to an index to trace line in the input image as we move 
  // across the scanline of the output image.
  //
  // We determine delta in this manner so that Images and
  // OrientedImages are both handled properly (with the delta for
  // OrientedImages taking into account the direction cosines).
  // 
  ++index[0];
  outputPtr->TransformIndexToPhysicalPoint( index, tmpOutputPoint );
  tmpInputPoint = m_Transform->TransformPoint( tmpOutputPoint );
  inputPtr->TransformPhysicalPointToContinuousIndex(tmpInputPoint,
                                                    tmpInputIndex);
  delta = tmpInputIndex - inputIndex;


  // This fix works for images up to approximately 2^25 pixels in
  // any dimension.  If the image is larger than this, this constant
  // needs to be made lower.
  double precisionConstant = 1<<(NumericTraits<double>::digits>>1);

  // Delta is precise to many decimal points, but this precision
  // involves some error in the last bits.  This error can accumulate
  // as the delta values are added.
  // Sometimes, when the accumulated delta should be inside of the
  // image, it will be slightly
  // greater than the largest index in the image, like 255.00000000002
  // for a image of size 256.  This can cause an empty column to show up
  // at the right side of the image. If we instead
  // truncate this delta value to some precision, this solves the problem.
  // Therefore, the following routine uses a
  // precisionConstant that specifies the number of relevant bits,
  // and the value is truncated to this precision.
  for (int i=0; i < ImageDimension; ++i)
    {
    double roundedDelta = vcl_floor(delta[i]);
    double deltaFrac = delta[i] - roundedDelta;
    double newDeltaFrac = vcl_floor(precisionConstant * deltaFrac)/precisionConstant;
    delta[i] = roundedDelta + newDeltaFrac;
    }


  while ( !outIt.IsAtEnd() )
    {
    // Determine the continuous index of the first pixel of output
    // scanline when mapped to the input coordinate frame.
    //

    // First get the position of the pixel in the output coordinate frame
    index = outIt.GetIndex();
    outputPtr->TransformIndexToPhysicalPoint( index, outputPoint );

    // Compute corresponding input pixel continuous index, this index
    // will incremented in the scanline loop
    inputPoint = m_Transform->TransformPoint(outputPoint);
    inputPtr->TransformPhysicalPointToContinuousIndex(inputPoint, inputIndex);
    
    // The inputIndex is precise to many decimal points, but this precision
    // involves some error in the last bits.
    // Sometimes, when an index should be inside of the image, the
    // index will be slightly
    // greater than the largest index in the image, like 255.00000000002
    // for a image of size 256.  This can cause an empty row to show up
    // at the bottom of the image.
    // Therefore, the following routine uses a
    // precisionConstant that specifies the number of relevant bits,
    // and the value is truncated to this precision.
    for (int i=0; i < ImageDimension; ++i)
      {
      double roundedInputIndex = vcl_floor(inputIndex[i]);
      double inputIndexFrac = inputIndex[i] - roundedInputIndex;
      double newInputIndexFrac = vcl_floor(precisionConstant * inputIndexFrac)/precisionConstant;
      inputIndex[i] = roundedInputIndex + newInputIndexFrac;
      }    


    while( !outIt.IsAtEndOfLine() )
      {
      // Evaluate input at right position and copy to the output
      if( m_Interpolator->IsInsideBuffer(inputIndex) )
        {
        PixelType pixval;
        const OutputType value
          = m_Interpolator->EvaluateAtContinuousIndex(inputIndex);
        if( value <  minOutputValue )
          {
          pixval = minValue;
          }
        else if( value > maxOutputValue )
          {
          pixval = maxValue;
          }
        else 
          {
          pixval = static_cast<PixelType>( value );
          }
        outIt.Set( pixval );      
        }
      else
        {
        outIt.Set(defaultValue); // default background value
        }
      
      progress.CompletedPixel();
      ++outIt;
      inputIndex += delta;
      }
    outIt.NextLine();
    }

  return;
}


/** 
 * Inform pipeline of necessary input image region
 *
 * Determining the actual input region is non-trivial, especially
 * when we cannot assume anything about the transform being used.
 * So we do the easy thing and request the entire input image.
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::GenerateInputRequestedRegion()
{
  // call the superclass's implementation of this method
  Superclass::GenerateInputRequestedRegion();

  if ( !this->GetInput() )
    {
    return;
    }

  // get pointers to the input and output
  InputImagePointer  inputPtr  = 
    const_cast< TInputImage *>( this->GetInput() );

  // Request the entire input image
  InputImageRegionType inputRegion;
  inputRegion = inputPtr->GetLargestPossibleRegion();
  inputPtr->SetRequestedRegion(inputRegion);

  return;
}


/** 
 * Set the smart pointer to the reference image that will provide
 * the grid parameters for the output image.
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
const typename ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>::OutputImageType *
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::GetReferenceImage() const
{
  Self * surrogate = const_cast< Self * >( this );
  const OutputImageType * referenceImage = 
    static_cast<const OutputImageType *>(surrogate->ProcessObject::GetInput(1));
  return referenceImage;
}


/** 
 * Set the smart pointer to the reference image that will provide
 * the grid parameters for the output image.
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::SetReferenceImage( const TOutputImage *image )
{
  itkDebugMacro("setting input ReferenceImage to " << image);
  if( image != static_cast<const TOutputImage *>(this->GetInput( 1 )) )
    {
    this->ProcessObject::SetNthInput(1, const_cast< TOutputImage *>( image ) );
    this->Modified();
    }
}

/** 
 * Inform pipeline of required output region
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  OutputImagePointer outputPtr = this->GetOutput();
  if ( !outputPtr )
    {
    return;
    }

  const OutputImageType * referenceImage = this->GetReferenceImage();

  // Set the size of the output region
  if( m_UseReferenceImage && referenceImage )
    {
    outputPtr->SetLargestPossibleRegion( referenceImage->GetLargestPossibleRegion() );
    }
  else
    {
    typename TOutputImage::RegionType outputLargestPossibleRegion;
    outputLargestPossibleRegion.SetSize( m_Size );
    outputLargestPossibleRegion.SetIndex( m_OutputStartIndex );
    outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
    }

  // Set spacing and origin
  if (m_UseReferenceImage && referenceImage)
    {
    outputPtr->SetSpacing( referenceImage->GetSpacing() );
    outputPtr->SetOrigin( referenceImage->GetOrigin() );
    outputPtr->SetDirection( referenceImage->GetDirection() );
    }
  else
    {
    outputPtr->SetSpacing( m_OutputSpacing );
    outputPtr->SetOrigin( m_OutputOrigin );
    outputPtr->SetDirection( m_OutputDirection );
    }
  return;
}



/** 
 * Verify if any of the components has been modified.
 */
template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
unsigned long 
ResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::GetMTime( void ) const
{
  unsigned long latestTime = Object::GetMTime(); 

  if( m_Transform )
    {
    if( latestTime < m_Transform->GetMTime() )
      {
      latestTime = m_Transform->GetMTime();
      }
    }

  if( m_Interpolator )
    {
    if( latestTime < m_Interpolator->GetMTime() )
      {
      latestTime = m_Interpolator->GetMTime();
      }
    }

  return latestTime;
}



} // end namespace itk

#endif

#endif

