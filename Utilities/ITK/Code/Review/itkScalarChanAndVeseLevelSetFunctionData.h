/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarChanAndVeseLevelSetFunctionData.h,v $
  Language:  C++
  Date:      $Date: 2009-05-14 21:46:50 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkScalarChanAndVeseLevelSetFunctionData_h
#define __itkScalarChanAndVeseLevelSetFunctionData_h

#include "itkRegionBasedLevelSetFunctionData.h"

namespace itk
{

/** \class ScalarChanAndVeseLevelSetFunctionData
 *
 * \brief Helper class used to share data in the ScalarChanAndVeseLevelSetFunction.
 *
 * This class holds cache data used during the computation of the LevelSet updates.
 *
 * Based on the paper:
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
 *
 */
template < class TInputImage, class TFeatureImage >
class ScalarChanAndVeseLevelSetFunctionData :
  public RegionBasedLevelSetFunctionData< TInputImage, TFeatureImage >
{
public:

  typedef ScalarChanAndVeseLevelSetFunctionData                         Self;
  typedef RegionBasedLevelSetFunctionData< TInputImage, TFeatureImage > Superclass;
  typedef SmartPointer<Self>                                            Pointer;
  typedef SmartPointer<const Self>                                      ConstPointer;

  itkStaticConstMacro( ImageDimension, unsigned int, TFeatureImage::ImageDimension );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  itkTypeMacro(ScalarChanAndVeseLevelSetFunctionData, RegionBasedLevelSetFunctionData );

  typedef TInputImage                                   InputImageType;
  typedef typename Superclass::InputImagePointer        InputImagePointer;
  typedef typename Superclass::InputImageConstPointer   InputImageConstPointer;
  typedef typename Superclass::InputPixelType           InputPixelType;
  typedef typename Superclass::InputRegionType          InputRegionType;
  typedef typename Superclass::InputSizeType            InputSizeType;
  typedef typename Superclass::InputSizeValueType       InputSizeValueType;
  typedef typename Superclass::InputSpacingType         InputSpacingType;
  typedef typename Superclass::InputIndexType           InputIndexType;
  typedef typename Superclass::InputIndexValueType      InputIndexValueType;
  typedef typename Superclass::InputPointType           InputPointType;

  typedef TFeatureImage                                 FeatureImageType;
  typedef typename Superclass::FeatureImagePointer      FeatureImagePointer;
  typedef typename Superclass::FeatureImageConstPointer FeatureImageConstPointer;
  typedef typename Superclass::FeaturePixelType         FeaturePixelType;
  typedef typename Superclass::FeatureRegionType        FeatureRegionType;
  typedef typename Superclass::FeatureSizeType          FeatureSizeType;
  typedef typename Superclass::FeatureSizeValueType     FeatureSizeValueType;
  typedef typename Superclass::FeatureSpacingType       FeatureSpacingType;
  typedef typename Superclass::FeatureIndexType         FeatureIndexType;
  typedef typename Superclass::FeaturePointType         FeaturePointType;

  double              m_BackgroundConstantValues;
  double              m_ForegroundConstantValues;
  double              m_WeightedSumOfPixelValuesInsideLevelSet;
  double              m_WeightedSumOfPixelValuesOutsideLevelSet;

protected:
  ScalarChanAndVeseLevelSetFunctionData() : Superclass()
    {
    m_BackgroundConstantValues = 0.;
    m_ForegroundConstantValues = 0.;
    m_WeightedSumOfPixelValuesInsideLevelSet = 0.;
    m_WeightedSumOfPixelValuesOutsideLevelSet = 0.;
    }
  virtual ~ScalarChanAndVeseLevelSetFunctionData() {}

private:
  ScalarChanAndVeseLevelSetFunctionData(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} //end namespace itk

#endif
