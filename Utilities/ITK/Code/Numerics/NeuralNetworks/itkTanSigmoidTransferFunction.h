/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTanSigmoidTransferFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:04:59 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkTanSigmoidTransferFunction_h
#define __itkTanSigmoidTransferFunction_h

#include "itkTransferFunctionBase.h"

namespace itk
{
namespace Statistics
{

template<class ScalarType>
class TanSigmoidTransferFunction : public TransferFunctionBase<ScalarType>
{
public:

  /** Standard class typedefs. */
  typedef TanSigmoidTransferFunction       Self;
  typedef TransferFunctionBase<ScalarType> Superclass;
  typedef SmartPointer<Self>               Pointer;
  typedef SmartPointer<const Self>         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(TanSigmoidTransferFunction, TransferFunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Evaluate at the specified input position */
  virtual ScalarType Evaluate(const ScalarType& input) const;

  /** Evaluate the derivative at the specified input position */
  virtual ScalarType EvaluateDerivative(const ScalarType& input) const;

protected:

  TanSigmoidTransferFunction();
  virtual ~TanSigmoidTransferFunction();

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkTanSigmoidTransferFunction.txx"
#endif

#endif
