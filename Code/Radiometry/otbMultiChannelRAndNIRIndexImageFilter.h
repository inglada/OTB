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
#ifndef __otbMultiChannelRAndNIRIndexImageFilter_h
#define __otbMultiChannelRAndNIRIndexImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class MultiChannelRAndNIRIndexImageFilter
 * \brief Implements mutli channel R and NIR  pixel-wise generic index operation on one vector image.
 *
 * This class is parameterized over the type of the input image and
 * the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 * 
 * \sa UnaryFunctorImageFilter
 */
template <class TInputImage, class TOutputImage,
	  class TFunction = Functor::NDVI< typename TInputImage::InternalPixelType,
					   typename TInputImage::InternalPixelType,
					   typename TOutputImage::PixelType>  >
class ITK_EXPORT MultiChannelRAndNIRIndexImageFilter 
  : public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction>
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelRAndNIRIndexImageFilter                               Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                           Pointer;
  typedef itk::SmartPointer<const Self>                                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelRAndNIRIndexImageFilter, UnaryFunctorImageFilter);

  /** Some typedefs. */
  typedef TFunction   FunctorType;
 
  /** Set/Get the red channel index. Value must be in [1...[ */
  itkSetMacro(RedIndex,unsigned int);
  itkGetMacro(RedIndex,unsigned int);
  /** Set/Get the nir channel index. Value must be in [1...[ */
  itkSetMacro(NIRIndex,unsigned int);
  itkGetMacro(NIRIndex,unsigned int);

protected:
  /// Constructor
  MultiChannelRAndNIRIndexImageFilter(): m_RedIndex(3), m_NIRIndex(4) {};
  /// Destructor
  virtual ~MultiChannelRAndNIRIndexImageFilter() {};
  /// Before generating data, set functor parameters
  virtual void BeforeThreadedGenerateData()
  {
    if(m_RedIndex < 1 || m_NIRIndex < 1)
      {
      itkExceptionMacro(<<"Channel indices must belong to range [1, ...[");
      }
    this->GetFunctor().SetRedIndex(m_RedIndex);
    this->GetFunctor().SetNIRIndex(m_NIRIndex);
  }
  /// PrintSelf Method
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    this->Superclass::PrintSelf(os,indent);
    os << indent << "Red index: "<<m_RedIndex<<std::endl;
    os << indent << "NIR index: "<<m_NIRIndex<<std::endl;
  }
private:
  MultiChannelRAndNIRIndexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  /** Red channel index */
  unsigned int m_RedIndex;
  /** NIR channel index */
  unsigned int m_NIRIndex;
};
} // end namespace otb

#endif