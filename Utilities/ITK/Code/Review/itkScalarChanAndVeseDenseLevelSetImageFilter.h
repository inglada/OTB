/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarChanAndVeseDenseLevelSetImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-07-29 15:14:09 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkScalarChanAndVeseDenseLevelSetImageFilter_h
#define __itkScalarChanAndVeseDenseLevelSetImageFilter_h

#include "itkMultiphaseDenseFiniteDifferenceImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"


/** \class ScalarChanAndVeseDenseLevelSetImageFilter
 * \brief Dense implementation of the Chan and Vese multiphase level set image filter.
 *
 *
 * This code was adapted from the paper:
 *
 *        "An active contour model without edges"
 *         T. Chan and L. Vese.
 *         In Scale-Space Theories in Computer Vision, pages 141–151, 1999.
 *
 * \author Mosaliganti K., Smith B., Gelas A., Gouaillard A., Megason S.
 *
 *  This code was taken from the Insight Journal paper:
 *
 *      "Cell Tracking using Coupled Active Surfaces for Nuclei and Membranes"
 *      http://www.insight-journal.org/browse/publication/642
 *      http://hdl.handle.net/10380/3055
 *
 *  That is based on the papers:
 *
 *      "Level Set Segmentation: Active Contours without edge"
 *      http://www.insight-journal.org/browse/publication/322
 *      http://hdl.handle.net/1926/1532
 *
 *      and
 *
 *      "Level set segmentation using coupled active surfaces"
 *      http://www.insight-journal.org/browse/publication/323
 *      http://hdl.handle.net/1926/1533
 *
 */
namespace itk
{
template < class TInputImage, class TFeatureImage, class TOutputImage, class TFunction,
  class TSharedData >
class ITK_EXPORT ScalarChanAndVeseDenseLevelSetImageFilter:
  public MultiphaseDenseFiniteDifferenceImageFilter< TInputImage, TFeatureImage, TOutputImage,
  TFunction >
{
public:

  typedef ScalarChanAndVeseDenseLevelSetImageFilter         Self;
  typedef MultiphaseDenseFiniteDifferenceImageFilter< TInputImage,
    TFeatureImage, TOutputImage, TFunction >                Superclass;
  typedef SmartPointer<Self>                                Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ScalarChanAndVeseDenseLevelSetImageFilter,
    MultiphaseDenseFiniteDifferenceImageFilter );

  itkStaticConstMacro( ImageDimension, unsigned int, TInputImage::ImageDimension );

  /** Inherited typedef from the superclass. */
  typedef typename Superclass::InputImageType         InputImageType;
  typedef typename Superclass::InputImagePointer      InputImagePointer;
  typedef typename Superclass::InputPointType         InputPointType;
  typedef typename Superclass::ValueType              ValueType;
  typedef typename InputImageType::SpacingType        InputSpacingType;

  typedef TFeatureImage                               FeatureImageType;
  typedef typename FeatureImageType::Pointer          FeatureImagePointer;
  typedef typename FeatureImageType::PixelType        FeaturePixelType;
  typedef typename FeatureImageType::IndexType        FeatureIndexType;
  typedef typename FeatureIndexType::IndexValueType   FeatureIndexValueType;
  typedef typename FeatureImageType::RegionType       FeatureRegionType;

  /** Output image type typedefs */
  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::IndexType         IndexType;
  typedef typename OutputImageType::PixelType         OutputPixelType;

  typedef typename Superclass::TimeStepType           TimeStepType;
  typedef typename Superclass::FiniteDifferenceFunctionType
                                                      FiniteDifferenceFunctionType;

  typedef TFunction                                   FunctionType;
  typedef typename FunctionType::Pointer              FunctionPointer;

  typedef TSharedData                                 SharedDataType;
  typedef typename SharedDataType::Pointer            SharedDataPointer;

  typedef RegionOfInterestImageFilter<
    FeatureImageType, FeatureImageType >              ROIFilterType;
  typedef typename ROIFilterType::Pointer             ROIFilterPointer;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<OutputPixelType>) );
  /** End concept checking */
#endif

  /** Set/Get the feature image to be used for speed function of the level set
   *  equation.  Equivalent to calling Set/GetInput(1, ..) */
  virtual void SetFeatureImage( const FeatureImagePointer f )
    {
    this->SetInput( f );
    }

protected:
  ScalarChanAndVeseDenseLevelSetImageFilter()
    {
    this->m_SharedData = SharedDataType::New();
    }
  ~ScalarChanAndVeseDenseLevelSetImageFilter(){}

  SharedDataPointer   m_SharedData;

  virtual void Initialize();
  virtual void InitializeIteration();

private:
  ScalarChanAndVeseDenseLevelSetImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScalarChanAndVeseDenseLevelSetImageFilter.txx"
#endif

#endif
