/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimumDecisionRule2.h,v $
  Language:  C++
  Date:      $Date: 2009-05-02 05:43:58 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMinimumDecisionRule2_h
#define __itkMinimumDecisionRule2_h

#include "itkDecisionRule.h"

namespace itk { 
namespace Statistics { 

/** \class MinimumDecisionRule2
 *  \brief A Decision rule that choose the class of which discriminant
 *  score is the largest. This class will replace the MinimumDecisionRule
 *  in Code/Common
 */
 
class ITK_EXPORT MinimumDecisionRule2 : public DecisionRule
{
public:
  /** Standard class typedefs */ 
  typedef MinimumDecisionRule2            Self;
  typedef DecisionRule                    Superclass;
  typedef SmartPointer<Self>              Pointer;
  typedef SmartPointer<const Self>        ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(MinimumDecisionRule2, DecisionRuleBase);

  /** Standard New() method support */
  itkNewMacro(Self);

  typedef Superclass::MembershipVectorType  MembershipVectorType;
 

  /** The return value of this function is a class label.
   * Basically, using its internal logic based on the discriminant
   * scores, this function decides best class label and return it.
   */
  virtual unsigned int Evaluate( const MembershipVectorType &discriminantScores) const;


protected:
  MinimumDecisionRule2() {}
  virtual ~MinimumDecisionRule2() {}
  void PrintSelf(std::ostream& os, Indent indent) const;
}; // end of class

} // end of namespace itk
} // end of namespace Statistics

#endif
