/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkThinPlateR2LogRSplineKernelTransform.h,v $
  Language:  C++
  Date:      $Date: 2007-12-20 17:10:46 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkThinPlateR2LogRSplineKernelTransform_h
#define __itkThinPlateR2LogRSplineKernelTransform_h

#include "itkKernelTransform.h"

namespace itk
{
/** \class ThinPlateR2LogRSplineKernelTransform
 * This class defines the thin plate spline (TPS) transformation.
 * It is implemented in as straightforward a manner as possible from
 * the IEEE TMI paper by Davis, Khotanzad, Flamig, and Harms,
 * Vol. 16 No. 3 June 1997.
 *
 * The kernel used in this variant of TPS is \f$ R^2 log(R) \f$
 *
 * \ingroup Transforms
 */
template <class TScalarType,         // Data type for scalars (float or double)
          unsigned int NDimensions = 3>          // Number of dimensions
class ITK_EXPORT ThinPlateR2LogRSplineKernelTransform : 
                public KernelTransform<   TScalarType, NDimensions>
{
public:
  /** Standard class typedefs. */
  typedef ThinPlateR2LogRSplineKernelTransform Self;
  typedef KernelTransform<    TScalarType, NDimensions>   Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** New macro for creation of through a Smart Pointer */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ThinPlateR2LogRSplineKernelTransform, KernelTransform );

  /** Scalar type. */
  typedef typename Superclass::ScalarType  ScalarType;

  /** Parameters type. */
  typedef typename Superclass::ParametersType  ParametersType;

  /** Jacobian Type */
  typedef typename Superclass::JacobianType  JacobianType;

  /** Dimension of the domain space. */
  itkStaticConstMacro(SpaceDimension, unsigned int,Superclass::SpaceDimension);
                              
  /** These (rather redundant) typedefs are needed because on SGI, typedefs
   * are not inherited */
  typedef typename Superclass::InputPointType  InputPointType;
  typedef typename Superclass::OutputPointType  OutputPointType;
  typedef typename Superclass::InputVectorType InputVectorType;
  typedef typename Superclass::OutputVectorType OutputVectorType;
  typedef typename Superclass::InputCovariantVectorType InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType OutputCovariantVectorType;
  typedef typename Superclass::PointsIterator PointsIterator;
    

protected:
  ThinPlateR2LogRSplineKernelTransform() {};
  virtual ~ThinPlateR2LogRSplineKernelTransform() {}
  
  /** These (rather redundant) typedefs are needed because on SGI, typedefs
   * are not inherited. */
  typedef typename Superclass::GMatrixType GMatrixType;
  
  /** Compute G(x)
   * For the thin plate spline, this is:
   * G(x) = r(x)*I
   * \f$ G(x) = r(x)^2 log(r(x) ) *I \f$
   * where
   * r(x) = Euclidean norm = sqrt[x1^2 + x2^2 + x3^2]
   * \f[ r(x) = \sqrt{ x_1^2 + x_2^2 + x_3^2 }  \f]
   * I = identity matrix. */
  virtual void ComputeG(const InputVectorType& landmarkVector, GMatrixType & gmatrix) const;
  /**
   * \deprecated in ITK 3.6, please use void ComputeG(vector,gmatrix) instead.
   */
  itkLegacyMacro( virtual const GMatrixType & ComputeG(const InputVectorType& landmarkVector) const );


  /** Compute the contribution of the landmarks weighted by the kernel funcion
      to the global deformation of the space  */
  virtual void ComputeDeformationContribution( const InputPointType & inputPoint,
                                                     OutputPointType & result ) const;

 private:
  ThinPlateR2LogRSplineKernelTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_ThinPlateR2LogRSplineKernelTransform(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT ThinPlateR2LogRSplineKernelTransform< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef ThinPlateR2LogRSplineKernelTransform< ITK_TEMPLATE_2 x > \
                                                  ThinPlateR2LogRSplineKernelTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkThinPlateR2LogRSplineKernelTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkThinPlateR2LogRSplineKernelTransform.txx"
#endif

#endif // __itkThinPlateR2LogRSplineKernelTransform_h
