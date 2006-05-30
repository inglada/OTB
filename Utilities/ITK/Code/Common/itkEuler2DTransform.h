/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkEuler2DTransform.h,v $
  Language:  C++
  Date:      $Date: 2005/08/11 16:44:08 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkEuler2DTransform_h
#define __itkEuler2DTransform_h

#include <iostream>
#include "itkRigid2DTransform.h"

namespace itk
{

/** \brief Euler2DTransform of a vector space (e.g. space coordinates)
 *
 * This transform applies a rigid transformation is 2D space.
 * The transform is specified as a rotation around arbitrary center
 * and is followed by a translation.
 *
 * This transform is basically is a synonym for Rigid2DTransform.
 *
 * \sa Rigid2DTransform
 *
 * \ingroup Transforms
 */
template < class TScalarType=double >    // Data type for scalars 
class ITK_EXPORT Euler2DTransform : 
            public Rigid2DTransform< TScalarType > 
{
public:
  /** Standard class typedefs. */
  typedef Euler2DTransform Self;
  typedef Rigid2DTransform< TScalarType >   Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
    
  /** New macro for creation of through a Smart Pointer. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( Euler2DTransform, Rigid2DTransform );

  /** Dimension of parameters. */
  itkStaticConstMacro(SpaceDimension, unsigned int, 2);
  itkStaticConstMacro(ParametersDimension, unsigned int, 3);

  /** Scalar type. */
  typedef typename Superclass::ScalarType  ScalarType;

  /** Parameters type. */
  typedef typename Superclass::ParametersType  ParametersType;

  /** Jacobian type. */
  typedef typename Superclass::JacobianType  JacobianType;
  
  /** Point type. */
  typedef typename Superclass::InputPointType   InputPointType;
  typedef typename Superclass::OutputPointType  OutputPointType;
  
  /** Vector type. */
  typedef typename Superclass::InputVectorType   InputVectorType;
  typedef typename Superclass::OutputVectorType  OutputVectorType;
  
  /** CovariantVector type. */
  typedef typename Superclass::InputCovariantVectorType   InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType  OutputCovariantVectorType;
  
  /** VnlVector type. */
  typedef typename Superclass::InputVnlVectorType   InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType  OutputVnlVectorType;
  typedef typename Superclass::MatrixType           MatrixType;
  
  /**
   * This method creates and returns a new Euler2DTransform object
   * which is the inverse of self.
   **/
  void CloneInverseTo( Pointer & newinverse ) const;

  /**
   * This method creates and returns a new Euler2DTransform object
   * which has the same parameters as self.
   **/
  void CloneTo( Pointer & clone ) const;

  /**
   * Update the angle from the underlying matrix. This method
   * is old and is retained for backward compatibility.
   */
  void ComputeAngleFromMatrix()
    { this->ComputeMatrixParameters(); }

protected:
  Euler2DTransform();
  ~Euler2DTransform(){};

  Euler2DTransform(unsigned int outputSpaceDimension, unsigned int parametersDimension);

  void PrintSelf(std::ostream &os, Indent indent) const;

private:
  Euler2DTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


}; //class Euler2DTransform


}  // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkEuler2DTransform.txx"
#endif

#endif /* __itkEuler2DTransform_h */
