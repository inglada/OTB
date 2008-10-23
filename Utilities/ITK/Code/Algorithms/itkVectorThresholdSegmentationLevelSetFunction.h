/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorThresholdSegmentationLevelSetFunction.h,v $
  Language:  C++
  Date:      $Date: 2003-12-23 18:33:14 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorThresholdSegmentationLevelSetFunction_h_
#define __itkVectorThresholdSegmentationLevelSetFunction_h_

#include "itkSegmentationLevelSetFunction.h"
#include "itkNumericTraits.h"
#include "itkMahalanobisDistanceMembershipFunction.h"
namespace itk {

/** \class VectorThresholdSegmentationLevelSetFunction
 *    
 * \brief This function is used in VectorThresholdSegmentationLevelSetImageFilter to
 * segment structures in images based on the Mahalanobis distance.
 *
 *   \par CREDITS
 *   This class was contributed to ITK by Stefan Lindenau 
 *   http://www.itk.org/pipermail/insight-users/2003-December/005969.html
 *   
 * \par  SegmentationLevelSetFunction is a subclass of the generic LevelSetFunction.
 * It useful for segmentations based on intensity values in an image.  It works
 * by constructing a speed term (feature image) with positive values inside an
 * intensity window (between a low and high threshold) and negative values
 * outside that intensity window.  The evolving level set front will lock onto
 * regions that are at the edges of the intensity window.
 *
 *
 *  \par
 *  Image \f$ f(x) \f$ is thresholded pixel by pixel using threshold \f$T\f$
 *  according to the following formula.
 *
 *  \par
 *  \f[
 *           f(x) = T - MahalanobisDistance(x)
 *  \f]
 *
 *  \sa SegmentationLevelSetImageFunction
 *  \sa ThresholdSegmentationLevelSetImageFilter
 *  \sa MahalanobisDistanceMembershipFunction*/
template <class TImageType, class TFeatureImageType>
class ITK_EXPORT VectorThresholdSegmentationLevelSetFunction
  : public SegmentationLevelSetFunction<TImageType, TFeatureImageType>
{
public:
  /** Standard class typedefs. */
  typedef VectorThresholdSegmentationLevelSetFunction Self;
  typedef SegmentationLevelSetFunction<TImageType, TFeatureImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  typedef TFeatureImageType FeatureImageType;

  

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( VectorThresholdSegmentationLevelSetFunction, SegmentationLevelSetFunction );

  /** Extract some parameters from the superclass. */
  typedef typename Superclass::ImageType ImageType;
  typedef typename Superclass::ScalarValueType ScalarValueType;
  typedef typename Superclass::FeatureScalarType FeatureScalarType;
  typedef typename Superclass::RadiusType RadiusType;

  /** Extract some parameters from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      Superclass::ImageDimension);

  /** Extract the number of components in the vector pixel type . */
  typedef typename FeatureImageType::PixelType FeatureImagePixelType;
  itkStaticConstMacro(NumberOfComponents, unsigned int,
                        FeatureImagePixelType::Dimension);


  typedef Statistics::MahalanobisDistanceMembershipFunction<FeatureScalarType> MahalanobisFunctionType;
  typedef typename MahalanobisFunctionType::Pointer MahalanobisFunctionPointer;
  typedef typename MahalanobisFunctionType::MeanVectorType MeanVectorType;
  typedef typename MahalanobisFunctionType::CovarianceMatrixType CovarianceMatrixType;
  
  /** Set/Get mean and covariance */
  void SetMean(const MeanVectorType &mean) 
  {  m_Mahalanobis->SetMean(mean); }
  const MeanVectorType & GetMean() const 
  {  return m_Mahalanobis->GetMean(); }

  
  void SetCovariance(const CovarianceMatrixType &cov) 
  { m_Mahalanobis->SetCovariance(cov); }
  const CovarianceMatrixType & GetCovariance() const
  { return m_Mahalanobis->GetCovariance(); }
  
  /** Set/Get the threshold value for the MahanalobisDistance */
  void SetThreshold(ScalarValueType thr) 
  {
  m_Threshold = thr;
  
  }
  ScalarValueType GetThreshold() 
  {
  return m_Threshold;
  }
  


  virtual void CalculateSpeedImage();

  virtual void Initialize(const RadiusType &r)
  {
    Superclass::Initialize(r);
    
    this->SetAdvectionWeight( NumericTraits<ScalarValueType>::Zero);
    this->SetPropagationWeight(-1.0 * NumericTraits<ScalarValueType>::One);
    this->SetCurvatureWeight(NumericTraits<ScalarValueType>::One);
  }

  
protected:
  VectorThresholdSegmentationLevelSetFunction()
  {
    MeanVectorType mean( NumberOfComponents );
    CovarianceMatrixType covariance( NumberOfComponents, NumberOfComponents );
    
    mean.fill(NumericTraits<ITK_TYPENAME FeatureScalarType::ValueType>::Zero);
    covariance.fill(NumericTraits<ITK_TYPENAME FeatureScalarType::ValueType>::Zero);
  
    m_Mahalanobis = MahalanobisFunctionType::New();
    m_Mahalanobis->SetMean(mean);
    m_Mahalanobis->SetCovariance(covariance);
    
    
    this->SetAdvectionWeight(0.0);
    this->SetPropagationWeight(1.0);
    this->SetThreshold(1.8);
  }
  virtual ~VectorThresholdSegmentationLevelSetFunction(){}

  VectorThresholdSegmentationLevelSetFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  void PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os, indent );
    os << indent << "MahalanobisFunction: " << m_Mahalanobis << std::endl;
  os << indent << "ThresholdValue: " << m_Threshold << std::endl;
  }
  
  
  MahalanobisFunctionPointer m_Mahalanobis;
  ScalarValueType  m_Threshold;
  
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorThresholdSegmentationLevelSetFunction.txx"
#endif

#endif
