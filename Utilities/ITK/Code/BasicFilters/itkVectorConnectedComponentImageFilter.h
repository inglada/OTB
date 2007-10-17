/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorConnectedComponentImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/09/26 12:05:05 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorConnectedComponentImageFilter_h
#define __itkVectorConnectedComponentImageFilter_h

#include "vnl/vnl_math.h"
#include "itkNumericTraits.h"
#include "itkConnectedComponentFunctorImageFilter.h"

namespace itk
{
  
/** \class SimilarVectorsFunctor
 *
 *  \brief A connected components filter that labels the
 *         objects in a vector image.  Two vectors are pointing
 *         similar directions if one minus their dot product is less than a
 *         threshold.  Vectors that are 180 degrees out of phase
 *         are similar.  Assumes that vectors are normalized.
 */

namespace Functor {  
  
template<class TInput>
class SimilarVectorsFunctor
{
public:
  SimilarVectorsFunctor()
    {threshold = itk::NumericTraits<ITK_TYPENAME TInput::ValueType>::Zero;};

  ~SimilarVectorsFunctor() {};

  void SetDistanceThreshold(const typename TInput::ValueType &thresh)
    {threshold = thresh;};
  typename TInput::ValueType GetDistanceThreshold() {return (threshold);};
  
  bool operator!=( const SimilarVectorsFunctor & ) const
  {
    return false;
  }
  bool operator==( const SimilarVectorsFunctor & other ) const
  {
    return !(*this != other);
  }
  bool operator()(const TInput &a, const TInput &b)
    {
    typename TInput::ValueType dotProduct = vnl_math_abs(a * b);
    return (1.0 - dotProduct <= threshold);
    };

protected:
  typename TInput::ValueType threshold;
                            
};

} // end namespace Functor 

template <class TInputImage, class TOutputImage, class TMaskImage=TInputImage>
class ITK_EXPORT VectorConnectedComponentImageFilter :
    public ConnectedComponentFunctorImageFilter<TInputImage,TOutputImage,
             Functor::SimilarVectorsFunctor<typename TInputImage::ValueType>,
                                           TMaskImage> 
{
public:
  /** Standard class typedefs. */
  typedef VectorConnectedComponentImageFilter Self;
  typedef ConnectedComponentFunctorImageFilter<TInputImage,TOutputImage, 
             Functor::SimilarVectorsFunctor<typename TInputImage::ValueType>,
             TMaskImage> Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorConnectedComponentImageFilter,ConnectedComponentFunctorImageFilter);

  typedef typename TInputImage::PixelType::ValueType  InputValueType;
  
  virtual void SetDistanceThreshold(const InputValueType& thresh)
    {this->GetFunctor().SetDistanceThreshold(thresh);}

  virtual InputValueType GetDistanceThreshold()
    {return (this->GetFunctor().GetDistanceThreshold());}

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<InputValueType>));
  /** End concept checking */
#endif

protected:
  VectorConnectedComponentImageFilter() {};
  virtual ~VectorConnectedComponentImageFilter() {};

private:
  VectorConnectedComponentImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
