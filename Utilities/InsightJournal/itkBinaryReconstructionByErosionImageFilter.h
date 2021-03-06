/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryReconstructionByErosionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryReconstructionByErosionImageFilter_h
#define __itkBinaryReconstructionByErosionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkAttributeLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryNotImageFilter.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkBinaryReconstructionLabelMapFilter.h"
#include "itkAttributeOpeningLabelMapFilter.h"
#include "itkLabelMapMaskImageFilter.h"


namespace itk {

/** \class BinaryReconstructionByErosionImageFilter
 * \brief binary reconstruction by erosion of an image
 *
 * Reconstruction by erosion operates on a "marker" image and a "mask"
 * image, and is defined as the erosion of the marker image with
 * respect to the mask image iterated until stability.
 *
 * Geodesic morphology is described in Chapter 6.2 of Pierre Soille's
 * book "Morphological Image Analysis: Principles and Applications",
 * Second Edition, Springer, 2003.
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa MorphologyImageFilter, ReconstructionByErosionImageFilter, BinaryReconstructionByDilationImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage>
class ITK_EXPORT BinaryReconstructionByErosionImageFilter : 
    public ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryReconstructionByErosionImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TInputImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TInputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef BinaryNotImageFilter< InputImageType > NotType;
  typedef AttributeLabelObject<unsigned long, ImageDimension, bool> LabelObjectType;
  typedef typename itk::LabelMap< LabelObjectType > LabelMapType;
  typedef typename itk::BinaryImageToLabelMapFilter< InputImageType, LabelMapType > LabelizerType;
  typedef typename itk::BinaryReconstructionLabelMapFilter< LabelMapType, InputImageType > ReconstructionType;
  typedef typename itk::AttributeOpeningLabelMapFilter< LabelMapType > OpeningType;
  typedef typename itk::LabelMapMaskImageFilter< LabelMapType, OutputImageType > BinarizerType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryReconstructionByErosionImageFilter, 
               ImageToImageFilter);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));
  /** End concept checking */
#endif

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

  /**
   * Set/Get the value used as "foreground" in the output image.
   * Defaults to NumericTraits<PixelType>::max().
   */
  itkSetMacro(ForegroundValue, OutputImagePixelType);
  itkGetConstMacro(ForegroundValue, OutputImagePixelType);

   /** Set the marker image */
  void SetMarkerImage(InputImageType *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 0, const_cast<InputImageType *>(input) );
     }

  /** Get the marker image */
  InputImageType * GetMarkerImage()
    {
    return static_cast<InputImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(0)));
    }

   /** Set the mask image */
  void SetMaskImage(const InputImageType *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 1, const_cast<InputImageType *>(input) );
     }

  /** Get the mask image */
  InputImageType * GetMaskImage()
    {
    return static_cast<InputImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }


protected:
  BinaryReconstructionByErosionImageFilter();
  ~BinaryReconstructionByErosionImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryReconstructionByErosionImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** BinaryReconstructionByErosionImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  BinaryReconstructionByErosionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                m_FullyConnected;
  OutputImagePixelType m_BackgroundValue;
  OutputImagePixelType m_ForegroundValue;
} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryReconstructionByErosionImageFilter.txx"
#endif

#endif


