/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelImageToShapeLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-08-11 14:24:42 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelImageToShapeLabelMapFilter_h
#define __itkLabelImageToShapeLabelMapFilter_h

#include "itkImageToImageFilter.h"
#include "itkShapeLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkShapeLabelMapFilter.h"


namespace itk {

/** \class LabelImageToShapeLabelMapFilter
 * \brief Converts a label image to a label map and valuates the shape attributes
 *  
 *  A convenient class that converts a label image to a label map and valuates the shape attribute at once.
 *
 * This implementation was taken from the Insight Journal paper:
 *
 * http://hdl.handle.net/1926/584  or 
 * http://www.insight-journal.org/browse/publication/176
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ShapeLabelObject, LabelShapeOpeningImageFilter, LabelStatisticsOpeningImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage=LabelMap< ShapeLabelObject< ITK_TYPENAME TInputImage::PixelType, ::itk::GetImageDimension<TInputImage>::ImageDimension> > >
class ITK_EXPORT LabelImageToShapeLabelMapFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelImageToShapeLabelMapFilter               Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;

  typedef TOutputImage                              OutputImageType;
  typedef typename OutputImageType::Pointer         OutputImagePointer;
  typedef typename OutputImageType::ConstPointer    OutputImageConstPointer;
  typedef typename OutputImageType::RegionType      OutputImageRegionType;
  typedef typename OutputImageType::PixelType       OutputImagePixelType;
  typedef typename OutputImageType::LabelObjectType LabelObjectType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  typedef LabelImageToLabelMapFilter< InputImageType, OutputImageType >
                         LabelizerType;
  typedef Image< typename OutputImageType::PixelType, itkGetStaticConstMacro(OutputImageDimension)>
                         ShapeLabelFilterOutput;
  typedef ShapeLabelMapFilter< TOutputImage, ShapeLabelFilterOutput >
                         LabelObjectValuatorType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelImageToShapeLabelMapFilter, ImageToImageFilter);

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
  itkSetMacro( BackgroundValue, OutputImagePixelType );
  itkGetConstMacro( BackgroundValue, OutputImagePixelType );

  /**
   * Set/Get whether the maximum Feret diameter should be computed or not. 
   * Default value is false, because of the high computation time required.
   */
  itkSetMacro(ComputeFeretDiameter, bool);
  itkGetConstReferenceMacro(ComputeFeretDiameter, bool);
  itkBooleanMacro(ComputeFeretDiameter);

  /**
   * Set/Get whether the perimeter should be computed or not.
   * Default value is false, because of the high computation time required.
   */
  itkSetMacro(ComputePerimeter, bool);
  itkGetConstReferenceMacro(ComputePerimeter, bool);
  itkBooleanMacro(ComputePerimeter);

protected:
  LabelImageToShapeLabelMapFilter();
  ~LabelImageToShapeLabelMapFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** LabelImageToShapeLabelMapFilter needs the entire input be available.
   * Thus, it needs to provide an implementation of GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** LabelImageToShapeLabelMapFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.
   * This filter delegates to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  LabelImageToShapeLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImagePixelType m_BackgroundValue;
  bool                 m_ComputeFeretDiameter;
  bool                 m_ComputePerimeter;

}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelImageToShapeLabelMapFilter.txx"
#endif

#endif
