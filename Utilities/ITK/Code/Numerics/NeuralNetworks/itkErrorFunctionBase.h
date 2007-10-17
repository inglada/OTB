/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkErrorFunctionBase.h,v $
  Language:  C++
  Date:      $Date: 2007/08/17 13:10:57 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _itkErrorFunctionBase_h
#define _itkErrorFunctionBase_h

#include "itkFunctionBase.h"

namespace itk
{
namespace Statistics
{

template<class TMeasurementVector, class TTargetVector>
class ErrorFunctionBase : public FunctionBase<TMeasurementVector, TTargetVector>
{
public:
  /** Standard class typedefs. */
  typedef ErrorFunctionBase Self;
  typedef FunctionBase<TMeasurementVector, TTargetVector> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ErrorFunctionBase, FunctionBase);

  /** Error type */
  typedef TMeasurementVector ErrorVectorType;

  /** Output type */
  typedef TTargetVector OutputType;

  typedef Array<double> InternalVectorType;

  virtual OutputType Evaluate(const ErrorVectorType&) const = 0;

  virtual InternalVectorType EvaluateDerivative(const ErrorVectorType&) const = 0;

protected:

  ErrorFunctionBase(){};
  ~ErrorFunctionBase(){};

private:

  ErrorFunctionBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk
} // end namespace Statistics
#endif
