/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageClassifierFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-22 17:44:20 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageClassifierFilter_txx
#define __itkImageClassifierFilter_txx

#include "itkImageClassifierFilter.h"

namespace itk {
namespace Statistics {

template< class TSample, class TInputImage, class TOutputImage >
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::ImageClassifierFilter()
{
  this->m_NumberOfClasses = 0;
  this->SetNumberOfRequiredInputs( 3 );
  this->SetNumberOfRequiredOutputs( 1 );

  /** Initialize decision rule */
  m_DecisionRule = NULL;

  m_NumberOfClasses = 0;
}

template< class TSample, class TInputImage, class TOutputImage >
void
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Number of classes: " 
     << this->GetNumberOfClasses()
     << std::endl;
  os << indent << "Decision Rule: " 
     << this->GetDecisionRule()
     << std::endl;
  os << indent << "Image: " 
     << this->GetImage()
     << std::endl;
}

template< class TSample, class TInputImage, class TOutputImage >
void
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::SetImage( const InputImageType * image )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0,
                                   const_cast< InputImageType * >( image ) );
}

template< class TSample, class TInputImage, class TOutputImage >
const TInputImage *
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::GetImage( ) const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage * >
  (this->ProcessObject::GetInput(0) );
}


template< class TSample, class TInputImage, class TOutputImage >
void
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::SetClassLabels( const ClassLabelVectorObjectType * classLabels )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(1,
                                   const_cast< ClassLabelVectorObjectType * >( classLabels ) );
}

template< class TSample, class TInputImage, class TOutputImage >
void
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::SetMembershipFunctions( const MembershipFunctionVectorObjectType * membershipFunctions )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(2,
                                   const_cast< MembershipFunctionVectorObjectType * >( membershipFunctions ) );
}

template< class TSample, class TInputImage, class TOutputImage >
void
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::SetMembershipFunctionsWeightsArray( const
MembershipFunctionsWeightsArrayObjectType * weightsArray )
{
 // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(3,
                                   const_cast<
  MembershipFunctionsWeightsArrayObjectType * >( weightsArray ) );
}

template< class TSample, class TInputImage, class TOutputImage >
void
ImageClassifierFilter<TSample,TInputImage,TOutputImage>
::GenerateData()
{
  const ClassLabelVectorObjectType * classLabelsDecorated = 
    static_cast< const ClassLabelVectorObjectType * >( this->ProcessObject::GetInput( 1 ) );

  const MembershipFunctionVectorObjectType * membershipFunctionsDecorated = 
    static_cast< const MembershipFunctionVectorObjectType * >( this->ProcessObject::GetInput( 2 ) );

  const MembershipFunctionsWeightsArrayObjectType *
                membershipFunctionsWeightsArrayDecorated = 
    static_cast< const MembershipFunctionsWeightsArrayObjectType * >( this->ProcessObject::GetInput( 3 ) );

  const ClassLabelVectorType & classLabels = classLabelsDecorated->Get();

  const MembershipFunctionVectorType & membershipFunctions = membershipFunctionsDecorated->Get();

  // Check number of Labels and MembershipSamples against the number of classes */
  if( membershipFunctions.size() != this->m_NumberOfClasses )
    {
    itkExceptionMacro("Number of Membership functions does not match the number of classes");
    }

  if( classLabels.size() != this->m_NumberOfClasses )
    {
    itkExceptionMacro("Number of class labels does not match the number of classes");
    }

  if( m_DecisionRule.IsNull())
    {
    itkExceptionMacro("Decision rule is not set");
    }

  MembershipFunctionsWeightsArrayType membershipFunctionsWeightsArray;
  if( membershipFunctionsWeightsArrayDecorated == NULL )
    {
    // no weights array is set and hence all membership functions will have equal
    // weight
    membershipFunctionsWeightsArray.SetSize( this->m_NumberOfClasses );
    membershipFunctionsWeightsArray.Fill(1.0);
    }
  else
    {
    membershipFunctionsWeightsArray = membershipFunctionsWeightsArrayDecorated->Get();
    }

  if ( membershipFunctionsWeightsArray.Size() != this->m_NumberOfClasses
)
    {
    itkExceptionMacro("Membership functions weight array size does not match the\
                      number of classes "); 
    }
  
  const InputImageType * inputImage =
    static_cast< const InputImageType * >( this->ProcessObject::GetInput( 0 ) );

  std::vector< double > discriminantScores;
  discriminantScores.resize( this->m_NumberOfClasses );

  OutputImageType * outputImage = dynamic_cast< OutputImageType * >(
                      this->ProcessObject::GetOutput(0)); 

  outputImage->CopyInformation( inputImage );
  outputImage->SetRegions( inputImage->GetBufferedRegion() );
  outputImage->Allocate();

  ImageRegionConstIterator< InputImageType >   inpItr( inputImage, inputImage->GetBufferedRegion() );
  ImageRegionIterator< OutputImageType >       outItr( outputImage, outputImage->GetBufferedRegion() );

  inpItr.GoToBegin();
  outItr.GoToBegin();

  while( !inpItr.IsAtEnd() )
    {
    MeasurementVectorType measurements;

    MeasurementVectorTraits::Assign( measurements, inpItr.Get() ); 

    for (unsigned int i = 0; i < this->m_NumberOfClasses; i++)
      {
      discriminantScores[i] = membershipFunctionsWeightsArray[i] * 
        membershipFunctions[i]->Evaluate(measurements); 
      }

    unsigned int classIndex;
    classIndex = m_DecisionRule->Evaluate(discriminantScores);

    OutputPixelType value = static_cast< OutputPixelType >(classLabels[classIndex]);
    outItr.Set(value);

    ++inpItr;
    ++outItr;
    }
}

} // end of namespace Statistics
} // end of namespace itk

#endif
