/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToShapeLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-08-04 15:08:28 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryImageToShapeLabelMapFilter_h
#define __itkBinaryImageToShapeLabelMapFilter_h

#include "itkImageToImageFilter.h"
#include "itkShapeLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkShapeLabelMapFilter.h"


namespace itk {

/** \class BinaryImageToShapeLabelMapFilter
 * \brief Converts a binary image to a label map and valuate the shape attributes.
 *
 * A convenient class that converts a binary image to a label map and valuates the 
 * shape attributes at once.
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/584  or 
 * http://www.insight-journal.org/browse/publication/176
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ShapeLabelObject, LabelShapeOpeningImageFilter, BinaryStatisticsOpeningImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage=LabelMap< ShapeLabelObject< unsigned long, ::itk::GetImageDimension<TInputImage>::ImageDimension> > >
class ITK_EXPORT BinaryImageToShapeLabelMapFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryImageToShapeLabelMapFilter              Self;
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
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension );
  itkStaticConstMacro( OutputImageDimension, unsigned int, TInputImage::ImageDimension );
  itkStaticConstMacro( ImageDimension, unsigned int, TInputImage::ImageDimension );

  typedef BinaryImageToLabelMapFilter< InputImageType, OutputImageType > LabelizerType;
  typedef Image< typename OutputImageType::PixelType, itkGetStaticConstMacro(OutputImageDimension)> 
                                                                         ShapeLabelFilterOutput;
  typedef ShapeLabelMapFilter< TOutputImage, ShapeLabelFilterOutput >    LabelObjectValuatorType;

  /** Standard New method. */
  itkNewMacro( Self );

  /** Runtime information support. */
  itkTypeMacro( BinaryImageToShapeLabelMapFilter, ImageToImageFilter );

  /**
   * Set/Get whether the connected components are defined strictly by face connectivity or 
   * by face+edge+vertex connectivity.  Default is FullyConnectedOff.  For objects that
   * are 1 pixel wide, use FullyConnectedOn.
   */
  itkSetMacro( FullyConnected, bool );
  itkGetConstReferenceMacro( FullyConnected, bool );
  itkBooleanMacro( FullyConnected );

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
  itkSetMacro( OutputBackgroundValue, OutputImagePixelType );
  itkGetConstMacro( OutputBackgroundValue, OutputImagePixelType );

  /**
   * Set/Get the value used as "foreground" in the output image.
   * Defaults to NumericTraits<PixelType>::max().
   */
  itkSetMacro( InputForegroundValue, InputImagePixelType );
  itkGetConstMacro( InputForegroundValue, InputImagePixelType );

  /**
   * Set/Get whether the maximum Feret diameter should be computed or not.
   * Default value is false, because of the high computation time required.
   */
  itkSetMacro( ComputeFeretDiameter, bool );
  itkGetConstReferenceMacro( ComputeFeretDiameter, bool );
  itkBooleanMacro( ComputeFeretDiameter );

  /**
   * Set/Get whether the perimeter should be computed or not. 
   * Default value is false, because of the high computation time required.
   */
  itkSetMacro( ComputePerimeter, bool );
  itkGetConstReferenceMacro( ComputePerimeter, bool );
  itkBooleanMacro( ComputePerimeter );


protected:
  BinaryImageToShapeLabelMapFilter();
  ~BinaryImageToShapeLabelMapFilter() {};
  void PrintSelf( std::ostream& os, Indent indent ) const;

  /** BinaryImageToShapeLabelMapFilter needs the entire input be available.
   * Thus, it needs to provide an implementation of GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** BinaryImageToShapeLabelMapFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion( DataObject *itkNotUsed( output ) );
  
  /** Single-threaded version of GenerateData.  
   * This filter delegates to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  BinaryImageToShapeLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                 m_FullyConnected;
  OutputImagePixelType m_OutputBackgroundValue;
  InputImagePixelType  m_InputForegroundValue;
  bool                 m_ComputeFeretDiameter;
  bool                 m_ComputePerimeter;

}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryImageToShapeLabelMapFilter.txx"
#endif

#endif
