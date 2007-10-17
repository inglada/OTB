/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSignedMaurerDistanceMapImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2007/08/20 07:46:05 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSignedMaurerDistanceMapImageFilter_txx
#define __itkSignedMaurerDistanceMapImageFilter_txx

#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkProgressReporter.h"
#include "itkProgressAccumulator.h"
#include "vnl/vnl_vector.h"
#include "vnl/vnl_math.h"

//Simple functor to invert an image for Outside Danielsson distance map
namespace itk
{
namespace Functor
{
template <class InputPixelType> 
class InvertBinaryIntensityFunctor
{
public:
  InputPixelType operator()( InputPixelType input )
    {
    if( input )
      {
      return NumericTraits<InputPixelType>::Zero;
      }
    else
      {
      return NumericTraits<InputPixelType>::One;
      }
    }
};
}
}


namespace itk
{


template<class TInputImage, class TOutputImage>
SignedMaurerDistanceMapImageFilter<TInputImage, TOutputImage>
::SignedMaurerDistanceMapImageFilter() : m_BackgroundValue( 0 ),
                                         m_InsideIsPositive( false ),
                                         m_UseImageSpacing( false ),
                                         m_SquaredDistance( true )
{
}


template<class TInputImage, class TOutputImage>
SignedMaurerDistanceMapImageFilter<TInputImage, TOutputImage>
::~SignedMaurerDistanceMapImageFilter()
{
}


template<class TInputImage, class TOutputImage>
void
SignedMaurerDistanceMapImageFilter<TInputImage, TOutputImage>
::GenerateData()
{

  this->GetOutput()->SetRegions(
                         this->GetInput()->GetRequestedRegion() );

  this->GetOutput()->Allocate();
  this->m_Spacing = this->GetOutput()->GetSpacing();

  // store the binary image in an image with a pixel type as small as possible
  // instead of keeping the native input pixel type to avoid using too much
  // memory.
  typedef Image< unsigned char, InputImageDimension > BinaryImageType;
  typedef BinaryThresholdImageFilter<InputImageType, 
                                     BinaryImageType
                                        > BinaryFilterType;
  
  ProgressAccumulator::Pointer progressAcc = ProgressAccumulator::New();
  progressAcc->SetMiniPipelineFilter(this);
  
  // compute the boundary of the binary object.
  // To do that, we erode the binary object. The eroded pixels are the ones
  // on the boundary. We mark them with the value 2
  typename BinaryFilterType::Pointer binaryFilter = BinaryFilterType::New();

  binaryFilter->SetLowerThreshold( this->m_BackgroundValue );
  binaryFilter->SetUpperThreshold( this->m_BackgroundValue );
  binaryFilter->SetInsideValue( 0 );
  binaryFilter->SetOutsideValue( 1 );
  binaryFilter->SetInput( this->GetInput() );
//   progressAcc->RegisterInternalFilter( binaryFilter, 0.1f );
  binaryFilter->Update();

  // Dilate the inverted image by 1 pixel to give it the same boundary
  // as the univerted this->GetInput().

  typedef BinaryBallStructuringElement<
                     unsigned char,
                     InputImageDimension  > StructuringElementType;

  typedef BinaryErodeImageFilter<
                         BinaryImageType,
                         BinaryImageType,
                         StructuringElementType >     ErodeType;

  typename ErodeType::Pointer erode = ErodeType::New();

  StructuringElementType structuringElement;
  structuringElement.SetRadius( 1 );
  structuringElement.CreateStructuringElement();
  erode->SetKernel( structuringElement );
  erode->SetForegroundValue( 1 );
  erode->SetBackgroundValue( 2 );
  erode->SetInput( binaryFilter->GetOutput() );
  progressAcc->RegisterInternalFilter( erode, 0.33f );
  erode->Update();

  typedef ImageRegionConstIterator<BinaryImageType> InputIterator;

  InputIterator inIterator( erode->GetOutput(),
                            erode->GetOutput()->GetRequestedRegion() );

  typedef ImageRegionIterator<OutputImageType>  OutputIterator;

  OutputIterator outIterator( this->GetOutput(),
                              this->GetOutput()->GetRequestedRegion() );

  for (  inIterator.GoToBegin(), outIterator.GoToBegin();
        !inIterator.IsAtEnd();
        ++inIterator, ++outIterator )
    {
    if( inIterator.Get() == 2 )
      {
      outIterator.Set( NumericTraits< OutputPixelType >::Zero );
      }
    else
      {
      outIterator.Set( NumericTraits< OutputPixelType >::max() );
      }
    }

  vnl_vector<unsigned int> k(InputImageDimension-1);

  typedef typename InputImageType::RegionType   InputRegionType;
  typedef typename InputImageType::SizeType     InputSizeType;

  InputRegionType region = this->GetInput()->GetRequestedRegion();
  InputSizeType   size   = region.GetSize();
  typename InputImageType::RegionType::IndexType startIndex;
  startIndex = this->GetInput()->GetRequestedRegion().GetIndex();

  // compute the number of rows first, so we can setup a progress reporter
  typename std::vector< unsigned int > NumberOfRows;
  unsigned long totalNumberOfRows = 0;

  for (unsigned int i = 0; i < InputImageDimension; i++)
    {
    NumberOfRows.push_back( 1 );
    for (unsigned int d = 0; d < InputImageDimension; d++)
      {
      if( d != i )
        {
        NumberOfRows[i] *= size[ d ];
        }
      }
    totalNumberOfRows += NumberOfRows[i];
    }

  // set the progress reporter
  ProgressReporter progress(this, 0, totalNumberOfRows, 67, 0.33f, 0.67f);

  for (unsigned int i = 0; i < InputImageDimension; i++)
    {

    OutputIndexType idx;
    idx.Fill( 0 );

    k[0] = 1;
    unsigned int count = 1;


    for (unsigned int d = i+2; d < i+InputImageDimension; d++)
      {
      k[ count ] = k[ count-1 ] * size[ d % InputImageDimension ];
      count++;
      }
    k.flip();

    unsigned int index;
    for (unsigned int n = 0; n < NumberOfRows[i];n++)
      {
      index = n;
      count = 0;
      for (unsigned int d = i+1; d < i+InputImageDimension; d++)
        {
        idx[ d % InputImageDimension ] =
             static_cast<unsigned int>(
                 static_cast<double>( index ) /
                 static_cast<double>( k[count] ) )
             + startIndex[ d % InputImageDimension ];

        index %= k[ count ];
        count++;
        }
      this->Voronoi(i, idx);
      progress.CompletedPixel();
      }
    }

  if ( !this->m_SquaredDistance )
    {
    typedef ImageRegionIterator< OutputImageType > OutputIterator;
    typedef ImageRegionConstIterator< InputImageType  > InputIterator;

    typename OutputImageType::RegionType region =
                       this->GetOutput()->GetRequestedRegion();

    OutputIterator Ot( this->GetOutput(), region );
    InputIterator  It( this->GetInput(),  region );

    Ot.GoToBegin();
    It.GoToBegin();

    while( !Ot.IsAtEnd() )
      {
      
      // cast to a real type is required on some platforms 
      // TODO: use "typename NumericTraits<OutputPixelType>::RealType" instead
      // double. cableswig currently fail to build it with msvc 7.1
      const OutputPixelType outputValue =
        static_cast<OutputPixelType>(
          sqrt( static_cast<double>(vnl_math_abs( Ot.Get() ) ) ) );

      if( It.Get() != this->m_BackgroundValue )
        {
        if( this->GetInsideIsPositive() )
          {
          Ot.Set(  outputValue );
          }
        else
          {
          Ot.Set( -outputValue );
          }
        }
      else
        {
        if( this->GetInsideIsPositive() )
          {
          Ot.Set( -outputValue );
          }
        else
          {
          Ot.Set(  outputValue );
          }
        }

      ++Ot;
      ++It;
      }
    }

}


template<class TInputImage, class TOutputImage>
void
SignedMaurerDistanceMapImageFilter<TInputImage, TOutputImage>
::Voronoi(unsigned int d, OutputIndexType idx)
{
  typename OutputImageType::Pointer output(this->GetOutput());
  unsigned int nd = output->GetRequestedRegion().GetSize()[d];

  vnl_vector<OutputPixelType> g(nd);  g = 0;
  vnl_vector<OutputPixelType> h(nd);  h = 0;

  typename InputImageType::RegionType::IndexType startIndex;
  startIndex = this->GetInput()->GetRequestedRegion().GetIndex();

  OutputPixelType di;

  int l = -1;

  for( unsigned int i = 0; i < nd; i++ )
    {
    idx[d] = i + startIndex[d];

    di = output->GetPixel(idx);

    OutputPixelType iw;

    if( this->GetUseImageSpacing() )
      {
      iw = static_cast<OutputPixelType>( i * this->m_Spacing[d] );
      }
    else
      {
      iw  = static_cast<OutputPixelType>(i);
      }

    if( di != NumericTraits< OutputPixelType >::max() )
      {
      if( l < 1 )
        {
        l++;
        g(l) = di;
        h(l) = iw;
        }
      else
        {
        while( (l >= 1) &&
               this->Remove( g(l-1), g(l), di, h(l-1), h(l), iw) )
          {
          l--;
          }
        l++;
        g(l) = di;
        h(l) = iw;
        }
      }
    }

  if( l == -1 )
    {
    return;
    }

  int ns = l;

  l = 0;

  for( unsigned int i = 0; i < nd; i++ )
    {

    OutputPixelType iw;

    if( this->GetUseImageSpacing() )
      {
      iw = static_cast<OutputPixelType>( i * this->m_Spacing[d] );
      }
    else
      {
      iw = static_cast<OutputPixelType>( i );
      }

    OutputPixelType d1 = vnl_math_abs(g(l  )) + (h(l  )-iw)*(h(l  )-iw);
    OutputPixelType d2 = vnl_math_abs(g(l+1)) + (h(l+1)-iw)*(h(l+1)-iw);

    while( (l < ns) && (d1 > d2) )
      {
      l++;
      d1 = d2;
      d2 = vnl_math_abs(g(l+1)) + (h(l+1)-iw)*(h(l+1)-iw);
      }
    idx[d] = i + startIndex[d];

    if ( this->GetInput()->GetPixel( idx ) != this->m_BackgroundValue )
      {
      if ( this->m_InsideIsPositive )
        {
        output->SetPixel( idx,  d1 );
        }
      else
        {
        output->SetPixel( idx, -d1 );
        }
      }
    else
      {
      if ( this->m_InsideIsPositive )
        {
        output->SetPixel( idx, -d1 );
        }
      else
        {
        output->SetPixel( idx,  d1 );
        }
      }
    }
}


template<class TInputImage, class TOutputImage>
bool
SignedMaurerDistanceMapImageFilter<TInputImage, TOutputImage>
::Remove( OutputPixelType d1, OutputPixelType d2, OutputPixelType df,
          OutputPixelType x1, OutputPixelType x2, OutputPixelType xf )
{
  OutputPixelType a = x2 - x1;
  OutputPixelType b = xf - x2;
  OutputPixelType c = xf - x1;

  return ( (   c * vnl_math_abs( d2 ) - b * vnl_math_abs( d1 )
             - a * vnl_math_abs( df ) - a * b * c ) > 0);
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage>
void
SignedMaurerDistanceMapImageFilter<TInputImage, TOutputImage>
::PrintSelf( std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Background Value: "
     << this->m_BackgroundValue << std::endl;
  os << indent << "Spacing: "
     << this->m_Spacing << std::endl;
  os << indent << "Inside is positive: "
     << this->m_InsideIsPositive << std::endl;
  os << indent << "Use image spacing: "
     << this->m_UseImageSpacing << std::endl;
  os << indent << "Squared distance: "
     << this->m_SquaredDistance << std::endl;
}

} // end namespace itk

#endif
