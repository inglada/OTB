/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarChanAndVeseSparseLevelSetImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-05-16 12:35:12 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkScalarChanAndVeseSparseLevelSetImageFilter_h
#define __itkScalarChanAndVeseSparseLevelSetImageFilter_h

#include "itkMultiphaseSparseFiniteDifferenceImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"

/** \class ScalarChanAndVeseSparseLevelSetImageFilter
 * \brief Sparse implementation of the Chan and Vese multiphase level set image filter.
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
  class TSharedData, typename TIdCell = unsigned int >
class ITK_EXPORT ScalarChanAndVeseSparseLevelSetImageFilter :
public MultiphaseSparseFiniteDifferenceImageFilter< TInputImage, TOutputImage, TFunction, TIdCell >
{
public:
  typedef ScalarChanAndVeseSparseLevelSetImageFilter      Self;
  typedef MultiphaseSparseFiniteDifferenceImageFilter<
    TInputImage, TOutputImage, TFunction, TIdCell >       Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ScalarChanAndVeseSparseLevelSetImageFilter,
    MultiphaseSparseFiniteDifferenceImageFilter );

  itkStaticConstMacro( ImageDimension, unsigned int, TInputImage::ImageDimension );

  /** Inherited typedef from the superclass. */
  typedef TFeatureImage                                 FeatureImageType;
  typedef typename FeatureImageType::Pointer            FeatureImagePtr;
  typedef typename FeatureImageType::PixelType          FeaturePixelType;
  typedef typename FeatureImageType::IndexType          FeatureIndexType;
  typedef typename FeatureIndexType::IndexValueType     FeatureIndexValueType;
  typedef typename FeatureImageType::RegionType         FeatureRegionType;

  /** Output image type typedefs */
  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::InputImagePointer        InputImagePointer;
  typedef typename Superclass::InputPointType           InputPointType;
  typedef typename Superclass::InputSpacingType         InputSpacingType;

  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename OutputImageType::ValueType           ValueType;
  typedef typename OutputImageType::IndexType           IndexType;
  typedef typename OutputImageType::PixelType           OutputPixelType;

  typedef typename Superclass::TimeStepType             TimeStepType;
  typedef typename Superclass::FiniteDifferenceFunctionType
                                                        FiniteDifferenceFunctionType;

  typedef typename Superclass::IdCellType               IdCellType;

  typedef TFunction                                     FunctionType;
  typedef typename FunctionType::Pointer                FunctionPtr;

  typedef TSharedData                                   SharedDataType;
  typedef typename SharedDataType::Pointer              SharedDataPointer;

  typedef RegionOfInterestImageFilter<
    FeatureImageType, FeatureImageType >                ROIFilterType;
  typedef typename ROIFilterType::Pointer               ROIFilterPointer;


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<OutputPixelType>) );
  /** End concept checking */
#endif

  /** Set/Get the feature image to be used for speed function of the level set
   *  equation.  Equivalent to calling Set/GetInput(1, ..) */
  virtual void SetFeatureImage(const FeatureImageType *f)
    {
    this->ProcessObject::SetNthInput( 0, const_cast< FeatureImageType *>(f) );
    }

  virtual const FeatureImageType * GetFeatureImage() const
    {
    return (static_cast< const FeatureImageType*>(this->ProcessObject::GetInput(0)));
    }

protected:
  ScalarChanAndVeseSparseLevelSetImageFilter()
    {
    this->SetNumberOfLayers(5); // Narrow-band usage
    this->m_SharedData = SharedDataType::New();
    }

  ~ScalarChanAndVeseSparseLevelSetImageFilter() {}

  SharedDataPointer     m_SharedData;

  virtual void Initialize();
  virtual void InitializeIteration();
  virtual void UpdatePixel( unsigned int functionIndex,
    unsigned int idx, NeighborhoodIterator< OutputImageType > &iterator,
    ValueType &newValue, bool &status );
};

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScalarChanAndVeseSparseLevelSetImageFilter.txx"
#endif

#endif
