/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTernaryFunctorImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/01/15 04:28:36 $
  Version:   $Revision: 1.35 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTernaryFunctorImageFilter_h
#define __itkTernaryFunctorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace itk
{
  
/** \class TernaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation of three images.
 *
 * This class is parameterized over the types of the three input images
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 * 
 * \ingroup IntensityImageFilters
 */
template <class TInputImage1, class TInputImage2, 
          class TInputImage3, class TOutputImage, class TFunction    >
class ITK_EXPORT TernaryFunctorImageFilter :
    public InPlaceImageFilter<TInputImage1,TOutputImage> 
{
public:
  /** Standard class typedefs. */
  typedef TernaryFunctorImageFilter  Self;
  typedef InPlaceImageFilter<TInputImage1,TOutputImage>   Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TernaryFunctorImageFilter, InPlaceImageFilter);

  /** Some typedefs. */
  typedef TFunction   FunctorType;
  typedef TInputImage1 Input1ImageType;
  typedef typename Input1ImageType::ConstPointer Input1ImagePointer;
  typedef typename Input1ImageType::RegionType Input1ImageRegionType; 
  typedef typename Input1ImageType::PixelType Input1ImagePixelType; 
  typedef TInputImage2 Input2ImageType;
  typedef typename Input2ImageType::ConstPointer Input2ImagePointer;
  typedef typename Input2ImageType::RegionType Input2ImageRegionType; 
  typedef typename Input2ImageType::PixelType Input2ImagePixelType; 
  typedef TInputImage3 Input3ImageType;
  typedef typename Input3ImageType::ConstPointer Input3ImagePointer;
  typedef typename Input3ImageType::RegionType Input3ImageRegionType; 
  typedef typename Input3ImageType::PixelType Input3ImagePixelType; 
  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType OutputImagePixelType;

  /** Connect one of the operands for pixel-wise addition. */
  void SetInput1( const TInputImage1 *image1);

  /** Connect one of the operands for pixel-wise addition. */
  void SetInput2( const TInputImage2 *image2);

  /** Connect one of the operands for pixel-wise addition. */
  void SetInput3( const TInputImage3 *image3);

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer). */
  FunctorType& GetFunctor(void) { return m_Functor; };

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator==() be defined on the functor
   * (or the compiler's default implementation of operator==() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
    {
      if (! (functor == m_Functor) )
        {
        m_Functor = functor;
        this->Modified();
        }
    }

protected:
  TernaryFunctorImageFilter();
  virtual ~TernaryFunctorImageFilter() {};

  /** Validate the presence of all three inputs. If one or more inputs
   * are missing, throw an exception. */
  void BeforeThreadedGenerateData();

  /** TernaryFunctorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  TernaryFunctorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  FunctorType m_Functor;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTernaryFunctorImageFilter.txx"
#endif

#endif
