/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRGBToVectorImageAdaptor.h,v $
  Language:  C++
  Date:      $Date: 2003-12-08 14:08:20 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRGBToVectorImageAdaptor_h
#define __itkRGBToVectorImageAdaptor_h

#include "itkImageAdaptor.h"
#include "itkRGBToVectorPixelAccessor.h"

namespace itk
{
 

/** \class RGBToVectorImageAdaptor
 * \brief Presents an image of pixel type RGBPixel as being and image of
 * Vectors.
 *
 * \ingroup ImageAdaptors
 *
 */
template <class TImage>
class ITK_EXPORT RGBToVectorImageAdaptor : public
      ImageAdaptor<TImage,
        Accessor::RGBToVectorPixelAccessor<
           typename TImage::PixelType::ComponentType
                                      > >
{
public:
  /** Standard class typedefs. */
  typedef RGBToVectorImageAdaptor  Self;
  typedef ImageAdaptor<TImage,
              Accessor::RGBToVectorPixelAccessor<
                 typename TImage::PixelType::ComponentType
                                       >  > Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro( RGBToVectorImageAdaptor, ImageAdaptor );

  /** PixelContainer typedef support. Used to construct a container for
   * the pixel data. */
  typedef typename Superclass::PixelContainer        PixelContainer;
  typedef typename Superclass::PixelContainerPointer PixelContainerPointer;
  typedef typename Superclass::PixelContainerConstPointer PixelContainerConstPointer;
  
 protected:
  RGBToVectorImageAdaptor() {}
  virtual ~RGBToVectorImageAdaptor() {}
  
 private:
  RGBToVectorImageAdaptor(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
