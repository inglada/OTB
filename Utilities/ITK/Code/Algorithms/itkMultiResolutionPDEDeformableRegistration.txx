/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultiResolutionPDEDeformableRegistration.txx,v $
  Language:  C++
  Date:      $Date: 2007-07-31 23:12:20 $
  Version:   $Revision: 1.28 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkMultiResolutionPDEDeformableRegistration_txx
#define _itkMultiResolutionPDEDeformableRegistration_txx
#include "itkMultiResolutionPDEDeformableRegistration.h"

#include "itkRecursiveMultiResolutionPyramidImageFilter.h"
#include "itkImageRegionIterator.h"
#include "vnl/vnl_math.h"

namespace itk {

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::MultiResolutionPDEDeformableRegistration()
{
 
  this->SetNumberOfRequiredInputs(2);

  typename DefaultRegistrationType::Pointer registrator =
    DefaultRegistrationType::New();
  m_RegistrationFilter = static_cast<RegistrationType*>(
    registrator.GetPointer() );

  m_MovingImagePyramid  = MovingImagePyramidType::New();
  m_FixedImagePyramid     = FixedImagePyramidType::New();
  m_FieldExpander     = FieldExpanderType::New();
  m_InitialDeformationField = NULL;

  m_NumberOfLevels = 3;
  m_NumberOfIterations.resize( m_NumberOfLevels );
  m_FixedImagePyramid->SetNumberOfLevels( m_NumberOfLevels );
  m_MovingImagePyramid->SetNumberOfLevels( m_NumberOfLevels );

  unsigned int ilevel;
  for( ilevel = 0; ilevel < m_NumberOfLevels; ilevel++ )
    {
    m_NumberOfIterations[ilevel] = 10;
    }
  m_CurrentLevel = 0;

  m_StopRegistrationFlag = false;

}


/*
 * Set the moving image image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::SetMovingImage(
const MovingImageType * ptr )
{
  this->ProcessObject::SetNthInput( 2, const_cast< MovingImageType * >( ptr ) );
}


/*
 * Get the moving image image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
const typename MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::MovingImageType *
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::GetMovingImage(void) const
{
  return dynamic_cast< const MovingImageType * >
    ( this->ProcessObject::GetInput( 2 ) );
}


/*
 * Set the fixed image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::SetFixedImage(
const FixedImageType * ptr )
{
  this->ProcessObject::SetNthInput( 1, const_cast< FixedImageType * >( ptr ) );
}


/*
 * Get the fixed image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
const typename MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::FixedImageType *
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::GetFixedImage(void) const
{
  return dynamic_cast< const FixedImageType * >
    ( this->ProcessObject::GetInput( 1 ) );
}

/*
 * 
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
std::vector<SmartPointer<DataObject> >::size_type
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::GetNumberOfValidRequiredInputs() const
{
  typename std::vector<SmartPointer<DataObject> >::size_type num = 0;

  if (this->GetFixedImage())
    {
    num++;
    }

  if (this->GetMovingImage())
    {
    num++;
    }
  
  return num;
}


/*
 * Set the number of multi-resolution levels
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::SetNumberOfLevels(
unsigned int num )
{
  if( m_NumberOfLevels != num )
    {
    this->Modified();
    m_NumberOfLevels = num;
    m_NumberOfIterations.resize( m_NumberOfLevels );
    }

  if( m_MovingImagePyramid && m_MovingImagePyramid->GetNumberOfLevels() != num )
    {
    m_MovingImagePyramid->SetNumberOfLevels( m_NumberOfLevels );
    }
  if( m_FixedImagePyramid && m_FixedImagePyramid->GetNumberOfLevels() != num )
    {
    m_FixedImagePyramid->SetNumberOfLevels( m_NumberOfLevels );
    }  
    
}


/*
 * Standard PrintSelf method.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "NumberOfLevels: " << m_NumberOfLevels << std::endl;
  os << indent << "CurrentLevel: " << m_CurrentLevel << std::endl;

  os << indent << "NumberOfIterations: [";
  unsigned int ilevel;
  for( ilevel = 0; ilevel < m_NumberOfLevels - 1; ilevel++ )
    {
    os << m_NumberOfIterations[ilevel] << ", ";
    }
  os << m_NumberOfIterations[ilevel] << "]" << std::endl;
  
  os << indent << "RegistrationFilter: ";
  os << m_RegistrationFilter.GetPointer() << std::endl;
  os << indent << "MovingImagePyramid: ";
  os << m_MovingImagePyramid.GetPointer() << std::endl;
  os << indent << "FixedImagePyramid: ";
  os << m_FixedImagePyramid.GetPointer() << std::endl;

  os << indent << "StopRegistrationFlag: ";
  os << m_StopRegistrationFlag << std::endl;

}

/*
 * Perform a the deformable registration using a multiresolution scheme
 * using an internal mini-pipeline
 *
 *  ref_pyramid ->  registrator  ->  field_expander --|| tempField
 * test_pyramid ->           |                              |
 *                           |                              |
 *                           --------------------------------    
 *
 * A tempField image is used to break the cycle between the
 * registrator and field_expander.
 *
 */                              
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::GenerateData()
{
  //DeformationFieldPointer NullDeformationFieldPointer = DeformationFieldType::New();
  
  // Check for NULL images and pointers
  MovingImageConstPointer movingImage = this->GetMovingImage();
  FixedImageConstPointer  fixedImage = this->GetFixedImage();

  if( !movingImage || !fixedImage )
    {
    itkExceptionMacro( << "Fixed and/or moving image not set" );
    }

  if( !m_MovingImagePyramid || !m_FixedImagePyramid )
    {
    itkExceptionMacro( << "Fixed and/or moving pyramid not set" );
    }

  if( !m_RegistrationFilter )
    {
    itkExceptionMacro( << "Registration filter not set" );
    }
  
  // Create the image pyramids.
  m_MovingImagePyramid->SetInput( movingImage );
  m_MovingImagePyramid->UpdateLargestPossibleRegion();

  m_FixedImagePyramid->SetInput( fixedImage );
  m_FixedImagePyramid->UpdateLargestPossibleRegion();
 
  // Initializations
  m_CurrentLevel = 0;
  m_StopRegistrationFlag = false;

  unsigned int movingLevel = vnl_math_min( (int) m_CurrentLevel, 
    (int) m_MovingImagePyramid->GetNumberOfLevels() );

  unsigned int fixedLevel = vnl_math_min( (int) m_CurrentLevel, 
    (int) m_FixedImagePyramid->GetNumberOfLevels() );

  DeformationFieldPointer tempField = this->m_InitialDeformationField;
  bool lastShrinkFactorsAllOnes = false;

  while ( !this->Halt() )
    {
   
    if( tempField.IsNull() )
      {
      m_RegistrationFilter->SetInitialDeformationField( NULL );
      }
    else
      {
      // Resample the field to be the same size as the fixed image
      // at the current level
      m_FieldExpander->SetInput( tempField );
      
      typename FloatImageType::Pointer fi = 
        m_FixedImagePyramid->GetOutput( fixedLevel );
      m_FieldExpander->SetSize( 
        fi->GetLargestPossibleRegion().GetSize() );
      m_FieldExpander->SetOutputStartIndex(
        fi->GetLargestPossibleRegion().GetIndex() );
      m_FieldExpander->SetOutputOrigin( fi->GetOrigin() );
      m_FieldExpander->SetOutputSpacing( fi->GetSpacing());

      m_FieldExpander->UpdateLargestPossibleRegion();
      m_FieldExpander->SetInput( NULL );
      tempField = m_FieldExpander->GetOutput();
      tempField->DisconnectPipeline();

      m_RegistrationFilter->SetInitialDeformationField( tempField );

      }

    // setup registration filter and pyramids 
    m_RegistrationFilter->SetMovingImage( m_MovingImagePyramid->GetOutput(movingLevel) );
    m_RegistrationFilter->SetFixedImage( m_FixedImagePyramid->GetOutput(fixedLevel) );

    m_RegistrationFilter->SetNumberOfIterations(
      m_NumberOfIterations[m_CurrentLevel] );

    // cache shrink factors for computing the next expand factors.
    lastShrinkFactorsAllOnes = true;
    for( unsigned int idim = 0; idim < ImageDimension; idim++ )
      {
      if ( m_FixedImagePyramid->GetSchedule()[fixedLevel][idim] > 1 )
        {
        lastShrinkFactorsAllOnes = false;
        break;
        }
      }

    // compute new deformation field
    m_RegistrationFilter->UpdateLargestPossibleRegion();
    tempField = m_RegistrationFilter->GetOutput();
    tempField->DisconnectPipeline();

    // Increment level counter.  
    m_CurrentLevel++;
    movingLevel = vnl_math_min( (int) m_CurrentLevel, 
      (int) m_MovingImagePyramid->GetNumberOfLevels() );
    fixedLevel = vnl_math_min( (int) m_CurrentLevel, 
      (int) m_FixedImagePyramid->GetNumberOfLevels() );

    // Invoke an iteration event.
    this->InvokeEvent( IterationEvent() );

    // We can release data from pyramid which are no longer required.
    if ( movingLevel > 0 )
      {
      m_MovingImagePyramid->GetOutput( movingLevel - 1 )->ReleaseData();
      }
    if( fixedLevel > 0 )
      {
      m_FixedImagePyramid->GetOutput( fixedLevel - 1 )->ReleaseData();
      }

    } // while not Halt()

    if( !lastShrinkFactorsAllOnes )
      {
      // Some of the last shrink factors are not one
      // graft the output of the expander filter to
      // to output of this filter

      // resample the field to the same size as the fixed image
      m_FieldExpander->SetInput( tempField );
      m_FieldExpander->SetSize( 
        fixedImage->GetLargestPossibleRegion().GetSize() );
      m_FieldExpander->SetOutputStartIndex(
        fixedImage->GetLargestPossibleRegion().GetIndex() );
      m_FieldExpander->SetOutputOrigin( fixedImage->GetOrigin() );
      m_FieldExpander->SetOutputSpacing( fixedImage->GetSpacing());

      m_FieldExpander->UpdateLargestPossibleRegion();
      this->GraftOutput( m_FieldExpander->GetOutput() );
      }
    else
      {
      // all the last shrink factors are all ones
      // graft the output of registration filter to
      // to output of this filter
      this->GraftOutput( tempField );
      }

    // Release memory
    m_FieldExpander->SetInput( NULL );
    m_FieldExpander->GetOutput()->ReleaseData();
    m_RegistrationFilter->SetInput( NULL );
    m_RegistrationFilter->GetOutput()->ReleaseData();

}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::StopRegistration()
{
  m_RegistrationFilter->StopRegistration();
  m_StopRegistrationFlag = true;
}

template <class TFixedImage, class TMovingImage, class TDeformationField>
bool
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::Halt()
{
  // Halt the registration after the user-specified number of levels
  if (m_NumberOfLevels != 0)
  {
  this->UpdateProgress( static_cast<float>( m_CurrentLevel ) /
                        static_cast<float>( m_NumberOfLevels ) );
  }

  if ( m_CurrentLevel >= m_NumberOfLevels )
    {
    return true;
    }
  if ( m_StopRegistrationFlag )
    {
    return true;
    }
  else
    { 
    return false; 
    }

}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::GenerateOutputInformation()
{

 typename DataObject::Pointer output;

 if( this->GetInput(0) )
  {
  // Initial deformation field is set.
  // Copy information from initial field.
  this->Superclass::GenerateOutputInformation();

  }
 else if( this->GetFixedImage() )
  {
  // Initial deforamtion field is not set. 
  // Copy information from the fixed image.
  for (unsigned int idx = 0; idx < 
    this->GetNumberOfOutputs(); ++idx )
    {
    output = this->GetOutput(idx);
    if (output)
      {
      output->CopyInformation(this->GetFixedImage());
      }  
    }

  }

}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::GenerateInputRequestedRegion()
{

  // call the superclass's implementation
  Superclass::GenerateInputRequestedRegion();

  // request the largest possible region for the moving image
  MovingImagePointer movingPtr = 
    const_cast< MovingImageType * >( this->GetMovingImage() );
  if( movingPtr )
    {
    movingPtr->SetRequestedRegionToLargestPossibleRegion();
    }
  
  // just propagate up the output requested region for
  // the fixed image and initial deformation field.
  DeformationFieldPointer inputPtr = 
      const_cast< DeformationFieldType * >( this->GetInput() );
  DeformationFieldPointer outputPtr = this->GetOutput();
  FixedImagePointer fixedPtr = 
        const_cast< FixedImageType *>( this->GetFixedImage() );

  if( inputPtr )
    {
    inputPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
    }

  if( fixedPtr )
    {
    fixedPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
    }

}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
MultiResolutionPDEDeformableRegistration<TFixedImage,TMovingImage,TDeformationField>
::EnlargeOutputRequestedRegion(
DataObject * ptr )
{
  // call the superclass's implementation
  Superclass::EnlargeOutputRequestedRegion( ptr );

  // set the output requested region to largest possible.
  DeformationFieldType * outputPtr;
  outputPtr = dynamic_cast<DeformationFieldType*>( ptr );

  if( outputPtr )
    {
    outputPtr->SetRequestedRegionToLargestPossibleRegion();
    }

}


} // end namespace itk

#endif
