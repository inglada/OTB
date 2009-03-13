/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsOpeningLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsOpeningLabelMapFilter_h
#define __itkStatisticsOpeningLabelMapFilter_h

#include "itkShapeOpeningLabelMapFilter.h"
#include "itkStatisticsLabelObject.h"

namespace itk {
/** \class StatisticsOpeningLabelMapFilter
 * \brief remove the objects according to the value of their statistics attribute
 *
 * StatisticsOpeningLabelMapFilter removes the objects in a lavbel collection image
 * with an attribute value smaller or greater than a threshold called Lambda.
 * The attributes are the ones of the StatisticsLabelObject.
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa StatisticsLabelObject, BinaryStatisticsOpeningImageFilter, LabelShapeOpeningImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage>
class ITK_EXPORT StatisticsOpeningLabelMapFilter : 
    public ShapeOpeningLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef StatisticsOpeningLabelMapFilter Self;
  typedef ShapeOpeningLabelMapFilter<TImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  
  typedef typename LabelObjectType::AttributeType AttributeType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StatisticsOpeningLabelMapFilter, 
               ShapeOpeningLabelMapFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
/*  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));*/
  /** End concept checking */
#endif



protected:
  StatisticsOpeningLabelMapFilter();
  ~StatisticsOpeningLabelMapFilter() {};

  void GenerateData();

  template <class TAttributeAccessor> void TemplatedGenerateData();

private:
  StatisticsOpeningLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkStatisticsOpeningLabelMapFilter.txx"
#endif

#endif


