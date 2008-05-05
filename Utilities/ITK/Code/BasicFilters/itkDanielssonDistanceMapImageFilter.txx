/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDanielssonDistanceMapImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-03-13 23:07:39 $
  Version:   $Revision: 1.37 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkDanielssonDistanceMapImageFilter_txx
#define _itkDanielssonDistanceMapImageFilter_txx

#include <iostream>

#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkReflectiveImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace itk
{


/**
 *    Constructor
 */
template <class TInputImage,class TOutputImage>
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::DanielssonDistanceMapImageFilter()
{

  this->SetNumberOfRequiredOutputs( 3 );

  OutputImagePointer distanceMap = OutputImageType::New();
  this->SetNthOutput( 0, distanceMap.GetPointer() );

  OutputImagePointer voronoiMap = OutputImageType::New();
  this->SetNthOutput( 1, voronoiMap.GetPointer() );

  VectorImagePointer distanceVectors = VectorImageType::New();
  this->SetNthOutput( 2, distanceVectors.GetPointer() );

  m_SquaredDistance     = false;
  m_InputIsBinary       = false;
  m_UseImageSpacing     = false;
}



/**
 *  Return the distance map Image pointer
 */
template <class TInputImage,class TOutputImage>
typename DanielssonDistanceMapImageFilter<
  TInputImage,TOutputImage>::OutputImageType * 
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::GetDistanceMap(void)
{
  return  dynamic_cast< OutputImageType * >(
    this->ProcessObject::GetOutput(0) );
}





/**
 *  Return Closest Points Map
 */
template <class TInputImage,class TOutputImage>
typename
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>::OutputImageType*
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::GetVoronoiMap(void)
{
  return  dynamic_cast< OutputImageType * >(
    this->ProcessObject::GetOutput(1) );
}






/**
 *  Return the distance vectors
 */
template <class TInputImage,class TOutputImage>
typename DanielssonDistanceMapImageFilter<
  TInputImage,TOutputImage>::VectorImageType * 
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::GetVectorDistanceMap(void)
{
  return  dynamic_cast< VectorImageType * >(
    this->ProcessObject::GetOutput(2) );
}






/**
 *  Prepare data for computation
 */
template <class TInputImage,class TOutputImage>
void 
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::PrepareData(void) 
{
  
  itkDebugMacro(<< "PrepareData Start");
  OutputImagePointer voronoiMap = this->GetVoronoiMap();

  InputImagePointer  inputImage  = 
    dynamic_cast<const TInputImage  *>( ProcessObject::GetInput(0) );

  voronoiMap->SetLargestPossibleRegion( 
    inputImage->GetLargestPossibleRegion() );

  voronoiMap->SetBufferedRegion( 
    inputImage->GetBufferedRegion() );

  voronoiMap->SetRequestedRegion( 
    inputImage->GetRequestedRegion() );

  voronoiMap->Allocate();

  OutputImagePointer distanceMap = this->GetDistanceMap();

  distanceMap->SetLargestPossibleRegion( 
    inputImage->GetLargestPossibleRegion() );

  distanceMap->SetBufferedRegion( 
    inputImage->GetBufferedRegion() );

  distanceMap->SetRequestedRegion( 
    inputImage->GetRequestedRegion() );

  distanceMap->Allocate();


  typename OutputImageType::RegionType region  = voronoiMap->GetRequestedRegion();


  // find the largest of the image dimensions
  typename TInputImage::SizeType size = region.GetSize();
  unsigned int maxLength = 0;
  for( unsigned int dim=0; dim < TInputImage::ImageDimension; dim++)
    {
    if( maxLength < size[ dim ] )
      {
      maxLength = size[ dim ];
      }
    }



  ImageRegionConstIteratorWithIndex< TInputImage >  it( inputImage,  region );
  ImageRegionIteratorWithIndex< TOutputImage > ot( voronoiMap,  region );

  it.GoToBegin();
  ot.GoToBegin();

  itkDebugMacro(<< "PrepareData: Copy input to output");
  if( m_InputIsBinary ) 
    {
    unsigned int npt = 1;
    while( !ot.IsAtEnd() )
      {
      if( it.Get() )
        {
        ot.Set( npt++ );
        }
      else 
        {
        ot.Set( 0 );
        }
      ++it;
      ++ot;
      }
    }
  else 
    {
    while( !ot.IsAtEnd() )
      {
      ot.Set( static_cast< typename OutputImageType::PixelType >( it.Get() ) );
      ++it;
      ++ot;
      }
    }

  VectorImagePointer distanceComponents = GetVectorDistanceMap();

  distanceComponents->SetLargestPossibleRegion( 
    inputImage->GetLargestPossibleRegion() );

  distanceComponents->SetBufferedRegion( 
    inputImage->GetBufferedRegion() );

  distanceComponents->SetRequestedRegion( 
    inputImage->GetRequestedRegion() );

  distanceComponents->Allocate();

  ImageRegionIteratorWithIndex< VectorImageType >  ct( distanceComponents,  region );

  typename VectorImageType::PixelType maxValue;
  typename VectorImageType::PixelType minValue;

  for( unsigned int j=0; j<InputImageDimension; j++ )
    {
    maxValue[j] =  2 * maxLength;
    minValue[j] =              0;
    }

  itkDebugMacro(<< "PrepareData: Copy output to ct");
  
  ot.GoToBegin();
  ct.GoToBegin();
  while( !ot.IsAtEnd() )
    {
    if( ot.Get() )
      {
      ct.Set( minValue );
      }
    else 
      {
      ct.Set( maxValue );
      }
    ++ot;
    ++ct;
    }
  itkDebugMacro(<< "PrepareData End");    
}




/**
 *  Post processing for computing the Voronoi Map
 */
template <class TInputImage,class TOutputImage>
void 
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::ComputeVoronoiMap() 
{

  itkDebugMacro( << "ComputeVoronoiMap Start");
  OutputImagePointer    voronoiMap          =  this->GetVoronoiMap();
  OutputImagePointer    distanceMap         =  this->GetDistanceMap();
  VectorImagePointer    distanceComponents  =  this->GetVectorDistanceMap();

  typename OutputImageType::RegionType region  = voronoiMap->GetRequestedRegion();

  ImageRegionIteratorWithIndex< OutputImageType >  ot( voronoiMap,          region );
  ImageRegionIteratorWithIndex< VectorImageType >  ct( distanceComponents,  region );
  ImageRegionIteratorWithIndex< OutputImageType >  dt( distanceMap,         region );

  typename InputImageType::SpacingType spacing = Self::GetInput()->GetSpacing();

  itkDebugMacro( << "ComputeVoronoiMap Region: " << region);
  ot.GoToBegin();
  ct.GoToBegin();
  dt.GoToBegin();
  while( ! ot.IsAtEnd() )
    {
    IndexType index = ct.GetIndex() + ct.Get();
    if( region.IsInside( index ) )
      { 
      ot.Set( voronoiMap->GetPixel( index ) );
      }

    OffsetType distanceVector = ct.Get();
    double distance = 0.0;
    if (m_UseImageSpacing)
      {
      for(unsigned int i=0; i<InputImageDimension; i++)
        {
        double spacingComponent = static_cast< double >(spacing[i]);
        distance += distanceVector[i] * distanceVector[i] * spacingComponent * spacingComponent;
        }
      }
    else
      {
      for(unsigned int i=0; i<InputImageDimension; i++)
        {
        distance += distanceVector[i] * distanceVector[i];
        }
      }

    if( m_SquaredDistance )
      {
      dt.Set( static_cast<typename OutputImageType::PixelType>( distance ) );
      }
    else
      {
      dt.Set( static_cast<typename OutputImageType::PixelType>(sqrt( distance )) );
      }
    ++ot;
    ++ct;
    ++dt;
    }
  itkDebugMacro( << "ComputeVoronoiMap End");
}

/**
 *  Locally update the distance.
 */
template <class TInputImage,class TOutputImage>
void
DanielssonDistanceMapImageFilter<TInputImage, TOutputImage>
::UpdateLocalDistance(VectorImageType* components,
                      const IndexType& here,
                      const OffsetType& offset)
{
  IndexType  there            = here + offset;
  OffsetType offsetValueHere  = components->GetPixel( here  );
  OffsetType offsetValueThere = components->GetPixel( there ) + offset;

  typename InputImageType::SpacingType spacing = Self::GetInput()->GetSpacing();

  double norm1 = 0.0;
  double norm2 = 0.0;
  for( unsigned int i=0; i<InputImageDimension; i++ )
    {
    double v1 = static_cast< double >(  offsetValueHere[ i]  );
    double v2 = static_cast< double >(  offsetValueThere[i] );
    
    if (m_UseImageSpacing)
      {
      double spacingComponent = static_cast< double >(spacing[i]);
      v1 *= spacingComponent;
      v2 *= spacingComponent;
      }

    norm1 +=  v1 * v1;
    norm2 +=  v2 * v2;
    }
  
  if( norm1 > norm2 ) 
    {
    components->GetPixel( here ) = offsetValueThere;
    }

}


/**
 *  Compute Distance and Voronoi maps
 */
template <class TInputImage,class TOutputImage>
void 
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::GenerateData() 
{

  this->PrepareData();

  // Specify images and regions.
  
  OutputImagePointer    voronoiMap             =  this->GetVoronoiMap();
  VectorImagePointer    distanceComponents     =  this->GetVectorDistanceMap();
  
  typename InputImageType::RegionType region  = voronoiMap->GetRequestedRegion();
  
  itkDebugMacro (<< "Region to process: " << region);

  // Instantiate reflective iterator

  ReflectiveImageRegionConstIterator< VectorImageType > 
    it( distanceComponents, region );
  typename VectorImageType::OffsetType voffset;
  for(unsigned int dim=0; dim <VectorImageType::ImageDimension; dim++)
    {
    if (region.GetSize()[dim] > 1)
      {
      voffset[dim] = 1;
      }
    else
      {
      voffset[dim] = 0;
      }
    }
  it.SetBeginOffset(voffset);
  it.SetEndOffset(voffset);

  it.GoToBegin();

  // Support progress methods/callbacks.

  // Each pixel is visited 2^InputImageDimension times, and the number
  // of visits per pixel needs to be computed for progress reporting.
  unsigned long visitsPerPixel = (1 << InputImageDimension);
  unsigned long updateVisits, i=0;
  updateVisits = region.GetNumberOfPixels() * visitsPerPixel / 10;
  if ( updateVisits < 1 ) 
    {
    updateVisits = 1;
    }
  const float updatePeriod = static_cast<float>(updateVisits) * 10.0;

  // Process image.

  OffsetType  offset;
  offset.Fill( 0 );

  itkDebugMacro(<< "GenerateData: Computing distance transform");
  while( !it.IsAtEnd() )
    {

    if ( !(i % updateVisits ) )
      {
      this->UpdateProgress( (float) i / updatePeriod );
      }

    IndexType here = it.GetIndex();
    for(unsigned int dim=0; dim <VectorImageType::ImageDimension; dim++)
      {
      if (region.GetSize()[dim] <= 1)
        {
        continue;
        }
      if( it.IsReflected(dim) ) 
        {
        offset[dim]++;
        UpdateLocalDistance( distanceComponents, here, offset );
        offset[dim]=0;
        }
      else
        {
        offset[dim]--;
        UpdateLocalDistance( distanceComponents, here, offset );
        offset[dim]=0;
        }
      }
    ++it;
    ++i;
    }
  
  itkDebugMacro(<< "GenerateData: ComputeVoronoiMap");
  
  this->ComputeVoronoiMap();

} // end GenerateData()




/**
 *  Print Self
 */
template <class TInputImage,class TOutputImage>
void 
DanielssonDistanceMapImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Danielson Distance: " << std::endl;
  os << indent << "Input Is Binary   : " << m_InputIsBinary << std::endl;
  os << indent << "Use Image Spacing : " << m_UseImageSpacing << std::endl;
  os << indent << "Squared Distance  : " << m_SquaredDistance << std::endl;

}



} // end namespace itk

#endif
