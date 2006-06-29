/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbAssymmetricFusionOfLineDetectorImageFilter_txx
#define __otbAssymmetricFusionOfLineDetectorImageFilter_txx

#include "otbAssymmetricFusionOfLineDetectorImageFilter.h"


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class InterpolatorType>
AssymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, InterpolatorType>
::AssymmetricFusionOfLineDetectorImageFilter()
{
  this->m_LengthLine = 1;
  this->m_WidthLine = 0;

  this->m_Radius.Fill(1);
  this->m_LengthLine = 1;
  this->m_WidthLine = 0;
  
  m_LineRatio         = LineRatioType::New();
  m_LineCorrelation   = LineCorrelationType::New();
  m_AssSymSum         = AssSymSumType::New();
}

template <class TInputImage, class TOutputImage, class InterpolatorType>
void
AssymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, InterpolatorType>
::GenerateData()
{
  m_LineRatio->SetInput( this->GetInput() );
  m_LineRatio->SetLengthLine( this->m_LengthLine );
  m_LineRatio->SetWidthLine( this->m_WidthLine );

  m_LineCorrelation->SetInput( this->GetInput() );
  m_LineCorrelation->SetLengthLine( this->m_LengthLine );
  m_LineCorrelation->SetWidthLine( this->m_WidthLine );  

  m_AssSymSum->SetInput1( m_LineRatio->GetOutput() );
  m_AssSymSum->SetInput2( m_LineCorrelation->GetOutput() );
  
  m_AssSymSum->GraftOutput(this->GetOutput() ); 
  m_AssSymSum->Update();
  this->GraftOutput(m_AssSymSum->GetOutput() );
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class InterpolatorType>
void 
AssymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutput, InterpolatorType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Length: " << this->m_LengthLine << std::endl;
  os << indent << "Width: " << this->m_WidthLine << std::endl;
  
}


} // end namespace otb


#endif
