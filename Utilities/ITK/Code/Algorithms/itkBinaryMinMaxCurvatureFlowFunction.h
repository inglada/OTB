/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryMinMaxCurvatureFlowFunction.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:28 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryMinMaxCurvatureFlowFunction_h_
#define __itkBinaryMinMaxCurvatureFlowFunction_h_

#include "itkMinMaxCurvatureFlowFunction.h"
#include "itkMacro.h"
#include "itkNeighborhoodOperator.h"

namespace itk {

/** \class BinaryMinMaxCurvatureFlowFunction
 *  
 * This class encapsulate the finite difference equation which drives a
 * min/max curvature flow algorithm for denoising binary images.
 *
 * This class uses a zero flux Neumann boundary condition when computing
 * derivatives near the data boundary.
 *
 * This class operates as part of the finite difference solver hierarchy.
 *
 * \sa BinaryMinMaxCurvatureFlowImageFilter
 * \sa ZeroFluxNeumannBoundaryCondition 
 * \ingroup FiniteDifferenceFunctions
*/
template <class TImage>
class ITK_EXPORT BinaryMinMaxCurvatureFlowFunction :
    public MinMaxCurvatureFlowFunction<TImage>
{
public:
  /**  Standard class typedefs. */
  typedef BinaryMinMaxCurvatureFlowFunction Self;
  typedef MinMaxCurvatureFlowFunction<TImage> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( BinaryMinMaxCurvatureFlowFunction,
                MinMaxCurvatureFlowFunction );
  
  /** Inherit some parameters from the superclass type. */
  typedef typename Superclass::PixelType PixelType;
  typedef typename Superclass::RadiusType RadiusType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  typedef typename Superclass::FloatOffsetType FloatOffsetType;
  typedef typename Superclass::ImageType  ImageType;

  /** Extract superclass dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);  

  /** Set/Get the threshold value. */
  void SetThreshold( const double thresh )
  { m_Threshold = thresh; }
  const double & GetThreshold() const
  { return m_Threshold; }

  /** This method computes the solution update for each pixel that does not
   * lie on a the data set boundary. */
  virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
                                  void * globalData,
                                  const FloatOffsetType& offset = FloatOffsetType(0.0)
    );

protected:
  BinaryMinMaxCurvatureFlowFunction();
  ~BinaryMinMaxCurvatureFlowFunction() {}

private:
  BinaryMinMaxCurvatureFlowFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double           m_Threshold;

};

}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryMinMaxCurvatureFlowFunction.txx"
#endif

#endif
