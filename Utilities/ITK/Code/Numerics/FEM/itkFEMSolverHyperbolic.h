/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMSolverHyperbolic.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:44 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFEMSolverHyperbolic_h
#define __itkFEMSolverHyperbolic_h

#include "itkFEMSolver.h"

namespace itk {
namespace fem {




/**
 * \class SolverHyperbolic
 * \brief Solver class suitable for hyperbolic problems.
 *
 * M*ddu + C*du + K*u=F
 *
 */
class SolverHyperbolic : public Solver
{
public:

  /**
   * Default constructor
   */
  SolverHyperbolic();

  /**
   * Initialize the linear system wrapper.
   */
  virtual void InitializeLinearSystemWrapper(void);

  /**
   * When assembling the element matrix into master matrix, we
   * need to assemble the mass matrix too.
   */
  virtual void AssembleElementMatrix(Element::Pointer e);

  /**
   * Initializes the storasge for all master matrices.
   */
  virtual void InitializeMatrixForAssembly(unsigned int N);

  /**
   * Combines the M, C and K matrices into one big system of linear 
   * equations.
   */
  virtual void FinalizeMatrixAfterAssembly( void );

  /**
   * Solves the system for the next time step.
   */
  virtual void Solve( void );

  virtual Float GetTimeStep( void ) const { return m_deltaT; }
  virtual void SetTimeStep(Float dt) { this->m_deltaT=dt; }

  /**
   * Constants that specify, where matrices are strored.
   */
  enum { matrix_K=1, matrix_M=2, matrix_C=3, matrix_tmp=4 };

  /**
   * Constants that specify, where vectors are strored.
   */
  enum { solution_d=0, solution_v=1, solution_a=2};
  enum { vector_dhat=2, vector_vhat=3, vector_ahat=4, vector_tmp=5 };

  Float m_gamma;
  Float m_beta;
  Float m_deltaT;

};




}} // end namespace itk::fem

#endif // #ifndef __itkFEMSolverHyperbolic_h
