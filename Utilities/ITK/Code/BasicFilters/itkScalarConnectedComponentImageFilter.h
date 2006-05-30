/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarConnectedComponentImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/01/15 04:28:36 $
  Version:   $Revision: 1.3 $

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScalarConnectedComponentImageFilter_h
#define __itkScalarConnectedComponentImageFilter_h

#include "vnl/vnl_math.h"
#include "itkNumericTraits.h"
#include "itkConnectedComponentFunctorImageFilter.h"

namespace itk
{

/** \class ScalarConnectedComponentImageFilter
 *
 *  \brief A connected components filter that labels the
 *         objects in an arbitrary image.  Two pixels are
 *         similar if they are within threshold of each other.
 *         Uses ConnectedComponentFunctorImageFilter.
 */

namespace Functor {  
  
template<class TInput>
class SimilarPixelsFunctor
{
public:
  SimilarPixelsFunctor()
    {m_Threshold = itk::NumericTraits<TInput>::Zero;};
  ~SimilarPixelsFunctor() {};
  bool operator!=( const SimilarPixelsFunctor & other ) const
  {
    if( m_Threshold != other.m_Threshold )
        {
        return true;
        }
    return false;
   }
  bool operator==( const SimilarPixelsFunctor & other ) const
  {
    return !(*this != other);
  }

  void SetDistanceThreshold(const TInput &thresh) {m_Threshold = thresh;};
  TInput GetDistanceThreshold() {return (m_Threshold);};
  
  bool operator()(const TInput &a, const TInput &b) {
    return (vnl_math_abs(a-b) <= m_Threshold);
  };

protected:
  TInput m_Threshold;
                            
};

} // end namespace Functor 

template <class TInputImage, class TOutputImage, class TMaskImage=TInputImage>
class ITK_EXPORT ScalarConnectedComponentImageFilter :
    public ConnectedComponentFunctorImageFilter<TInputImage,TOutputImage,
             Functor::SimilarPixelsFunctor<typename TInputImage::ValueType>,
                                           TMaskImage> 
{
public:
  /** Standard class typedefs. */
  typedef ScalarConnectedComponentImageFilter Self;
  typedef ConnectedComponentFunctorImageFilter<TInputImage,TOutputImage, 
             Functor::SimilarPixelsFunctor<typename TInputImage::ValueType>,
             TMaskImage> Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ScalarConnectedComponentImageFilter,ConnectedComponentFunctorImageFilter);

  typedef typename TInputImage::PixelType  InputPixelType;
  
  virtual void SetDistanceThreshold(const InputPixelType& thresh)
    {this->GetFunctor().SetDistanceThreshold(thresh);}

  virtual InputPixelType GetDistanceThreshold()
    {return (this->GetFunctor().GetDistanceThreshold());}
  
protected:
  ScalarConnectedComponentImageFilter() {};
  virtual ~ScalarConnectedComponentImageFilter() {};

private:
  ScalarConnectedComponentImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
