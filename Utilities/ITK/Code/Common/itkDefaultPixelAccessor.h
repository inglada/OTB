/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDefaultPixelAccessor.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:05 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDefaultPixelAccessor_h
#define __itkDefaultPixelAccessor_h

#include "itkMacro.h"

namespace itk
{

/**
 * \class DefaultPixelAccessor
 * \brief Give access to partial aspects a type
 *
 * DefaultPixelAccessor is templated over an internal type and an
 * external type representation. This class encapsulates a
 * customized convertion between the internal and external
 * type representations.
 *
 * PixelAccessor is the class responsible for pixel-to-pixel
 * transformation during image data access. The DefaultPixelAccessor
 * is an identity operation on the pixel value. It only exist in
 * order to standarize the way in which pixels are accessed.
 *
 * PixelAccessor are used by ImageAdaptors in order to present
 * an Image as being of a different type. The usual application
 * of ImageAccessors is Image casting, by avoiding the overhead
 * of an ImageFilter that performs the complete transformation
 *
 * \sa ImageAdaptor
 * \sa PixelAccessor
 * \sa Image
 *
 * \ingroup ImageAdaptors
 *
 */

template <class TType>
class ITK_EXPORT DefaultPixelAccessor  
{
public:

 /** External typedef. It defines the external aspect
   * that this class will exhibit. */
  typedef TType ExternalType ;

  /** Internal typedef. It defines the internal real
   * representation of data. */
  typedef TType InternalType ;

  /** Set the pixel. */
  inline void Set(TType & output, const TType & input) const
    {output = input;}

  /** Get the pixel. */
  inline TType & Get( TType & input ) const
    {return input;}

  /** Get a const reference to the pixel. */
  inline const TType & Get( const TType & input ) const
    {return input;}
  
};

  
} // end namespace itk
  

#endif

