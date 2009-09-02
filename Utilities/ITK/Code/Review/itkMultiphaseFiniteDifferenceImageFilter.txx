/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMultiphaseFiniteDifferenceImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-20 22:03:45 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkMultiphaseFiniteDifferenceImageFilter_txx
#define __itkMultiphaseFiniteDifferenceImageFilter_txx

#include "itkMultiphaseFiniteDifferenceImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkExceptionObject.h"
#include "itkEventObject.h"

namespace itk {

template < class TInputImage,
  class TOutputImage,
  class TFiniteDifferenceFunction,
  typename TIdCell >
void
MultiphaseFiniteDifferenceImageFilter< TInputImage,
  TOutputImage,
  TFiniteDifferenceFunction,
  TIdCell >
::GenerateData()
{
  if( ! this->m_FunctionCount )
    {
    itkExceptionMacro("Number of level set functions not specified. "
      << "Please set using SetFunctionCount()" );
    }

  if( ! this->m_InitializedState )
    {
    // Set the coefficients for the deriviatives
    double coeffs[ImageDimension];
    unsigned int i;
    if (m_UseImageSpacing)
      {
      for (i = 0; i < ImageDimension; i++)
        {
        coeffs[i] = 1.0 / m_LevelSet[0]->GetSpacing()[i];
        }
      }
    else
      {
      for (i = 0; i < ImageDimension; i++)
        {
        coeffs[i] = 1.0;
        }
      }

    for( IdCellType id = 0; id <  this->m_FunctionCount; id++ )
      {
      this->m_DifferenceFunctions[id]->SetScaleCoefficients(coeffs);
      }

    // Allocate the output image -- inherited
    this->AllocateOutputs();

    // Copy the input image to the output image.  Algorithms will operate
    // directly on the output image and the update buffer.
    this->CopyInputToOutput();

    // Perform any other necessary pre-iteration initialization.
    this->Initialize();

    // Allocate the internal update buffer.  This takes place entirely within
    // the subclass, since this class cannot define an update buffer type.
    this->AllocateUpdateBuffer();

    this->SetInitializedState(true);

    this->m_ElapsedIterations = 0;
    }

  // Iterative algorithm
  TimeStepType dt;

  while ( ! this->Halt() )
    {
    /* An optional method for precalculating global values, or setting
    up for the next iteration*/
    this->InitializeIteration();

    dt = this->CalculateChange();

    this->ApplyUpdate( dt );
    this->m_ElapsedIterations++;

    // Invoke the iteration event.
    this->InvokeEvent( IterationEvent() );

    if( this->GetAbortGenerateData() )
      {
      this->InvokeEvent( IterationEvent() );
      this->ResetPipeline();
      throw ProcessAborted(__FILE__,__LINE__);
      }
    }

  // Reset the state once execution is completed
  if( !this->m_ManualReinitialization )
    {
    this->SetInitializedState(true);
    }

  // Any further processing of the solution can be done here.
  this->PostProcessOutput();
}

/**
 *
 */
template < class TInputImage,
  class TOutputImage,
  class TFiniteDifferenceFunction,
  typename TIdCell >
void
MultiphaseFiniteDifferenceImageFilter< TInputImage,
  TOutputImage,
  TFiniteDifferenceFunction,
  TIdCell >
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  // copy the output requested region to the input requested region
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input
  InputImagePointer  inputPtr  =
    const_cast< InputImageType* >( this->GetInput(0) );

  if ( inputPtr.IsNull() )
    {
    return;
    }

  if( this->m_DifferenceFunctions[0].IsNull() )
    {
    return;
    }

  // Get the size of the neighborhood on which we are going to operate.  This
  // radius is supplied by the difference function we are using.
  RadiusType radius = this->m_DifferenceFunctions[0]->GetRadius();

  // Try to set up a buffered region that will accommodate our
  // neighborhood operations.  This may not be possible and we
  // need to be careful not to request a region outside the largest
  // possible region, because the pipeline will give us whatever we
  // ask for.

  // get a copy of the input requested region (should equal the output
  // requested region)
  InputRegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( radius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop( inputPtr->GetLargestPossibleRegion() ) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );

    // build an exception
    InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the "
      "largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template < class TInputImage,
  class TOutputImage,
  class TFiniteDifferenceFunction,
  typename TIdCell >
typename MultiphaseFiniteDifferenceImageFilter< TInputImage,
  TOutputImage, TFiniteDifferenceFunction, TIdCell >::TimeStepType
MultiphaseFiniteDifferenceImageFilter< TInputImage,
  TOutputImage,
  TFiniteDifferenceFunction,
  TIdCell >
::ResolveTimeStep( const TimeStepVectorType &timeStepList,
  const std::vector< bool >& valid )
{
  TimeStepType oMin = NumericTraits<TimeStepType>::Zero;
  size_t size = timeStepList.size();

  if( size == valid.size() )
    {
    bool flag = false;
    size_t k = 0;
    size_t i;

    for( i = 0; i < size; ++i)
      {
      if (valid[i])
        {
        oMin = timeStepList[i];
        k = i;
        flag = true;
        break;
        }
      }

    if( !flag )
      {
      itkExceptionMacro("No Values");
      }

    // find minimum value
    for ( i = k; i < size; ++i)
      {
      if ( valid[i] && (timeStepList[i] < oMin) )
        {
        oMin = timeStepList[i];
        }
      }
   }

   return oMin;
}

template < class TInputImage,
  class TOutputImage,
  class TFiniteDifferenceFunction,
  typename TIdCell >
bool
MultiphaseFiniteDifferenceImageFilter< TInputImage,
  TOutputImage,
  TFiniteDifferenceFunction,
  TIdCell >
::Halt()
{
  float progress = 0.;

  if( this->m_NumberOfIterations != 0 )
    {
    progress = static_cast<float>( this->GetElapsedIterations() )
      / static_cast<float>( this->m_NumberOfIterations );
    }
  this->UpdateProgress( progress );

  return ( (this->GetElapsedIterations() >= this->m_NumberOfIterations) ||
      ( this->GetMaximumRMSError() >= m_RMSChange ) );
}


template < class TInputImage,
  class TOutputImage,
  class TFiniteDifferenceFunction,
  typename TIdCell >
void
MultiphaseFiniteDifferenceImageFilter< TInputImage,
  TOutputImage,
  TFiniteDifferenceFunction,
  TIdCell >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ElapsedIterations: " << this->m_ElapsedIterations << std::endl;
  os << indent << "UseImageSpacing: " << (m_UseImageSpacing ? "On" : "Off") << std::endl;
  os << indent << "State: " <<  this->m_InitializedState << std::endl;
  os << indent << "MaximumRMSError: " << m_MaximumRMSError << std::endl;
  os << indent << "NumberOfIterations: " << this->m_NumberOfIterations << std::endl;
  os << indent << "ManualReinitialization: " << this->m_ManualReinitialization << std::endl;
  os << indent << "RMSChange: " << m_RMSChange << std::endl;
  os << std::endl;

  if (  this->m_FunctionCount )
    {
    if( this->m_DifferenceFunctions[0] )
      {
      os << indent << "DifferenceFunction: " << std::endl;
      for( IdCellType i = 0; i <  this->m_FunctionCount; ++i )
        {
        this->m_DifferenceFunctions[i]->Print(os,indent.GetNextIndent());
        }
      }
    }
  else
    {
    os << indent << "DifferenceFunction: " << "(None)" << std::endl;
    }

  os << std::endl;
}


}// end namespace itk

#endif
