/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorToRGBPixelAccessor.h,v $
  Language:  C++
  Date:      $Date: 2003-12-12 14:20:58 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorToRGBPixelAccessor_h
#define __itkVectorToRGBPixelAccessor_h


#include "itkRGBPixel.h"
#include "itkVector.h"


namespace itk
{
namespace Accessor
{
/**
 * \class VectorToRGBPixelAccessor
 * \brief Give access to a Vector pixel type as if it were a RGBPixel type.
 *
 * This class is intended to be used as parameter of 
 * an ImageAdaptor to make an Vector image appear as being
 * an image of RGB pixel type.
 *
 * \sa ImageAdaptor
 * \ingroup ImageAdaptors
 *
 */

template <class T>
class ITK_EXPORT VectorToRGBPixelAccessor
{
public:
  /** Standard class typedefs. */
  typedef   VectorToRGBPixelAccessor        Self;

 /** External typedef. It defines the external aspect
   * that this class will exhibit */
  typedef   RGBPixel<T>    ExternalType;

  /** Internal typedef. It defines the internal real
   * representation of data */
  typedef  Vector<T,3>     InternalType;

  /** Write access to the VectorToRGB component */
  inline void Set( InternalType & output, const ExternalType & input ) const
    { 
    output[0] = input[0];
    output[1] = input[1];
    output[2] = input[2];
    }

  /** Read access to the VectorToRGB component */
  inline ExternalType Get( const InternalType & input ) const
    {
    ExternalType rgb(input.GetDataPointer());
    return rgb;
    }

private:
};
  
}  // end namespace Accessor
}  // end namespace itk

#endif

