/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageMaskSpatialObject.h,v $
  Language:  C++
  Date:      $Date: 2005/06/16 13:54:53 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkImageMaskSpatialObject_h
#define __itkImageMaskSpatialObject_h

#include "itkImageSpatialObject.h"
#include "itkImageSliceConstIteratorWithIndex.h"

namespace itk
{
  
/** \class ImageMaskSpatialObject
 * \brief Implementation of an image mask as spatial object.
 *
 * This class derives from the ImageSpatialObject and overloads the IsInside()
 * method.  One of the common uses of this class is to serve as Mask for the
 * Image Registration Metrics.
 *
 * \sa ImageSpatialObject SpatialObject CompositeSpatialObject
 */

template < unsigned int TDimension = 3 >
class ImageMaskSpatialObject 
  : public ImageSpatialObject< TDimension, unsigned char >
{

public:
 
  typedef ImageMaskSpatialObject< TDimension >        Self;
  typedef ImageSpatialObject< TDimension >            Superclass;
  typedef SmartPointer< Self >                        Pointer;
  typedef SmartPointer< const Self >                  ConstPointer;

  typedef typename Superclass::ScalarType             ScalarType; 
  typedef typename Superclass::PixelType              PixelType; 
  typedef typename Superclass::ImageType              ImageType;
  typedef typename Superclass::ImagePointer           ImagePointer;
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::RegionType             RegionType;
  typedef typename Superclass::TransformType          TransformType;
  typedef typename Superclass::PointType              PointType;
  typedef typename Superclass::BoundingBoxType        BoundingBoxType;

  typedef itk::ImageSliceConstIteratorWithIndex< ImageType >  
                                                      SliceIteratorType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ImageMaskSpatialObject, ImageSpatialObject );

  /** Returns true if the point is inside, false otherwise. */
  bool IsInside( const PointType & point,
                 unsigned int depth, char *name) const;

   /** Test whether a point is inside or outside the object 
   *  For computational speed purposes, it is faster if the method does not
   *  check the name of the class and the current depth */ 
  virtual bool IsInside( const PointType & point) const;

  /** Compute axis aligned bounding box from the image mask. The bounding box
   * is returned as an image region. Each call to this function will recompute
   * the region. This function is useful in cases, where you may have a mask image
   * resulting from say a segmentation and you want to get the smallest box
   * region that encapsulates the mask image. Currently this is done only for 3D
   * volumes. */
  RegionType GetAxisAlignedBoundingBoxRegion() const;

protected:

  ImageMaskSpatialObject();
  virtual ~ImageMaskSpatialObject();

  void PrintSelf( std::ostream& os, Indent indent ) const;

};

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageMaskSpatialObject.txx"
#endif

#endif //__itkImageMaskSpatialObject_h

