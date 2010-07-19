/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMaximumDecisionRule.h,v $
  Language:  C++
  Date:      $Date: 2009-02-05 22:04:03 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMaximumDecisionRule_h
#define __itkMaximumDecisionRule_h

#include "itkWin32Header.h"

#include "itkDecisionRuleBase.h"

namespace itk { 

/** \class MaximumDecisionRule
 *  \brief A Decision rule that choose the class of which discriminant
 *  score is the largest.
 */
 
class ITKCommon_EXPORT MaximumDecisionRule : 
    public DecisionRuleBase
{
public:
  /** Standard class typedefs */ 
  typedef MaximumDecisionRule      Self;
  typedef DecisionRuleBase         Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(MaximumDecisionRule, DecisionRuleBase);

  /** Standard New() method support */
  itkNewMacro(Self);

  /** Types for the arguments that are acceptable in the Evaluate() method */
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::ArrayType                ArrayType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
 

  /** The return value of this function is a class label.
   * Basically, using its internal logic based on the discriminant
   * scores, this function decides best class label and return it.
   */
  virtual unsigned int Evaluate( const VectorType &discriminantScores) const;
  virtual unsigned int Evaluate( const ArrayType &discriminantScores) const;
  virtual unsigned int Evaluate( const VariableLengthVectorType &discriminantScores) const;


protected:
  MaximumDecisionRule() {}
  virtual ~MaximumDecisionRule() {}
  void PrintSelf(std::ostream& os, Indent indent) const;
}; // end of class

} // end of namespace itk

#endif
