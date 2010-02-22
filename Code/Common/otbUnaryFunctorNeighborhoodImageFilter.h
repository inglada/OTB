/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbUnaryFunctorNeighborhoodImageFilter_h
#define __otbUnaryFunctorNeighborhoodImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkProcessObject.h"

namespace otb
{
/** \class UnaryFunctorNeighborhoodImageFilter
 * \brief Implements neighborhood-wise generic operation on image
 *
 * This class is parameterized over the input image type
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used.
 *
 * \ingroup IntensityImageFilters   Multithreaded
 */
template <class TInputImage, class TOutputImage, class TFunction >
class ITK_EXPORT UnaryFunctorNeighborhoodImageFilter
      : public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef UnaryFunctorNeighborhoodImageFilter           Self;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage >  Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorNeighborhoodImageFilter,ImageToImageFilter);


  /** Some convenient typedefs. */
  typedef TFunction                             FunctorType;
  typedef typename Superclass::InputImageType   InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename Superclass::OutputImageType  OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;

  typedef itk::ProcessObject ProcessObjectType;

  /**Set/Get the radius of neighborhood.*/
  itkSetMacro(Radius,InputImageSizeType);
  itkGetMacro(Radius,InputImageSizeType);

  /** Set unsinged int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
    this->Modified();
    return m_Functor;
  }

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  };

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
  {
    m_Functor = functor;
    this->Modified();
  }

  typedef itk::ConstNeighborhoodIterator<TInputImage>     NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType   RadiusType;
  typedef unsigned char RadiusSizeType;

protected:
  /**
   * Constructor
   */
  UnaryFunctorNeighborhoodImageFilter();
  /**
   * Destructor
   */
  virtual ~UnaryFunctorNeighborhoodImageFilter() {};

  /** UnaryFunctorNeighborhoodImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );

  /**
   * Pad the input requested region by radius
   */
  virtual void GenerateInputRequestedRegion(void);

private:
  UnaryFunctorNeighborhoodImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputImageSizeType m_Radius;
  FunctorType m_Functor;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnaryFunctorNeighborhoodImageFilter.txx"
#endif


#endif
