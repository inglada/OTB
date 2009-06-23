/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBSplineResampleImageFunction.h,v $
  Language:  C++
  Date:      $Date: 2008-10-09 20:43:43 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBSplineResampleImageFunction_h
#define __itkBSplineResampleImageFunction_h

#include "itkBSplineInterpolateImageFunction.h"

namespace itk
{
/** \class BSplineResampleImageFunction
 * \brief Resample image intensity from a BSpline coefficient image.
 *
 * This class resample the image intensity at a non-integer position
 * from the input BSpline coefficient image.
 * 
 * Spline order may be from 0 to 5.
 *
 * In ITK, BSpline coefficient can be generated using a
 * BSplineDecompositionImageFilter. Using this image function in
 * conjunction with ResampleImageFunction allows the reconstruction
 * of the original image at different resolution and size.
 *
 * \sa BSplineInterpolateImageFunction
 * \sa BSplineDecompositionImageFilter
 * \sa ResampleImageFilter
 *
 * \ingroup ImageFunctions
 */
template <class TImageType, class TCoordRep = float>
class ITK_EXPORT BSplineResampleImageFunction : 
    public BSplineInterpolateImageFunction<
  TImageType,TCoordRep,ITK_TYPENAME TImageType::PixelType > 
{
public:
  /** Standard class typedefs. */
  typedef BSplineResampleImageFunction                          Self;
  typedef BSplineInterpolateImageFunction<
    TImageType,TCoordRep, ITK_TYPENAME TImageType::PixelType >  Superclass;
  typedef SmartPointer<Self>                                    Pointer;
  typedef SmartPointer<const Self>                              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSplineResampleImageFunction, 
               BSplineInterpolateImageFunction);

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro( Self );

  /** Set the input image representing the BSplineCoefficients */
  virtual void SetInputImage(const TImageType * inputData)
    {
    // bypass my superclass
    this->InterpolateImageFunction<TImageType,TCoordRep>::SetInputImage(inputData);
    this->m_Coefficients = inputData;
    if ( this->m_Coefficients.IsNotNull() )
      {
      this->m_DataLength = this->m_Coefficients->GetBufferedRegion().GetSize();
      }
    }

protected:
  BSplineResampleImageFunction() {};
  virtual ~BSplineResampleImageFunction() {};

private:
  BSplineResampleImageFunction(const Self&);//purposely not implemented
};

} // namespace itk


#endif
