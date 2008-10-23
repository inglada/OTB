/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_ITKOptimizers.cxx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:30:12 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkAmoebaOptimizer.h"
#include "itkConjugateGradientOptimizer.h"
#include "itkGradientDescentOptimizer.h"
#include "itkLBFGSOptimizer.h"
#include "itkLevenbergMarquardtOptimizer.h"
#include "itkMultipleValuedNonLinearOptimizer.h"
#include "itkMultipleValuedNonLinearVnlOptimizer.h"
#include "itkNonLinearOptimizer.h"
#include "itkOnePlusOneEvolutionaryOptimizer.h"
#include "itkOptimizer.h"
#include "itkQuaternionRigidTransformGradientDescentOptimizer.h"
#include "itkRegularStepGradientDescentBaseOptimizer.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkSingleValuedNonLinearOptimizer.h"
#include "itkSingleValuedNonLinearVnlOptimizer.h"
#include "itkVersorTransformOptimizer.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(ITKOptimizers);
  namespace wrappers
  {
    ITK_WRAP_OBJECT(AmoebaOptimizer);
    ITK_WRAP_OBJECT(ConjugateGradientOptimizer);
    ITK_WRAP_OBJECT(GradientDescentOptimizer);
    ITK_WRAP_OBJECT(LBFGSOptimizer);
    ITK_WRAP_OBJECT(LevenbergMarquardtOptimizer);
    ITK_WRAP_OBJECT(MultipleValuedNonLinearOptimizer);
    ITK_WRAP_OBJECT(MultipleValuedNonLinearVnlOptimizer);
    ITK_WRAP_OBJECT(NonLinearOptimizer);
    ITK_WRAP_OBJECT(OnePlusOneEvolutionaryOptimizer);
    ITK_WRAP_OBJECT(Optimizer);
    ITK_WRAP_OBJECT(QuaternionRigidTransformGradientDescentOptimizer);
    ITK_WRAP_OBJECT(RegularStepGradientDescentBaseOptimizer);
    ITK_WRAP_OBJECT(RegularStepGradientDescentOptimizer);
    ITK_WRAP_OBJECT(SingleValuedNonLinearOptimizer);
    ITK_WRAP_OBJECT(SingleValuedNonLinearVnlOptimizer);
    ITK_WRAP_OBJECT(VersorTransformOptimizer);
  }
}

#endif
