/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaImageConverter.txx,v $
  Language:  C++
  Date:      $Date: 2007/01/28 19:24:56 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaImageConverter_txx
#define __itkMetaImageConverter_txx

#include "itkMetaImageConverter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace itk  
{

/** Constructor */ 
template <unsigned int NDimensions, class PixelType>
MetaImageConverter<NDimensions,PixelType>
::MetaImageConverter()
{
  
}


/** Convert a metaImage into an Image SpatialObject  */
template <unsigned int NDimensions, class PixelType>
typename MetaImageConverter<NDimensions,PixelType>::SpatialObjectPointer
MetaImageConverter<NDimensions,PixelType>
::MetaImageToImageSpatialObject(MetaImage * image)
{ 
  SpatialObjectPointer spatialObject = SpatialObjectType::New();
  typedef itk::Image<PixelType,NDimensions>  ImageType;

  typedef typename ImageType::Pointer     ImagePointer;
  typedef typename ImageType::SizeType    SizeType;
  typedef typename ImageType::RegionType  RegionType;

  ImagePointer myImage = ImageType::New();

  SizeType size;

  double spacing[NDimensions];

  for(unsigned int i=0;i<NDimensions;i++)
    {
    size[i] = image->DimSize()[i];
    spacing[i] = image->ElementSpacing()[i];
    if(spacing[i] == 0) {spacing[i] = 1;}
    }

  RegionType region;
  region.SetSize(size);
  itk::Index<NDimensions> zeroIndex;
  zeroIndex.Fill(0);
  region.SetIndex( zeroIndex );
  myImage->SetLargestPossibleRegion(region);
  myImage->SetBufferedRegion(region);
  myImage->SetRequestedRegion(region);
  myImage->SetSpacing(spacing);
  myImage->Allocate();

  itk::ImageRegionIteratorWithIndex< ImageType > it(myImage, region);
  for(unsigned int i = 0; !it.IsAtEnd(); i++, ++it)
    { 
    it.Set( 
      static_cast< typename ImageType::PixelType >( image->ElementData(i) ));
    }

  spatialObject->SetImage(myImage);
  spatialObject->SetId(image->ID());
  spatialObject->SetParentId(image->ParentID());
  spatialObject->GetProperty()->SetName(image->Name());

  return spatialObject;
}

/** Convert an Image SpatialObject into a metaImage */
template <unsigned int NDimensions, class PixelType>
MetaImage*
MetaImageConverter<NDimensions,PixelType>
::ImageSpatialObjectToMetaImage(SpatialObjectType * spatialObject)
{ 
  typedef itk::Image<PixelType,NDimensions>  ImageType;
  typedef typename ImageType::ConstPointer   ImageConstPointer;
  typedef typename ImageType::SizeType       SizeType;
  typedef typename ImageType::RegionType     RegionType;
  
  ImageConstPointer SOImage = spatialObject->GetImage();

  float spacing[NDimensions];
  int size[NDimensions];

  for(unsigned int i=0;i<NDimensions;i++)
    {
    size[i] = SOImage->GetLargestPossibleRegion().GetSize()[i];
    spacing[i] = SOImage->GetSpacing()[i];
    }

  MetaImage* Image = new MetaImage(NDimensions,size,
                               spacing,MET_GetPixelType(typeid(PixelType)));

  itk::ImageRegionConstIterator< ImageType > it(SOImage, 
                                       SOImage->GetLargestPossibleRegion());
  for(unsigned int i = 0; !it.IsAtEnd(); i++, ++it)
    {
    Image->ElementData(i,it.Get());
    }

  Image->ID(spatialObject->GetId());
  if(spatialObject->GetParent())
    {
    Image->ParentID(spatialObject->GetParent()->GetId());
    }
  return Image;
}


/** Read a meta file give the type */
template <unsigned int NDimensions, class PixelType> 
typename MetaImageConverter<NDimensions,PixelType>::SpatialObjectPointer
MetaImageConverter<NDimensions,PixelType>
::ReadMeta(const char* name)
{
  SpatialObjectPointer spatialObject;
  MetaImage* Image = new MetaImage();
  Image->Read(name);
  Image->PrintInfo();

  spatialObject = MetaImageToImageSpatialObject(Image);

  return spatialObject;
}


/** Write a meta Image file */
template <unsigned int NDimensions, class PixelType>   
bool
MetaImageConverter<NDimensions,PixelType>
::WriteMeta(SpatialObjectType* spatialObject,const char* name)
{
  MetaImage* Image = ImageSpatialObjectToMetaImage(spatialObject);
  Image->Write(name);
  return true;
}

} // end namespace itk 


#endif
