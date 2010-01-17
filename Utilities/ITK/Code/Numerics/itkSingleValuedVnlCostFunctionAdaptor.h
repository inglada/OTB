/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSingleValuedVnlCostFunctionAdaptor.h,v $
  Language:  C++
  Date:      $Date: 2009-05-12 17:19:54 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSingleValuedVnlCostFunctionAdaptor_h
#define __itkSingleValuedVnlCostFunctionAdaptor_h

#include "itkSingleValuedCostFunction.h"
#include "vnl/vnl_cost_function.h"


namespace itk
{
  
/** \class SingleValuedVnlCostFunctionAdaptor
 * \brief This class is an Adaptor that allows to pass 
 * itk::SingleValuedCostFunctions to vnl_optimizers expecting
 * a vnl_cost_function.
 * 
 * This class returns a single valued.
 *
 * \ingroup Numerics Optimizers
 */
class ITK_EXPORT SingleValuedVnlCostFunctionAdaptor : 
    public vnl_cost_function
{
public:

  /** InternalParametersType typedef. */
  typedef   vnl_vector<double>     InternalParametersType;

  /** InternalMeasureType typedef. */
  typedef   double                 InternalMeasureType;

  /** InternalGradientType typedef. */
  typedef   vnl_vector<double>     InternalDerivativeType;

  /** Parameters of the SingleValuedCostFunction */
  typedef SingleValuedCostFunction::ParametersType ParametersType;

  /** Derivatives of the SingleValuedCostFunction */
  typedef SingleValuedCostFunction::DerivativeType DerivativeType;

  /** Type of the SingleValuedCostFunction value */
  typedef SingleValuedCostFunction::MeasureType MeasureType;

  /** Scales typedef */
  typedef Array<double>             ScalesType;

  /** Constructor with size */
  SingleValuedVnlCostFunctionAdaptor(unsigned int spaceDimension);


  /** Set the CostFunction deriving from SingleValuedCostFunction */
  void SetCostFunction( SingleValuedCostFunction * costFunction )
    { m_CostFunction = costFunction; }

  /** Get the CostFunction deriving from SingleValuedCostFunction */
  const SingleValuedCostFunction * GetCostFunction( void ) const
    { return m_CostFunction; }

  /**  Delegate computation of the value to the CostFunction. */
  virtual InternalMeasureType f( const InternalParametersType & inparameters );
    
  /**  Delegate computation of the gradient to the costFunction.  */
  virtual void gradf(const InternalParametersType   & inparameters,
                     InternalDerivativeType   & gradient );
    
  /**  Delegate computation of value and gradient to the costFunction.     */
  virtual void compute(const InternalParametersType   & x,
                       InternalMeasureType      * f, 
                       InternalDerivativeType   * g   );
    
  /**  Convert external derviative measures into internal type   */
  void ConvertExternalToInternalGradient(
    const DerivativeType         & input,
    InternalDerivativeType & output ) const;

  /** Set current parameters scaling. */
  void SetScales(const ScalesType & scales);

  /** Set/Get Negate cost function. The purpose of this boolean flag is to make
   * possible to take certain VNL optimizers that are only minimizers, and use
   * them for maximizing functions. When the boolean flag is set to true, the
   * values returned by GetValue in the internal ITK cost function will be
   * multiplied by -1 before returning it in the f() function. Similar
   * operations will be done for the gradf() and compute() methods. When the
   * boolean flag is set to false, then the values returned by the ITK cost
   * function will be passed unchanged to the VNL optimizers. */
  void SetNegateCostFunction( bool value );
  bool GetNegateCostFunction() const;
  void NegateCostFunctionOn() { m_NegateCostFunction = true; }
  void NegateCostFunctionOff() { m_NegateCostFunction = false; }
 
  /** This AddObserver method allows to simulate that this class derives from
   * an itkObject for the purpose of reporting iteration events. The goal of
   * this method is to allow ITK-vnl optimizer adaptors to get iteration events
   * despite the fact that VNL does not provide callbacks. */
  unsigned long AddObserver(const EventObject & event, Command *) const;

  /** Return the value of the last evaluation to the value of the cost function.
   *  Note that this method DOES NOT triggers a computation of the function or
   *  the derivatives, it only returns previous values. Therefore the values here
   *  are only valid after you invoke the f() or gradf() methods. */
  const MeasureType & GetCachedValue() const;
  const DerivativeType & GetCachedDerivative() const;
  const ParametersType & GetCachedCurrentParameters() const;
  
protected:

  /** This method is intended to be called by the derived classes in order to
   * notify of an iteration event to any Command/Observers */
  void ReportIteration( const EventObject & event ) const;

private:

  SingleValuedCostFunction::Pointer   m_CostFunction;
  bool                                m_ScalesInitialized;
  ScalesType                          m_Scales;
  bool                                m_NegateCostFunction;
  Object::Pointer                     m_Reporter;
  
  mutable MeasureType                 m_CachedValue;
  mutable DerivativeType              m_CachedDerivative;
  mutable ParametersType              m_CachedCurrentParameters;

};  // end of Class CostFunction

    
} // end namespace itk


#endif
