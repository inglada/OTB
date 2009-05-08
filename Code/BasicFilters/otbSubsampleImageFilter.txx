/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSubsampleImageFilter_txx
#define __otbSubsampleImageFilter_txx

#include "otbSubsampleImageFilter.h"

#include "otbMacro.h"
#include "otbSubsampledImageRegionConstIterator.h"
#include "otbSubsampledImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"


namespace otb {

template < class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::PrintSelf( std::ostream & os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "SubsampleFactor = [" << m_SubsampleFactor[0];
  for ( unsigned int i = 1; i < InputImageDimension; ++i )
  {
    os << ", " << m_SubsampleFactor[i];
  }
  os << "]\n";
}

template < class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
bool
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::IsSubsampleFactorOne () const
{
  for ( unsigned int i = 0; i < InputImageDimension; ++i )
  {
    if ( m_SubsampleFactor[i] != 1 )
      return false;
  }

  return true;
}

template < class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if ( !IsSubsampleFactorOne() )
  {
    OutputImageRegionType newRegion;
    this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput()->GetLargestPossibleRegion() );
    this->GetOutput()->SetRegions( newRegion );
  }
}

template <class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void 
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::CallCopyOutputRegionToInputRegion 
( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  if ( static_cast<int>( DirectionOfTransformation ) == INVERSE )
  {
    typename OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename OutputImageRegionType::SizeType srcSize = srcRegion.GetSize();

    typename InputImageRegionType::IndexType destIndex;
    typename InputImageRegionType::SizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; ++i )
    {
      destIndex[i] = srcIndex[i] / m_SubsampleFactor[i];
      destSize[i] = ( srcSize[i] - 1 ) / m_SubsampleFactor[i] + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template <class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void 
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::CallCopyInputRegionToOutputRegion
( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )         
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  if ( static_cast<int>( DirectionOfTransformation ) == INVERSE )
  {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; ++i )
    {
      destIndex[i] = srcIndex[i] * m_SubsampleFactor[i];
      destSize[i] = ( srcSize[i] - 1 ) * m_SubsampleFactor[i] + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template <class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  SubsampledImageRegionIterator< OutputImageType > outputIter
    ( this->GetOutput(), outputRegionForThread );
  outputIter.SetSubsampleFactor(1);
  outputIter.GoToBegin();

  InputImageRegionType inputRegionForThread;
        this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  SubsampledImageRegionConstIterator< InputImageType > inputIter 
    ( this->GetInput(), inputRegionForThread );

  switch ( DirectionOfTransformation )
  {
    case FORWARD:
    {
      inputIter.SetSubsampleFactor( GetSubsampleFactor() );
      inputIter.GoToBegin();

      while ( !inputIter.IsAtEnd() && !outputIter.IsAtEnd() )
      {
        outputIter.SetLocation( static_cast< typename SubsampledImageRegionIterator< OutputImageType >::OffsetType >
                            ( inputIter.GetLocationOffset() ) );
        outputIter.Set( static_cast< OutputPixelType >( inputIter.Get() ) );
        ++inputIter;
      }

      break;
    }
    case INVERSE:
    {
      inputIter.SetSubsampleFactor( 1 );
      inputIter.GoToBegin();

      while ( !inputIter.IsAtEnd() && !outputIter.IsAtEnd() )
      {
        InputImageIndexType inputIndex = inputIter.GetLocationIndex();
        OutputImageIndexType outputIndex;
        for ( unsigned int i = 0; i < OutputImageDimension; ++i )
        {
          outputIndex[i] = inputIndex[i] * m_SubsampleFactor[i];
        }
        outputIter.SetLocation( outputIndex );
        outputIter.Set( static_cast< OutputPixelType >( inputIter.Get() ) );
        ++inputIter;
      }

      break;
    }
    default:
    {
      itkExceptionMacro(<<"otb::SubsampleImageFilter have to be FORWARD or INVERSE only!!");
      break;
    }
  }
}


} // end of namespace otb

#endif



