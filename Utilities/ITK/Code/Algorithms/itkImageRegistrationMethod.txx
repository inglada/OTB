/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageRegistrationMethod.txx,v $
  Language:  C++
  Date:      $Date: 2009-01-24 20:02:56 $
  Version:   $Revision: 1.31 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageRegistrationMethod_txx
#define __itkImageRegistrationMethod_txx

#include "itkImageRegistrationMethod.h"


namespace itk
{

/**
 * Constructor
 */
template < typename TFixedImage, typename TMovingImage >
ImageRegistrationMethod<TFixedImage,TMovingImage>
::ImageRegistrationMethod()
{

  this->SetNumberOfRequiredOutputs( 1 );  // for the Transform

  m_FixedImage   = 0; // has to be provided by the user.
  m_MovingImage  = 0; // has to be provided by the user.
  m_Transform    = 0; // has to be provided by the user.
  m_Interpolator = 0; // has to be provided by the user.
  m_Metric       = 0; // has to be provided by the user.
  m_Optimizer    = 0; // has to be provided by the user.


  m_InitialTransformParameters = ParametersType(1);
  m_LastTransformParameters = ParametersType(1);

  m_InitialTransformParameters.Fill( 0.0f );
  m_LastTransformParameters.Fill( 0.0f );

  m_FixedImageRegionDefined = false;


  TransformOutputPointer transformDecorator = 
                 static_cast< TransformOutputType * >( 
                                  this->MakeOutput(0).GetPointer() );

  this->ProcessObject::SetNthOutput( 0, transformDecorator.GetPointer() );

#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
  this->SetNumberOfThreads( this->GetMultiThreader()->GetNumberOfThreads() );
#else
  this->SetNumberOfThreads( 1 );
  this->GetMultiThreader()->SetNumberOfThreads( this->GetNumberOfThreads() );
#endif
}


/**
 *
 */
template < typename TFixedImage, typename TMovingImage >
unsigned long
ImageRegistrationMethod<TFixedImage,TMovingImage>
::GetMTime() const
{
  unsigned long mtime = Superclass::GetMTime();
  unsigned long m;


  // Some of the following should be removed once ivars are put in the
  // input and output lists
  
  if (m_Transform)
    {
    m = m_Transform->GetMTime();
    mtime = (m > mtime ? m : mtime);
    }

  if (m_Interpolator)
    {
    m = m_Interpolator->GetMTime();
    mtime = (m > mtime ? m : mtime);
    }

  if (m_Metric)
    {
    m = m_Metric->GetMTime();
    mtime = (m > mtime ? m : mtime);
    }

  if (m_Optimizer)
    {
    m = m_Optimizer->GetMTime();
    mtime = (m > mtime ? m : mtime);
    }

  if (m_FixedImage)
    {
    m = m_FixedImage->GetMTime();
    mtime = (m > mtime ? m : mtime);
    }

  if (m_MovingImage)
    {
    m = m_MovingImage->GetMTime();
    mtime = (m > mtime ? m : mtime);
    }

  return mtime;
  
}

/*
 * Set the initial transform parameters
 */
template < typename TFixedImage, typename TMovingImage >
void
ImageRegistrationMethod<TFixedImage,TMovingImage>
::SetInitialTransformParameters( const ParametersType & param )
{
  m_InitialTransformParameters = param;
  this->Modified();
}


/**

 * Set the region of the fixed image to be considered for registration
 */
template < typename TFixedImage, typename TMovingImage >
void
ImageRegistrationMethod<TFixedImage,TMovingImage>
::SetFixedImageRegion( const FixedImageRegionType & region )
{ 
  m_FixedImageRegion = region;
  m_FixedImageRegionDefined = true;
  this->Modified();
}


/**
 * Initialize by setting the interconnects between components. 
 */
template < typename TFixedImage, typename TMovingImage >
void
ImageRegistrationMethod<TFixedImage,TMovingImage>
::Initialize() throw (ExceptionObject)
{

  if( !m_FixedImage )
    {
    itkExceptionMacro(<<"FixedImage is not present");
    }

  if( !m_MovingImage )
    {
    itkExceptionMacro(<<"MovingImage is not present");
    }

  if ( !m_Metric )
    {
    itkExceptionMacro(<<"Metric is not present" );
    }

  if ( !m_Optimizer )
    {
    itkExceptionMacro(<<"Optimizer is not present" );
    }

  if( !m_Transform )
    {
    itkExceptionMacro(<<"Transform is not present");
    }

  //
  // Connect the transform to the Decorator.
  //
  TransformOutputType * transformOutput =  
     static_cast< TransformOutputType * >( this->ProcessObject::GetOutput(0) );

  transformOutput->Set( m_Transform.GetPointer() );

  
  if( !m_Interpolator )
    {
    itkExceptionMacro(<<"Interpolator is not present");
    }

  // Setup the metric
#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
  this->GetMultiThreader()->SetNumberOfThreads( this->GetNumberOfThreads() );
  this->m_Metric->SetNumberOfThreads( this->GetNumberOfThreads() );
#endif
  m_Metric->SetMovingImage( m_MovingImage );
  m_Metric->SetFixedImage( m_FixedImage );
  m_Metric->SetTransform( m_Transform );
  m_Metric->SetInterpolator( m_Interpolator );

  if( m_FixedImageRegionDefined )
    {
    m_Metric->SetFixedImageRegion( m_FixedImageRegion );
    }
  else
    {
    m_Metric->SetFixedImageRegion( m_FixedImage->GetBufferedRegion() );
    }

  m_Metric->Initialize();

  // Setup the optimizer
  m_Optimizer->SetCostFunction( m_Metric );

  // Validate initial transform parameters
  if ( m_InitialTransformParameters.Size() != 
       m_Transform->GetNumberOfParameters() )
    {
    itkExceptionMacro(<<"Size mismatch between initial parameters and transform." <<
      "Expected " << m_Transform->GetNumberOfParameters() << " parameters and received "
      <<  m_InitialTransformParameters.Size() << " parameters");
    }

  m_Optimizer->SetInitialPosition( m_InitialTransformParameters );

}


/**
 * Starts the Registration Process
 */
template < typename TFixedImage, typename TMovingImage >
void
ImageRegistrationMethod<TFixedImage,TMovingImage>
::StartRegistration( void )
{ 

  // StartRegistration is an old API from before
  // ImageRegistrationMethod was a subclass of ProcessObject.
  // Historically, one could call StartRegistration() instead of
  // calling Update().  However, when called directly by the user, the
  // inputs to ImageRegistrationMethod may not be up to date.  This
  // may cause an unexpected behavior.
  //
  // Since we cannot eliminate StartRegistration for backward
  // compability reasons, we check whether StartRegistration was
  // called directly or whether Update() (which in turn called 
  // StartRegistration()).
  if (!m_Updating)
    {
    this->Update();
    }
  else
    {
    ParametersType empty(1);
    empty.Fill( 0.0 );
    try
      {
      // initialize the interconnects between components
      this->Initialize();
      }
    catch( ExceptionObject& err )
      {
      m_LastTransformParameters = empty;
      
      // pass exception to caller
      throw err;
      }
  
    this->StartOptimization();
    }
}


/**
 * Starts the Optimization process
 */
template < typename TFixedImage, typename TMovingImage >
void
ImageRegistrationMethod<TFixedImage,TMovingImage>
::StartOptimization( void )
{ 

  try
    {
    // do the optimization
    m_Optimizer->StartOptimization();
    }
  catch( ExceptionObject& err )
    {
    // An error has occurred in the optimization.
    // Update the parameters
    m_LastTransformParameters = m_Optimizer->GetCurrentPosition();

    // Pass exception to caller
    throw err;
    }

  // get the results
  m_LastTransformParameters = m_Optimizer->GetCurrentPosition();
  m_Transform->SetParameters( m_LastTransformParameters );
}


/**
 * PrintSelf
 */
template < typename TFixedImage, typename TMovingImage >
void
ImageRegistrationMethod<TFixedImage,TMovingImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Metric: " << m_Metric.GetPointer() << std::endl;
  os << indent << "Optimizer: " << m_Optimizer.GetPointer() << std::endl;
  os << indent << "Transform: " << m_Transform.GetPointer() << std::endl;
  os << indent << "Interpolator: " << m_Interpolator.GetPointer() << std::endl;
  os << indent << "Fixed Image: " << m_FixedImage.GetPointer() << std::endl;
  os << indent << "Moving Image: " << m_MovingImage.GetPointer() << std::endl;
  os << indent << "Fixed Image Region Defined: " << m_FixedImageRegionDefined << std::endl;
  os << indent << "Fixed Image Region: " << m_FixedImageRegion << std::endl;
  os << indent << "Initial Transform Parameters: " << m_InitialTransformParameters << std::endl;
  os << indent << "Last    Transform Parameters: " << m_LastTransformParameters << std::endl;
}

/*
 * Generate Data
 */
template < typename TFixedImage, typename TMovingImage >
void
ImageRegistrationMethod<TFixedImage,TMovingImage>
::GenerateData()
{
  this->StartRegistration();
}


/**
 *  Get Output
 */
template < typename TFixedImage, typename TMovingImage >
const typename ImageRegistrationMethod<TFixedImage,TMovingImage>::TransformOutputType *
ImageRegistrationMethod<TFixedImage,TMovingImage>
::GetOutput() const
{
  return static_cast< const TransformOutputType * >( this->ProcessObject::GetOutput(0) );
}

template < typename TFixedImage, typename TMovingImage >
DataObject::Pointer
ImageRegistrationMethod<TFixedImage,TMovingImage>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
    case 0:
      return static_cast<DataObject*>(TransformOutputType::New().GetPointer());
      break;
    default:
      itkExceptionMacro("MakeOutput request for an output number larger than the expected number of outputs");
      return 0;
    }
}


template < typename TFixedImage, typename TMovingImage >
void 
ImageRegistrationMethod<TFixedImage,TMovingImage>
::SetFixedImage( const FixedImageType * fixedImage )
{
  itkDebugMacro("setting Fixed Image to " << fixedImage ); 

  if (this->m_FixedImage.GetPointer() != fixedImage ) 
    { 
    this->m_FixedImage = fixedImage;

    // Process object is not const-correct so the const_cast is required here
    this->ProcessObject::SetNthInput(0, 
                                   const_cast< FixedImageType *>( fixedImage ) );
    
    this->Modified(); 
    } 
}

template < typename TFixedImage, typename TMovingImage >
void 
ImageRegistrationMethod<TFixedImage,TMovingImage>
::SetMovingImage( const MovingImageType * movingImage )
{
  itkDebugMacro("setting Moving Image to " << movingImage ); 

  if (this->m_MovingImage.GetPointer() != movingImage ) 
    { 
    this->m_MovingImage = movingImage;

    // Process object is not const-correct so the const_cast is required here
    this->ProcessObject::SetNthInput(1, 
                                   const_cast< MovingImageType *>( movingImage ) );
    
    this->Modified(); 
    } 
}

} // end namespace itk


#endif
