/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLineIterator.h,v $
  Language:  C++
  Date:      $Date: 2007/01/30 20:56:09 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLineIterator_h
#define __itkLineIterator_h

#include "itkIndex.h"
#include "itkImage.h"
#include "itkLineConstIterator.h"

namespace itk
{

/**
 * \class LineIterator
 * \brief Iterator that walks a Bresenham line through an ND image.
 *
 * LineIterator is an iterator that walks a Bresenham line
 * through an image.  The iterator is constructed similar to other
 * image iterators except for instead of specifying a region to
 * traverse, you specify two indices. The interval specified by
 * the two indices is closed.  So, a line iterator specified with
 * the same start and end index will visit exactly one pixel.
 *
 * \code
 * LineConstIterator<ImageType> it(image, I1, I2);
 * while (!it.IsAtEnd())
 * {
 *    // visits at least 1 pixel
 * }
 * \endcode
 *
 * This class was contributed by Benjamin King, Experimentelle
 * Radiologie, Medizinische Hochschule Hannover.
 *
 * \sa LineConstIterator
 */
template<class TImage>
class ITK_EXPORT LineIterator : public LineConstIterator<TImage>
{
public:
  /** Standard class typedefs. */
  typedef LineIterator Self;

  /** Dimension of the image the iterator walks.  This constant is needed so 
   * that functions that are templated over image iterator type (as opposed to
   * being templated over pixel type and dimension) can have compile time
   * access to the dimension of the image that the iterator walks. */
  itkStaticConstMacro(ImageIteratorDimension, unsigned int,
                      TImage::ImageDimension);

  /** Define the superclass */
  typedef LineConstIterator<TImage> Superclass;

  /** Inherit types from the superclass */
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::IndexValueType         IndexValueType;
  typedef typename Superclass::OffsetType             OffsetType;
  typedef typename Superclass::OffsetValueType        OffsetValueType;
  typedef typename Superclass::SizeType               SizeType;
  typedef typename Superclass::SizeValueType          SizeValueType;
  typedef typename Superclass::RegionType             RegionType;
  typedef typename Superclass::ImageType              ImageType;
  typedef typename Superclass::PixelContainer         PixelContainer;
  typedef typename Superclass::PixelContainerPointer  PixelContainerPointer;
  typedef typename Superclass::InternalPixelType      InternalPixelType;
  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::AccessorType           AccessorType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineIterator, LineConstIterator);
  
  /** Set the pixel value */
  void Set( const PixelType & value)
    {
    // Normally, this would just be the following:
    //   m_Image->SetPixel(m_CurrentImageIndex,value);
    // However, we don't want a warning about m_Image being a ConstPointer
    // in the Superclass.
    const_cast<ImageType *>(this->m_Image.GetPointer())->
      SetPixel(this->m_CurrentImageIndex,value);
    }

  /** Return a reference to the pixel 
   * This method will provide the fastest access to pixel
   * data, but it will NOT support ImageAdaptors. */
  const PixelType & Value(void) 
    {
    return (this->m_Image->GetPixel(this->m_CurrentImageIndex));
    }

  /** operator= is provided to make sure the handle to the image is properly
   * reference counted. */
  Self &operator=(const Self& it);

  /** Constructor establishes an iterator to walk along a path */
  LineIterator(ImageType *imagePtr, const IndexType &firstIndex,
               const IndexType &lastIndex);

  /** Default Destructor. */
  virtual ~LineIterator() {};
};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_LineIterator(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT LineIterator< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef LineIterator< ITK_TEMPLATE_1 x > \
                                     LineIterator##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkLineIterator+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkLineIterator.txx"
#endif

#endif 
