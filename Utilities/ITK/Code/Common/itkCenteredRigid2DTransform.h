/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCenteredRigid2DTransform.h,v $
  Language:  C++
  Date:      $Date: 2009-11-29 01:38:00 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkCenteredRigid2DTransform_h
#define __itkCenteredRigid2DTransform_h

#include <iostream>
#include "itkRigid2DTransform.h"

namespace itk
{

/** \class CenteredRigid2DTransform
 * \brief CenteredRigid2DTransform of a vector space (e.g. space coordinates)
 *
 * This transform applies a rigid transformation is 2D space.
 * The transform is specified as a rotation around arbitrary center
 * and is followed by a translation.
 *
 * The main difference between this class and its superclass
 * Rigid2DTransform is that the center of rotation is exposed
 * for optimization.
 *
 * The serialization of the optimizable parameters is an array of 5 elements
 * ordered as follows:
 * p[0] = angle
 * p[1] = x coordinate of the center
 * p[2] = y coordinate of the center
 * p[3] = x component of the translation
 * p[4] = y component of the translation
 *
 * There are no fixed parameters.
 *
 * \sa Rigid2DTransform 
 *
 * \ingroup Transforms
 */
template < class TScalarType=double >    // Data type for scalars
class ITK_EXPORT CenteredRigid2DTransform : 
            public Rigid2DTransform< TScalarType > 
{
public:
  /** Standard class typedefs. */
  typedef CenteredRigid2DTransform          Self;
  typedef Rigid2DTransform< TScalarType >   Superclass;
  typedef SmartPointer<Self>                Pointer;
  typedef SmartPointer<const Self>          ConstPointer;
    
  /** New macro for creation of through a Smart Pointer. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( CenteredRigid2DTransform, Rigid2DTransform );

  /** Dimension of parameters. */
  itkStaticConstMacro(SpaceDimension, unsigned int, 2);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, 2);
  itkStaticConstMacro(ParametersDimension, unsigned int, 5);

  /** Scalar type. */
  typedef typename Superclass::ScalarType  ScalarType;

  /** Parameters type. */
  typedef typename Superclass::ParametersType         ParametersType;
  typedef typename Superclass::ParametersValueType    ParametersValueType;

  /** Jacobian type. */
  typedef typename Superclass::JacobianType  JacobianType;
  
  /** Offset type. */
  typedef typename Superclass::OffsetType  OffsetType;

  /** Point type. */
  typedef typename Superclass::InputPointType         InputPointType;
  typedef typename Superclass::OutputPointType        OutputPointType;
  typedef typename Superclass::InputPointValueType    InputPointValueType;
  
  /** Vector type. */
  typedef typename Superclass::InputVectorType        InputVectorType;
  typedef typename Superclass::OutputVectorType       OutputVectorType;
  typedef typename Superclass::OutputVectorValueType  OutputVectorValueType;
  
  /** CovariantVector type. */
  typedef typename Superclass::InputCovariantVectorType   
                                                InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType  
                                                OutputCovariantVectorType;
  
  /** VnlVector type. */
  typedef typename Superclass::InputVnlVectorType   InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType  OutputVnlVectorType;

  /** Base inverse transform type. This type should not be changed to the
   * concrete inverse transform type or inheritance would be lost. */
  typedef typename Superclass::InverseTransformBaseType InverseTransformBaseType;
  typedef typename InverseTransformBaseType::Pointer    InverseTransformBasePointer;
  
  /** Set the transformation from a container of parameters
   * This is typically used by optimizers.
   * There are 5 parameters. The first one represents the
   * rotation, the next two the center of rotation and 
   * the last two represents the offset. 
   *
   * \sa Transform::SetParameters()
   * \sa Transform::SetFixedParameters() */
  void SetParameters( const ParametersType & parameters );

  /** Get the parameters that uniquely define the transform
   * This is typically used by optimizers.
   * There are 3 parameters. The first one represents the
   * rotation, the next two the center of rotation and 
   * the last two represents the offset. 
   *
   * \sa Transform::GetParameters()
   * \sa Transform::GetFixedParameters() */
  const ParametersType & GetParameters( void ) const;
  
  /** This method computes the Jacobian matrix of the transformation
   * at a given input point.
   *
   * \sa Transform::GetJacobian() */
  const JacobianType & GetJacobian(const InputPointType  &point ) const;

  /** Set the fixed parameters and update internal transformation. 
   * This is a null function as there are no fixed parameters. */
  virtual void SetFixedParameters( const ParametersType & );

  /** Get the Fixed Parameters. An empty array is returned
   * as there are no fixed parameters. */
  virtual const ParametersType& GetFixedParameters(void) const;

  /**
   * This method creates and returns a new CenteredRigid2DTransform object
   * which is the inverse of self. */
  void CloneInverseTo( Pointer & newinverse ) const;

  /** Get an inverse of this transform. */
  bool GetInverse(Self* inverse) const;

  /** Return an inverse of this transform. */
  virtual InverseTransformBasePointer GetInverseTransform() const;

  /**
   * This method creates and returns a new CenteredRigid2DTransform object
   * which has the same parameters as self. */
  void CloneTo( Pointer & clone ) const;

protected:
  CenteredRigid2DTransform();
  ~CenteredRigid2DTransform(){};

  CenteredRigid2DTransform( unsigned int outputSpaceDimension, 
                            unsigned int parametersDimension);

  void PrintSelf(std::ostream &os, Indent indent) const;

private:
  CenteredRigid2DTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; //class CenteredRigid2DTransform


}  // namespace itk

/** Define instantiation macro for this template. */
#define ITK_TEMPLATE_CenteredRigid2DTransform(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT CenteredRigid2DTransform< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef CenteredRigid2DTransform< ITK_TEMPLATE_1 x > \
                                            CenteredRigid2DTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkCenteredRigid2DTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkCenteredRigid2DTransform.txx"
#endif

#endif /* __itkCenteredRigid2DTransform_h */
