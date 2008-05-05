/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuaternionRigidTransform.h,v $
  Language:  C++
  Date:      $Date: 2007-01-30 20:56:09 $
  Version:   $Revision: 1.29 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuaternionRigidTransform_h
#define __itkQuaternionRigidTransform_h

#include <iostream>
#include "itkRigid3DTransform.h"
#include "vnl/vnl_quaternion.h"

namespace itk
{

/** \brief QuaternionRigidTransform of a vector space (e.g. space coordinates).
 *
 * This transform applies a rotation and translation to the space given 
 * a quaternion and a 3D translation. Rotation is about a user specified center.
 *
 * The parameters for this transform can be set either using individual Set
 * methods or in serialized form using SetParameters() and SetFixedParameters().
 *
 * The serialization of the optimizable parameters is an array of 7 elements.
 * The first 4 elements are the components of the quaternion representation
 * of 3D rotation. The last 3 parameters defines the translation in each
 * dimension.
 *
 * The serialization of the fixed parameters is an array of 3 elements defining
 * the center of rotation.
 *
 *
 * \ingroup Transforms
 */
template < class TScalarType=double >    // Data type for scalars (float or double)
class ITK_EXPORT QuaternionRigidTransform :
        public Rigid3DTransform< TScalarType > 
{
public:
  /** Standard class typedefs.   */
  typedef QuaternionRigidTransform            Self;
  typedef Rigid3DTransform< TScalarType >     Superclass;
  typedef SmartPointer<Self>                  Pointer;
  typedef SmartPointer<const Self>            ConstPointer;
  
  /** New macro for creation of through a Smart Pointer   */
  itkNewMacro( Self );

  /** Run-time type information (and related methods).   */
  itkTypeMacro( QuaternionRigidTransform, Rigid3DTransform );

  /** Dimension of parameters   */
  itkStaticConstMacro(InputSpaceDimension, unsigned int, 3);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, 3);
  itkStaticConstMacro(SpaceDimension, unsigned int, 3);
  itkStaticConstMacro(ParametersDimension, unsigned int, 7);

  /** Parameters Type   */
  typedef typename Superclass::ParametersType         ParametersType;
  typedef typename Superclass::JacobianType           JacobianType;
  typedef typename Superclass::ScalarType             ScalarType;
  typedef typename Superclass::InputPointType         InputPointType;
  typedef typename Superclass::OutputPointType        OutputPointType;
  typedef typename Superclass::InputVectorType        InputVectorType;
  typedef typename Superclass::OutputVectorType       OutputVectorType;
  typedef typename Superclass::InputVnlVectorType     InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType    OutputVnlVectorType;
  typedef typename Superclass::InputCovariantVectorType 
                                                      InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType      
                                                      OutputCovariantVectorType;
  typedef typename Superclass::MatrixType             MatrixType;
  typedef typename Superclass::InverseMatrixType      InverseMatrixType;
  typedef typename Superclass::CenterType             CenterType;
  typedef typename Superclass::OffsetType             OffsetType;
  typedef typename Superclass::TranslationType        TranslationType;


  /** VnlQuaternion type.  */
  typedef vnl_quaternion<TScalarType>           VnlQuaternionType;

  /** Compute the Jacobian Matrix of the transformation at one point */
  /** Set the rotation of the rigid transform.
   * This method sets the rotation of a QuaternionRigidTransform to a
   * value specified by the user. */
  void SetRotation(const VnlQuaternionType &rotation);

  /** Get the rotation from an QuaternionRigidTransform.
   * This method returns the value of the rotation of the
   * QuaternionRigidTransform.   **/
  const VnlQuaternionType & GetRotation(void) const
    { return m_Rotation; }

  /** Set the parameters to the IdentityTransform */
  virtual void SetIdentity(void);

  /** Set the transformation from a container of parameters.
   * This is typically used by optimizers.
   * There are 7 parameters. The first four represents the
   * quaternion and the last three represents the
   * offset. */
  void SetParameters( const ParametersType & parameters );
  virtual const ParametersType & GetParameters() const;

  /** Compute the Jacobian of the transformation.
   * This method computes the Jacobian matrix of the transformation.
   * given point or vector, returning the transformed point or
   * vector. The rank of the Jacobian will also indicate if the transform
   * is invertible at this point. */
  const JacobianType & GetJacobian(const InputPointType  &point ) const;

protected:
  QuaternionRigidTransform(const MatrixType &matrix,
                           const OutputVectorType &offset);
  QuaternionRigidTransform(unsigned int outputDims,
                           unsigned int paramDims);
  QuaternionRigidTransform();
  ~QuaternionRigidTransform(){};

  void ComputeMatrix();

  void ComputeMatrixParameters();

  void SetVarRotation(const VnlQuaternionType & rotation)
    { m_Rotation = rotation; };

  const InverseMatrixType & GetInverseMatrix( void ) const;

  void PrintSelf(std::ostream &os, Indent indent) const;

private:
  QuaternionRigidTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Rotation of the transformation. */
  VnlQuaternionType   m_Rotation;

}; //class QuaternionRigidTransform


}  // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_QuaternionRigidTransform(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT QuaternionRigidTransform< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef QuaternionRigidTransform< ITK_TEMPLATE_1 x > \
                                                  QuaternionRigidTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkQuaternionRigidTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkQuaternionRigidTransform.txx"
#endif

#endif /* __itkQuaternionRigidTransform_h */
