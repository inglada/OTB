/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkExpNegativeImageAdaptor.h,v $
  Language:  C++
  Date:      $Date: 2009-09-19 19:56:20 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkExpNegativeImageAdaptor_h
#define __itkExpNegativeImageAdaptor_h

#include <itkImageAdaptor.h>
#include "vnl/vnl_math.h"

namespace itk
{
 
namespace Accessor {
/** \class ExpNegativePixelAccessor
 * \brief Give access to the vcl_exp() function of a value
 *
 * ExpNegativePixelAccessor is templated over an internal type and an
 * external type representation. This class cast the input
 * applies the function to it and cast the result according 
 * to the types defined as template parameters
 * 
 * \ingroup ImageAdaptors
 */
template <class TInternalType, class TExternalType >
class ITK_EXPORT ExpNegativePixelAccessor  
{
public:
  /** External typedef. It defines the external aspect
    * that this class will exhibit. */
  typedef TExternalType ExternalType;

  /** Internal typedef. It defines the internal real
   * representation of data. */
  typedef TInternalType InternalType;

  static inline void Set(TInternalType & output, const TExternalType & input) 
    {output = static_cast<TInternalType>( vcl_exp(-static_cast<double>( input ) ) );}

  static inline TExternalType Get( const TInternalType & input ) 
    {return static_cast<TExternalType>( vcl_exp(-static_cast<double>( input) ) );}

};
  
} // end namespace Accessor

/** \class ExpNegativeImageAdaptor
 * \brief Presents an image as being composed of the vcl_exp() of its pixels
 *
 * Additional casting is performed according to the input and output image
 * types following C++ default casting rules.
 * 
 * \ingroup ImageAdaptors
 */
template <class TImage, class TOutputPixelType>
class ITK_EXPORT ExpNegativeImageAdaptor : public
      ImageAdaptor<TImage,Accessor::ExpNegativePixelAccessor<
                                      typename TImage::PixelType,
                                      TOutputPixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ExpNegativeImageAdaptor                         Self;
  typedef ImageAdaptor<
    TImage,Accessor::ExpNegativePixelAccessor<
      typename TImage::PixelType, TOutputPixelType> >     Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro( ExpNegativeImageAdaptor, ImageAdaptor );

protected:
  ExpNegativeImageAdaptor() {}
  virtual ~ExpNegativeImageAdaptor() {}
  
private:
  ExpNegativeImageAdaptor(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#endif
