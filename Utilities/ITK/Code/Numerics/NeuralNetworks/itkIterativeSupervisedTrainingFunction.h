/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIterativeSupervisedTrainingFunction.h,v $
  Language:  C++
  Date:      $Date: 2007-08-17 13:10:57 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkIterativeSupervisedTrainingFunction_h
#define __itkIterativeSupervisedTrainingFunction_h

#include "itkTrainingFunctionBase.h"


namespace itk
{
namespace Statistics
{

template<class TSample, class TTargetVector, class ScalarType>
class IterativeSupervisedTrainingFunction : public TrainingFunctionBase<TSample, TTargetVector, ScalarType>
{
public:

  typedef IterativeSupervisedTrainingFunction Self;
  typedef TrainingFunctionBase<TSample, TTargetVector, ScalarType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkTypeMacro(IterativeSupervisedTrainingFunction, TrainingFunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename Superclass::NetworkType NetworkType;
  typedef typename Superclass::InternalVectorType InternalVectorType;

  void SetNumOfIterations(long i);

  virtual void Train(NetworkType* net, TSample* samples, TTargetVector* targets);

  itkSetMacro(Threshold, ScalarType);

protected:

  IterativeSupervisedTrainingFunction();
  virtual ~IterativeSupervisedTrainingFunction(){};
  
  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

  ScalarType m_Threshold;
  bool       m_Stop; //stop condition
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkIterativeSupervisedTrainingFunction.txx"
#endif

#endif
