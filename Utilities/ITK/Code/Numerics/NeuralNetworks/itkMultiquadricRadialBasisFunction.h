/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultiquadricRadialBasisFunction.h,v $
  Language:  C++
  Date:      $Date: 2005/08/02 19:17:37 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkMultiquadricRadialBasisFunction_h
#define __itkMultiquadricRadialBasisFunction_h

#include "itkTransferFunctionBase.h"

namespace itk
{
namespace Statistics
{
template<class ScalarType>
class MultiquadricRadialBasisFunction : public TransferFunctionBase<ScalarType>
{
public:

  /** Standard class typedefs. */
  typedef MultiquadricRadialBasisFunction Self;
  typedef TransferFunctionBase<ScalarType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiquadricRadialBasisFunction, TransferFunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self) ;

  /** Evaluate at the specified input position */
  ScalarType Evaluate(const ScalarType& input) const; //{return 0;};

  /** Evaluate the derivative at the specified input position */
  ScalarType EvaluateDerivative(const ScalarType& input) const{return 0;};

  itkSetMacro(Radius,ScalarType);
  itkGetConstReferenceMacro(Radius,ScalarType);
  itkSetMacro(Center,ScalarType);
  itkGetConstReferenceMacro(Center,ScalarType);

protected:

  MultiquadricRadialBasisFunction();
  ~MultiquadricRadialBasisFunction();
 
  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

private:
   
  ScalarType m_Radius;
  ScalarType m_Center;
  
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMultiquadricRadialBasisFunction.txx"
#endif


#endif
