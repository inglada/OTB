/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIdentityTransferFunction.h,v $
  Language:  C++
  Date:      $Date: 2007/08/17 13:10:57 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkIdentityTransferFunction_h
#define __itkIdentityTransferFunction_h

#include "itkTransferFunctionBase.h"

namespace itk
{
namespace Statistics
{

template<class ScalarType>
class IdentityTransferFunction : public TransferFunctionBase<ScalarType>
{
public:

  /** Standard class typedefs. */
  typedef IdentityTransferFunction Self;
  typedef TransferFunctionBase<ScalarType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(IdentityTransferFunction, TransferFunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self) ;


  /** Evaluate at the specified input position */
  virtual ScalarType Evaluate(const ScalarType& input) const;

  /** Evaluate the derivative at the specified input position */
  virtual ScalarType EvaluateDerivative(const ScalarType& input) const;

protected:

  IdentityTransferFunction();
  virtual ~IdentityTransferFunction();

 /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

};//class

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkIdentityTransferFunction.txx"
#endif


#endif
