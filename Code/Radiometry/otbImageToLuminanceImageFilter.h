/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageToLuminanceImageFilter_h
#define __otbImageToLuminanceImageFilter_h

#include "otb_6S.h"
#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "otbVectorImage.h"
#include "itkNumericTraits.h"
#include "itkVariableLengthVector.h"
#include "otbMacro.h"
#include "otbImageMetadataInterfaceBase.h"
#include "otbImageMetadataInterfaceFactory.h"

#include <iomanip>

namespace otb
{
namespace Functor
{
/**
   * \class ImageToLuminanceImageFunctor
   * \brief Add beta to the quotient Input over alpha.
   *
   * \sa ImageToLuminanceImageFilter
   * \ingroup Functor
   * \ingroup Radiometry
 */

template <class TInput, class TOutput>
class ImageToLuminanceImageFunctor
{
public:
  ImageToLuminanceImageFunctor():
    m_Alpha(1.),
    m_Beta(0.)
  {};

  virtual ~ImageToLuminanceImageFunctor() {};

  void SetAlpha(double alpha)
  {
    m_Alpha = alpha;
  };
  void SetBeta(double beta)
  {
    m_Beta = beta;
  };
  double GetAlpha()
  {
    return m_Alpha;
  };
  double GetBeta()
  {
    return m_Beta;
  };


  inline TOutput operator() (const TInput & inPixel) const
  {
    TOutput outPixel;
    double temp;
    temp = static_cast<double>(inPixel)/m_Alpha + m_Beta;
    
    // Set a 1e-3 precision
    itk::OStringStream oss;
    oss.str("");
    oss << std::fixed << std::setprecision(3);
    oss<<temp;
    outPixel = static_cast<TOutput>( atof(oss.str().c_str()) );
    

    return outPixel;
  }

private:
  double m_Alpha;
  double m_Beta;
};
}

/** \class ImageToLuminanceImageFilter
 *  \brief Convert a raw value into a luminance value
 *
 * Transform a classical image into the luminance image. For this it
 * uses the functor ImageToLuminanceImageFunctor calling for each component of each pixel.
 *
 *
 * For Spot image in the dimap format, the correction parameters are
 * retrieved automatically from the metadata
 *
 * \ingroup ImageToLuminanceImageFunctor
 * \ingroup Radiometry
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageToLuminanceImageFilter :
      public UnaryImageFunctorWithVectorImageFilter< TInputImage,
      TOutputImage,
      ITK_TYPENAME Functor::ImageToLuminanceImageFunctor< ITK_TYPENAME TInputImage::InternalPixelType,
      ITK_TYPENAME TOutputImage::InternalPixelType > >
{
public:
  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage         InputImageType;
  typedef TOutputImage        OutputImageType;
  typedef typename Functor::ImageToLuminanceImageFunctor<ITK_TYPENAME InputImageType::InternalPixelType,
  ITK_TYPENAME OutputImageType::InternalPixelType> FunctorType;


  /** "typedef" for standard classes. */
  typedef ImageToLuminanceImageFilter Self;
  typedef UnaryImageFunctorWithVectorImageFilter< InputImageType, OutputImageType, FunctorType > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(ImageToLuminanceImageFilter, UnaryImageFunctorWithVectorImageFiltermageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType                           InputPixelType;
  typedef typename InputImageType::InternalPixelType                   InputInternalPixelType;
  typedef typename InputImageType::RegionType                          InputImageRegionType;
  typedef typename OutputImageType::PixelType                          OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                  OutputInternalPixelType;
  typedef typename OutputImageType::RegionType                         OutputImageRegionType;


  typedef typename itk::VariableLengthVector<double>                   VectorType;

  /** Image size "typedef" definition. */
  typedef typename InputImageType::SizeType SizeType;

  /** Set the absolute calibration gains. */
  itkSetMacro(Alpha, VectorType);

  /** Give the absolute calibration gains. */
  itkGetConstReferenceMacro(Alpha, VectorType);

  /** Set the absolute calibration bias. */
  itkSetMacro(Beta, VectorType);
  /** Give the absolute calibration bias. */
  itkGetConstReferenceMacro(Beta, VectorType);


protected:
  /** Constructor */
  ImageToLuminanceImageFilter()
  {
    m_Alpha.SetSize(0);
    m_Beta.SetSize(0);
  };

  /** Destructor */
  virtual ~ImageToLuminanceImageFilter() {};

  /** Update the functor list and input parameters */
  virtual void BeforeThreadedGenerateData(void)
  {
    ImageMetadataInterfaceBase::Pointer imageMetadataInterface = ImageMetadataInterfaceFactory::CreateIMI(this->GetInput()->GetMetaDataDictionary());
    if(m_Alpha.GetSize() == 0)
    {
      m_Alpha = imageMetadataInterface->GetPhysicalGain(this->GetInput()->GetMetaDataDictionary());
    }

    if(m_Beta.GetSize() == 0)
    {
      m_Beta = imageMetadataInterface->GetPhysicalBias(this->GetInput()->GetMetaDataDictionary());
    }

    if ((m_Alpha.GetSize() != this->GetInput()->GetNumberOfComponentsPerPixel())
      || (m_Beta.GetSize() != this->GetInput()->GetNumberOfComponentsPerPixel()))
    {
      itkExceptionMacro(<<"Alpha and Beta parameters should have the same size as the number of bands");
    }

    otbMsgDevMacro( << "Using correction parameters: ");
    otbMsgDevMacro( << "Alpha (gain): " << m_Alpha);
    otbMsgDevMacro( << "Beta (bias):  " << m_Beta);

    this->GetFunctorVector().clear();
    for (unsigned int i = 0;i<this->GetInput()->GetNumberOfComponentsPerPixel();++i)
    {
      FunctorType functor;
      functor.SetAlpha(m_Alpha[i]);
      functor.SetBeta(m_Beta[i]);
      this->GetFunctorVector().push_back(functor);
    }
  }


private:
  /** Ponderation declaration*/
  VectorType m_Alpha;
  VectorType m_Beta;
};

} // end namespace otb

#endif
