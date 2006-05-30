/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultipleValuedNonLinearVnlOptimizer.h,v $
  Language:  C++
  Date:      $Date: 2005/09/24 21:20:58 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMultipleValuedNonLinearVnlOptimizer_h
#define __itkMultipleValuedNonLinearVnlOptimizer_h

#include "itkMultipleValuedNonLinearOptimizer.h"
#include "itkMultipleValuedVnlCostFunctionAdaptor.h"


namespace itk
{
  
/** \class MultipleValuedNonLinearVnlOptimizer
 * \brief This class is a base for the Optimization methods that 
 * optimize a multi-valued function. 
 *
 * It is an Adaptor class for optimizers provided by the vnl library
 *
 * \ingroup Numerics Optimizers
 */
class ITK_EXPORT MultipleValuedNonLinearVnlOptimizer : 
    public MultipleValuedNonLinearOptimizer
{
public:
  /** Standard class typedefs. */
  typedef MultipleValuedNonLinearVnlOptimizer     Self;
  typedef MultipleValuedNonLinearOptimizer        Superclass;
  typedef SmartPointer<Self>                      Pointer;
  typedef SmartPointer<const Self>                ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( MultipleValuedNonLinearVnlOptimizer, 
                MultipleValueNonLinearOptimizer );

 
  /**  ParametersType typedef.
   *  It defines a position in the optimization search space. */
  typedef Superclass::ParametersType         ParametersType;

  /** Set the cost Function. This method has to be overloaded
   *  by derived classes because the CostFunctionAdaptor requires
   *  to know the number of parameters at construction time. This
   *  number of parameters is obtained at run-time from the itkCostFunction.
   *  As a consequence each derived optimizer should construct its own 
   *  CostFunctionAdaptor when overloading this method  */
  virtual void SetCostFunction( MultipleValuedCostFunction * costFunction ) = 0;

  /**  Define if the Cost function should provide a customized 
       Gradient computation or the gradient can be computed internally
       using a default approach  */
  void SetUseCostFunctionGradient(bool);
  void UseCostFunctionGradientOn()  { this->SetUseCostFunctionGradient( true  ); }
  void UseCostFunctionGradientOff() { this->SetUseCostFunctionGradient( false ); };
  bool GetUseCostFunctionGradient() const;

protected:
  MultipleValuedNonLinearVnlOptimizer();
  virtual ~MultipleValuedNonLinearVnlOptimizer();

  typedef MultipleValuedVnlCostFunctionAdaptor   CostFunctionAdaptorType;

  void SetCostFunctionAdaptor( CostFunctionAdaptorType * adaptor );
  const CostFunctionAdaptorType * GetCostFunctionAdaptor( void ) const;
  CostFunctionAdaptorType * GetCostFunctionAdaptor( void );

  /** The purpose of this method is to get around the lack of const 
   *  correctness in vnl cost_functions and optimizers */
  CostFunctionAdaptorType * GetNonConstCostFunctionAdaptor( void ) const;

private:
  MultipleValuedNonLinearVnlOptimizer(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  CostFunctionAdaptorType * m_CostFunctionAdaptor;
  bool                      m_UseGradient;
};

} // end namespace itk


  

#endif



