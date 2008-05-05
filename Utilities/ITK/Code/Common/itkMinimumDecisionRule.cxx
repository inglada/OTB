/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimumDecisionRule.cxx,v $
  Language:  C++
  Date:      $Date: 2005-07-15 21:51:01 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMinimumDecisionRule.h"

namespace itk
{

MinimumDecisionRule
::MinimumDecisionRule() 
{

}

unsigned int 
MinimumDecisionRule
::Evaluate( const VectorType &discriminantScores) const
{

  double       minimumDistance      = discriminantScores[0];
  unsigned int classifiedPixelIndex = 0;
  unsigned int numberOfClasses      = static_cast<unsigned int>( discriminantScores.size() );

  //Loop through the probabilities to get the best index
  for(unsigned int classIndex = 1; classIndex < numberOfClasses; classIndex++ )
    {  
    if( discriminantScores[classIndex] < minimumDistance ) 
      {
      minimumDistance      = discriminantScores[classIndex];
      classifiedPixelIndex = classIndex;
      }
    }// end for

  return classifiedPixelIndex;
}


unsigned int 
MinimumDecisionRule
::Evaluate( const ArrayType &discriminantScores) const
{

  double       minimumDistance      = discriminantScores[0];
  unsigned int classifiedPixelIndex = 0;
  unsigned int numberOfClasses      = static_cast<unsigned int>( discriminantScores.Size() );

  //Loop through the probabilities to get the best index
  for(unsigned int classIndex = 1; classIndex < numberOfClasses; classIndex++ )
    {  
    if( discriminantScores[classIndex] < minimumDistance ) 
      {
      minimumDistance      = discriminantScores[classIndex];
      classifiedPixelIndex = classIndex;
      }
    }// end for

  return classifiedPixelIndex;
}


} // namespace itk








