/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxBoundaryProfileItem.h,v $
  Language:  C++
  Date:      $Date: 2006/02/06 22:01:53 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxBoundaryProfileItem_h
#define __itkBloxBoundaryProfileItem_h

#include "itkBloxBoundaryPointItem.h"
#include "itkBloxItem.h"

namespace itk
{
template <unsigned int VImageDimension>
class ITK_EXPORT BloxBoundaryProfileItem : public BloxItem
{
public:
  /** Run-time type information (and related methods). */
  itkTypeMacro( BloxBoundaryProfileItem, BloxItem );

  /** The point type used to store the position of the boundary profile */
  typedef Point<double, VImageDimension> PositionType;

  /** The type of vector used to store the gradient of the BoundaryPointItem * */
  typedef CovariantVector<double, VImageDimension> GradientType;

  /** Vector type */
  typedef vnl_vector<double> VectorType;

  /** The type of boundary point item we store pointers to */
  typedef BloxBoundaryPointItem<VImageDimension> BPItemType;

  /** Set the position of the first boundary point in physical space */
  void SetBoundaryPoint(BPItemType * point);

  /** Set and get lower intensity estimates */
  void SetLowerIntensity(double lowerIntensity);
  double GetLowerIntensity(void);

  /** Set and get upper intensity estimates */
  void SetUpperIntensity(double upperIntensity);
  double GetUpperIntensity(void);

  /** Set and get mean estimates */
  //void SetMean(double mean) {m_Mean = mean;}
  void SetMean(double mean);
  double GetMean(void);

  /** Set and get the length of profile */
  void SetProfileLength(unsigned int profileLength);
  unsigned int GetProfileLength(void);

  /** Set and get mean normalized by profile length */
  void SetMeanNormalized(void);
  double GetMeanNormalized(void);
  
  /** Set and get standard deviation */
  void SetStandardDeviation(double standardDeviation);
  double GetStandardDeviation(void);

  /** Set and get standard deviation normalized by profile length */
  void SetStandardDeviationNormalized(void);

  double GetStandardDeviationNormalized(void);

  /** Set and get optimal boundary location */
  void SetOptimalBoundaryLocation(VectorType spatialFunctionOriginVector, VectorType orientation);

  PositionType GetOptimalBoundaryLocation(void);

  /** Set and get the gradient of the boundary profile * */
  void SetGradient(GradientType * gradient);
  GradientType * GetGradient();

  void SetGradient2(GradientType gradient) {m_Gradient2 = gradient;}
  GradientType GetGradient2() {return m_Gradient2;}

  BloxBoundaryProfileItem();
  ~BloxBoundaryProfileItem();

private:
  /** Lower estimated intensity */
  double m_LowerIntensity;

  /** Upper estimated intensity */
  double m_UpperIntensity;

  /** Length of sampling profile */
  unsigned int m_ProfileLength;

  /** Mean location of the boundary along sampling profile*/
  double m_Mean;

  /** Mean normalized by the profile length*/
  double m_MeanNormalized;

  /** Width of blurred boundary */
  double m_StandardDeviation;

  /** Width of blurred boundary normalized by sampling profile */
  double m_StandardDeviationNormalized;

  /** The boundary point to construct a profile from */
  BPItemType * m_BoundaryPoint;

  /** The position of the estimated boundary location */
  PositionType m_OptimalBoundaryLocation;

  /** The gradient of the boundary point (non-normalized) */
  GradientType * m_Gradient;

  GradientType m_Gradient2;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBloxBoundaryProfileItem.txx"
#endif

#endif
