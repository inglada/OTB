/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSinRegularizedHeavisideStepFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-05-09 21:35:53 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSinRegularizedHeavisideStepFunction_h
#define __itkSinRegularizedHeavisideStepFunction_h

#include "itkRegularizedHeavisideStepFunction.h"

namespace itk
{

/** \class SinRegularizedHeavisideStepFunction
 *
 * \brief Sin-based implementation of the Regularized (smoothed) Heaviside functions.
 *
 * \author Mosaliganti K., Smith B., Gelas A., Gouaillard A., Megason S.
 *
 *  This code was taken from the Insight Journal paper:
 *
 *      "Cell Tracking using Coupled Active Surfaces for Nuclei and Membranes"
 *      http://www.insight-journal.org/browse/publication/642
 *      http://hdl.handle.net/10380/3055
 *
 *  That is based on the papers:
 *
 *      "Level Set Segmentation: Active Contours without edge"
 *      http://www.insight-journal.org/browse/publication/322
 *      http://hdl.handle.net/1926/1532
 *
 *      and
 *
 *      "Level set segmentation using coupled active surfaces"
 *      http://www.insight-journal.org/browse/publication/323
 *      http://hdl.handle.net/1926/1533
 *
 *
 */
template< class TInput = float, class TOutput = double >
class SinRegularizedHeavisideStepFunction : 
 public RegularizedHeavisideStepFunction< TInput, TOutput >
{
public:
  typedef SinRegularizedHeavisideStepFunction                   Self;
  typedef RegularizedHeavisideStepFunction< TInput, TOutput >   Superclass;
  typedef SmartPointer<Self>                                    Pointer;
  typedef SmartPointer<const Self>                              ConstPointer;

  itkNewMacro( Self );
 
  itkTypeMacro( SinRegularizedHeavisideStepFunction, RegularizedHeavisideStepFunction );

  typedef typename Superclass::InputType                        InputType;
  typedef typename Superclass::OutputType                       OutputType;
  typedef typename Superclass::RealType                         RealType;

  /** Evaluate at the specified input position */
  virtual OutputType Evaluate( const InputType& input ) const
    {
    if( input > this->GetEpsilon() )
      {
      return 1.0;
      }
    else
      {
      if( input < -this->GetEpsilon() )
        {
        return 0.0;
        }
      else
        {
        const RealType angleFactor = 0.5 * vnl_math::pi * this->GetOneOverEpsilon();
        const RealType angle = input * angleFactor;
        return 0.5 * ( 1.0 + vcl_sin( angle ) );
        }
      }
    }

  /** Evaluate the derivative at the specified input position */
  virtual OutputType EvaluateDerivative( const InputType& input ) const
    {
    if( vnl_math_abs( input ) > this->GetEpsilon() )
      {
      return 0.0;
      }
    else
      {
      const RealType angleFactor = 0.5 * vnl_math::pi * this->GetOneOverEpsilon();
      const RealType angle = input * angleFactor;
      return 0.5 * angleFactor * vcl_cos( angle );
      }
    }

protected:
  SinRegularizedHeavisideStepFunction() {}
  virtual ~SinRegularizedHeavisideStepFunction() {}

private:
  SinRegularizedHeavisideStepFunction(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented

};

}

#endif
