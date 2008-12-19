/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRealAndImaginaryToComplexImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-05 10:47:06 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRealAndImaginaryToComplexImageFilter_h
#define __itkRealAndImaginaryToComplexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace itk
{
  
/** \class RealAndImaginaryToComplexImageFilter
 * \brief Implements pixel-wise conversion of real and imaginar data into
 * complex voxels.
 *
 * This filter is parametrized over the types of the two
 * input images and the type of the output image.
 *
 * The filter expect all images to have the same dimension
 * (e.g. all 2D, or all 3D, or all ND)
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \weakgroup FourierTransform
 *
 * \author Simon K. Warfield simon.warfield@childrens.harvard.edu
 *
 * \note Attribution Notice. This research work was made possible by Grant 
 * Number R01 RR021885 (PI Simon K. Warfield, Ph.D.) from
 * the National Center for Research Resources (NCRR), a component of the
 * National Institutes of Health (NIH).  Its contents are solely the
 * responsibility of the authors and do not necessarily represent the
 * official view of NCRR or NIH.
 *
 * This class was taken from the Insight Journal paper:
 * http://insight-journal.org/midas/handle.php?handle=1926/326
 *
 * \sa MagnitudeAndPhaseToComplexImageFilter
 */

namespace Functor {  
  
template< class TInput1, class TInput2, class TOutput>
class RealAndImaginaryToComplex
{
public:
  RealAndImaginaryToComplex() {};
  ~RealAndImaginaryToComplex() {};
  bool operator!=( const RealAndImaginaryToComplex & ) const
    {
    return false;
    }
  bool operator==( const RealAndImaginaryToComplex & other ) const
    {
    return !(*this != other);
    }
  inline std::complex<TOutput> operator()( const TInput1 & A, const TInput2 & B)
    {
    return std::complex<TOutput>( static_cast<TOutput>(A), static_cast<TOutput>(B));
    }
}; 
}

template <class TInputPixel1, class TInputPixel2, class TOutputPixel, unsigned int NDimension = 3>
class ITK_EXPORT RealAndImaginaryToComplexImageFilter :
    public BinaryFunctorImageFilter<
       Image < TInputPixel1, NDimension >,
       Image < TInputPixel2, NDimension >,
       Image < std::complex<TOutputPixel>, NDimension >, 
         Functor::RealAndImaginaryToComplex< 
            TInputPixel1, 
            TInputPixel2,
            TOutputPixel
                 > >
{
public:
  /** Standard class typedefs. */
  typedef RealAndImaginaryToComplexImageFilter      Self;
  typedef BinaryFunctorImageFilter<
    Image < TInputPixel1, NDimension >,
    Image < TInputPixel2, NDimension >,
    Image < std::complex<TOutputPixel>, NDimension >, 
      Functor::RealAndImaginaryToComplex< 
            TInputPixel1, 
            TInputPixel2,
            TOutputPixel > >                        Superclass;

  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RealAndImaginaryToComplexImageFilter, BinaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(Input1ConvertibleToDoubleCheck,
    (Concept::Convertible<TInputPixel1, double>));
  itkConceptMacro(Input2ConvertibleToDoubleCheck,
    (Concept::Convertible<TInputPixel2, double>));
  itkConceptMacro(DoubleConvertibleToOutputCheck,
    (Concept::Convertible<double, TOutputPixel>));
  /** End concept checking */
#endif

protected:
  RealAndImaginaryToComplexImageFilter() {}
  virtual ~RealAndImaginaryToComplexImageFilter() {}

private:
  RealAndImaginaryToComplexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
