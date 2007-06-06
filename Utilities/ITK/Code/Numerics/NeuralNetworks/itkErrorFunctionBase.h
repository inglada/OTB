/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkErrorFunctionBase.h,v $
  Language:  C++
  Date:      $Date: 2006/04/17 19:34:45 $
  Version:   $Revision: 1.2 $

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

template<class TVector, class TOutput>
class ErrorFunctionBase : public FunctionBase<TVector, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef ErrorFunctionBase Self;
  typedef FunctionBase<TVector, TOutput> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ErrorFunctionBase, FunctionBase);

  /** Error type */
  typedef TVector ErrorVectorType;

  /** Output type */
  typedef TOutput OutputType;

  typedef Array<double> InternalVectorType;

  virtual OutputType Evaluate(const ErrorVectorType&) const = 0;

  //virtual ErrorVectorType EvaluateDerivative(const ErrorVectorType&) const = 0;
  virtual InternalVectorType EvaluateDerivative(const ErrorVectorType&) const = 0;
  
protected:

  ErrorFunctionBase(){};
  virtual ~ErrorFunctionBase(){}; 

private:

  ErrorFunctionBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk
} // end namespace Statistics
#endif
