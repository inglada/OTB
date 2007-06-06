/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSinImageAdaptor.h,v $
  Language:  C++
  Date:      $Date: 2006/04/01 13:44:21 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSinImageAdaptor_h
#define __itkSinImageAdaptor_h

#include <itkImageAdaptor.h>
#include "vnl/vnl_math.h"

namespace itk
{
 
namespace Accessor {
/** \class SinPixelAccessor
 * \brief Give access to the vcl_sin() function of a value
 *
 * SinPixelAccessor is templated over an internal type and an
 * external type representation. This class cast the input
 * applies the funtion to it and cast the result according 
 * to the types defined as template parameters
 *
 * \ingroup ImageAdaptors
 */
template <class TInternalType, class TExternalType >
class ITK_EXPORT SinPixelAccessor  
{
public:
  /** External typedef. It defines the external aspect
   * that this class will exhibit. */
  typedef TExternalType ExternalType;

  /** Internal typedef. It defines the internal real
   * representation of data. */
  typedef TInternalType InternalType;

  static inline void Set(TInternalType & output, const TExternalType & input) 
    {output = (TInternalType)vcl_sin((double)input);}

  static inline TExternalType Get( const TInternalType & input ) 
    {return (TExternalType)vcl_sin((double)input);}
};
  
} // end namespace Accessor
 
/** \class SinImageAdaptor
 * \brief Presents an image as being composed of the vcl_sin() of its pixels
 *
 * Additional casting is performed according to the input and output image
 * types following C++ default casting rules.
 *
 * \ingroup ImageAdaptors
 */
template <class TImage, class TOutputPixelType>
class ITK_EXPORT SinImageAdaptor : public
      ImageAdaptor<TImage,
                   Accessor::SinPixelAccessor<
                                      typename TImage::PixelType,
                                      TOutputPixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef SinImageAdaptor           Self;
  typedef ImageAdaptor<TImage, Accessor::SinPixelAccessor<
                               typename TImage::PixelType,
                               TOutputPixelType> >  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro( SinImageAdaptor, ImageAdaptor );

protected:
  SinImageAdaptor() {}
  virtual ~SinImageAdaptor() {}
  
private:
  SinImageAdaptor(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
