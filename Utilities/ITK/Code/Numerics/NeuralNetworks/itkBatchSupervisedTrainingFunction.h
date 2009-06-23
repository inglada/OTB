/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBatchSupervisedTrainingFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-01-24 21:33:49 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBatchSupervisedTrainingFunction_h
#define __itkBatchSupervisedTrainingFunction_h

#include "itkTrainingFunctionBase.h"

namespace itk
{
namespace Statistics
{

template<class TSample, class TTargetVector, class ScalarType>
class BatchSupervisedTrainingFunction : public TrainingFunctionBase<TSample, TTargetVector, ScalarType>
{
public:

  typedef BatchSupervisedTrainingFunction  Self;
  typedef TrainingFunctionBase<TSample, TTargetVector, ScalarType>
                                           Superclass;
  typedef SmartPointer<Self>               Pointer;
  typedef SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkTypeMacro(BatchSupervisedTrainingFunction, TrainingFunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename Superclass::NetworkType        NetworkType;
  typedef typename Superclass::InternalVectorType InternalVectorType;

  /** Set the number of iterations */
  void SetNumOfIterations(long i);

  virtual void Train(NetworkType* net, TSample* samples, TTargetVector* targets);

  itkSetMacro(Threshold, ScalarType);

protected:

  BatchSupervisedTrainingFunction();
  virtual ~BatchSupervisedTrainingFunction(){};

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

  ScalarType  m_Threshold;
  bool        m_Stop; //stop condition
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkBatchSupervisedTrainingFunction.txx"
#endif

#endif
