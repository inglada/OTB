/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOrImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-14 19:56:22 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOrImageFilter_h
#define __itkOrImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkNumericTraits.h"


namespace itk
{
  
/** \class OrImageFilter
 * \brief Implements the OR logical operator pixel-wise between two images.
 *
 * This class is parametrized over the types of the two 
 * input images and the type of the output image. 
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * Since the logical OR operation is only defined in C++ for integer
 * types, the images passed to this filter must comply with the requirement
 * of using integer pixel type. 
 * 
 * The total operation over one pixel will be
 *
 *  output_pixel = static_cast<OutputPixelType>( input1_pixel | input2_pixel )
 *
 * Where "|" is the boolean OR operator in C++.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Functor {  
  
template< class TInput1, class TInput2=TInput1, class TOutput=TInput1 >
class OR
{
public:
  OR() {};
  ~OR() {};
  bool operator!=( const OR & ) const
    {
    return false;
    }
  bool operator==( const OR & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput1 & A, const TInput2 & B)
    {
    return static_cast<TOutput>( A | B );
    }
}; 

}
template <class TInputImage1, class TInputImage2=TInputImage1, class TOutputImage=TInputImage1>
class ITK_EXPORT OrImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Functor::OR< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >


{
public:
  /** Standard class typedefs. */
  typedef OrImageFilter             Self;
  typedef BinaryFunctorImageFilter<
    TInputImage1,TInputImage2,TOutputImage, 
    Functor::OR<typename TInputImage1::PixelType, 
                typename TInputImage2::PixelType,
                typename TOutputImage::PixelType> >
                                    Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(OrImageFilter, 
               BinaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(Input1Input2OutputLogicalOperatorsCheck,
    (Concept::LogicalOperators<typename TInputImage1::PixelType,
                               typename TInputImage2::PixelType,
                               typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  OrImageFilter() {}
  virtual ~OrImageFilter() {}

private:
  OrImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
