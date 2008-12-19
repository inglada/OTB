/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorCurvatureAnisotropicDiffusionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-17 16:30:54 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorCurvatureAnisotropicDiffusionImageFilter_h
#define __itkVectorCurvatureAnisotropicDiffusionImageFilter_h

#include "itkExceptionObject.h"
#include "itkAnisotropicDiffusionImageFilter.h"
#include "itkVectorCurvatureNDAnisotropicDiffusionFunction.h"

namespace itk {

/** \class VectorCurvatureAnisotropicDiffusionImageFilter
 *
 * This filter performs anisotropic diffusion on a vector itk::Image using the
 * modified curvature diffusion equation (MCDE) implemented in
 * itkVectorCurvatureNDAnisotropicDiffusionFunction.  For detailed information on
 * anisotropic diffusion and the MCDE see itkAnisotropicDiffusionFunction,
 * itkVectorCurvatureNDAnisotropicDiffusionFunction, and
 * itkCurvatureNDAnisotropicDiffusionFunction.
 *
 * \par Inputs and Outputs
 * The input to this filter must be an itk::Image with pixel
 * type which is either an itk::Vector, or a subclass of an itk::Vector.
 * Additionally, the component type of the vector should be a numerical type
 * (float or double, or a user defined type which correctly defines
 * arithmetic operations with floating point accuracy).  The output image type
 * also has these requirements.
 *
 * \par Parameters
 * Please first read all the documentation found in
 * AnisotropicDiffusionImageFilter and AnisotropicDiffusionFunction.  Also see
 * VectorCurvatureNDAnisotropicDiffusionFunction.
 *
 * The default time step for this filter is set to the maximum theoretically
 * stable value: 0.5 / 2^N, where N is the dimensionality of the image.  For a
 * 2D image, this means valid time steps are below 0.1250.  For a 3D image,
 * valid time steps are below 0.0625.
 *
 * \sa AnisotropicDiffusionImageFilter
 * \sa AnisotropicDiffusionFunction
 * \sa CurvatureNDAnisotropicDiffusionFunction
 
 * \ingroup ImageEnhancement
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT VectorCurvatureAnisotropicDiffusionImageFilter
  : public AnisotropicDiffusionImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard itk typedefs */
  typedef VectorCurvatureAnisotropicDiffusionImageFilter Self;
  typedef AnisotropicDiffusionImageFilter<TInputImage, TOutputImage>
                                                         Superclass;
  typedef SmartPointer<Self>                             Pointer;
  typedef SmartPointer<const Self>                       ConstPointer;

  /** Instantiation through object factory. */
  itkNewMacro(Self);

  /** Run-time type information. */
  itkTypeMacro(VectorCurvatureAnisotropicDiffusionImageFilter,
               AnisotropicDiffusionImageFilter);
  
  /** Convenient typedef. */
  typedef typename Superclass::UpdateBufferType UpdateBufferType;

  /** Determine the image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      Superclass::ImageDimension );

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TInputImage::PixelType::ValueType>));
  itkConceptMacro(OutputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TOutputImage::PixelType::ValueType>));
  /** End concept checking */
#endif

protected:
  VectorCurvatureAnisotropicDiffusionImageFilter()
    {
    typename VectorCurvatureNDAnisotropicDiffusionFunction<UpdateBufferType>::Pointer q
      = VectorCurvatureNDAnisotropicDiffusionFunction<UpdateBufferType>::New();
    this->SetDifferenceFunction(q);
    }
  ~VectorCurvatureAnisotropicDiffusionImageFilter() {}

  virtual void InitializeIteration()
    {
    Superclass::InitializeIteration();
    if (this->GetTimeStep() >  0.5 / vcl_pow(2.0, static_cast<double>(ImageDimension))  )
      {
      itkWarningMacro(<< "Anisotropic diffusion has attempted to use a time step which may introduce instability into the solution." );
      }
    }
  
private:
  VectorCurvatureAnisotropicDiffusionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namspace itk

#endif
