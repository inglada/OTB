/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegionBasedLevelSetFunctionData.h,v $
  Language:  C++
  Date:      $Date: 2009-05-14 21:46:48 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRegionBasedLevelSetFunctionData_h
#define __itkRegionBasedLevelSetFunctionData_h

#include "itkLightObject.h"

#include "itkVector.h"
#include "itkListSample.h"
#include "itkKdTree.h"
#include "itkKdTreeGenerator.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace itk
{

/** \class RegionBasedLevelSetFunctionData
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
class RegionBasedLevelSetFunctionData : public LightObject
{
public:

  typedef RegionBasedLevelSetFunctionData                   Self;
  typedef LightObject                                       Superclass;
  typedef SmartPointer<Self>                                Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;

  itkStaticConstMacro( ImageDimension, unsigned int, TFeatureImage::ImageDimension );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  itkTypeMacro(RegionBasedLevelSetFunctionData, LightObject);

  typedef TInputImage                                   InputImageType;
  typedef typename InputImageType::Pointer              InputImagePointer;
  typedef typename InputImageType::ConstPointer         InputImageConstPointer;
  typedef typename InputImageType::PixelType            InputPixelType;
  typedef typename InputImageType::RegionType           InputRegionType;
  typedef typename InputImageType::SizeType             InputSizeType;
  typedef typename InputSizeType::SizeValueType         InputSizeValueType;
  typedef typename InputImageType::SpacingType          InputSpacingType;
  typedef typename InputImageType::IndexType            InputIndexType;
  typedef typename InputIndexType::IndexValueType       InputIndexValueType;
  typedef typename InputImageType::PointType            InputPointType;

  typedef TFeatureImage                                 FeatureImageType;
  typedef typename FeatureImageType::Pointer            FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer       FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType          FeaturePixelType;
  typedef typename FeatureImageType::RegionType         FeatureRegionType;
  typedef typename FeatureImageType::SizeType           FeatureSizeType;
  typedef typename FeatureSizeType::SizeValueType       FeatureSizeValueType;
  typedef typename FeatureImageType::SpacingType        FeatureSpacingType;
  typedef typename FeatureImageType::IndexType          FeatureIndexType;
  typedef typename FeatureImageType::PointType          FeaturePointType;

  void CreateHeavisideFunctionOfLevelSetImage( const InputImageType * image );

  template< class TIndex >
  bool VerifyInsideRegion( const TIndex& featureIndex )
    {
    for( unsigned int j = 0; j < ImageDimension; j++ )
      {
      if(  (featureIndex[j] < static_cast< InputIndexValueType >(this->m_Start[j]) )
        || (featureIndex[j] > static_cast< InputIndexValueType >(this->m_End[j]))  )
        {
        return false;
        }
      }
    return true;
    }

  InputIndexType GetIndex( const FeatureIndexType& featureIndex );

  FeatureIndexType GetFeatureIndex( const InputIndexType& inputIndex );


  double              m_WeightedNumberOfPixelsInsideLevelSet;
  double              m_WeightedNumberOfPixelsOutsideLevelSet;

  InputImagePointer   m_HeavisideFunctionOfLevelSetImage;
  InputIndexType      m_Start;
  InputIndexType      m_End;

protected:
  RegionBasedLevelSetFunctionData();
  virtual ~RegionBasedLevelSetFunctionData() {}

private:
  RegionBasedLevelSetFunctionData(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRegionBasedLevelSetFunctionData.txx"
#endif
#endif
