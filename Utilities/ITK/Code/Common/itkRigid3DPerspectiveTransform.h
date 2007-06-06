/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRigid3DPerspectiveTransform.h,v $
  Language:  C++
  Date:      $Date: 2007/01/30 20:56:09 $
  Version:   $Revision: 1.30 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRigid3DPerspectiveTransform_h
#define __itkRigid3DPerspectiveTransform_h

#include "itkExceptionObject.h"
#include "vnl/vnl_quaternion.h"
#include <iostream>
#include "itkMatrix.h"
#include "itkTransform.h"
#include "itkVersor.h"

namespace itk
{

/** \brief Rigid3DTramsform of a vector space (e.g. space coordinates)
 *
 * This transform applies a rotation and translation to the 3D space
 * followed by a projection to 2D space along the Z axis.
 *
 * \ingroup Transforms
 */

template <
    class TScalarType=double>    // Data type for scalars (float or double)
class ITK_EXPORT Rigid3DPerspectiveTransform : 
        public Transform<  TScalarType, 3, 2 > 
{
public:
  /** Dimension of the domain space. */
  itkStaticConstMacro(InputSpaceDimension, unsigned int, 3);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, 2);

  /** Dimension of parameters. */
  itkStaticConstMacro(SpaceDimension, unsigned int, 3);
  itkStaticConstMacro(ParametersDimension, unsigned int, 6);

  /** Standard class typedefs. */ 
  typedef Rigid3DPerspectiveTransform Self;
  typedef Transform<  TScalarType, 
                      itkGetStaticConstMacro(InputSpaceDimension),
                      itkGetStaticConstMacro(OutputSpaceDimension)> Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( Rigid3DPerspectiveTransform, Transform );

  /** New macro for creation of through a Smart Pointer. */
  itkNewMacro( Self );

  /** Scalar type. */
  typedef typename Superclass::ScalarType  ScalarType;

  /** Parameters type. */
  typedef typename Superclass::ParametersType  ParametersType;

  /** Jacobian type. */
  typedef typename Superclass::JacobianType  JacobianType;

  /** Standard matrix type for this class. */
  typedef Matrix<TScalarType, itkGetStaticConstMacro(InputSpaceDimension), itkGetStaticConstMacro(InputSpaceDimension)> MatrixType;

  /** Standard vector type for this class. */
  typedef Vector<TScalarType, itkGetStaticConstMacro(InputSpaceDimension)> OffsetType;

  /** Standard vector type for this class. */
  typedef Vector<TScalarType, itkGetStaticConstMacro(InputSpaceDimension)> InputVectorType;
  typedef Vector<TScalarType, itkGetStaticConstMacro(OutputSpaceDimension)> OutputVectorType;
  
  /** Standard coordinate point type for this class. */
  typedef Point<TScalarType, itkGetStaticConstMacro(InputSpaceDimension)>    InputPointType;
  typedef Point<TScalarType, itkGetStaticConstMacro(OutputSpaceDimension)>    OutputPointType;
  
  /** Standard vnl_quaternion type. */
  typedef vnl_quaternion<TScalarType>           VnlQuaternionType;

  /** Versor type. */
  typedef Versor<TScalarType>             VersorType;
  typedef typename VersorType::VectorType  AxisType;
  typedef typename VersorType::ValueType   AngleType;
  
  /** Get offset of an Rigid3DPerspectiveTransform
   * This method returns the value of the offset of the
   * Rigid3DPerspectiveTransform. */
  const OffsetType & GetOffset() const
    { return m_Offset; }

  /** Get rotation from an Rigid3DPerspectiveTransform.
   * This method returns the value of the rotation of the
   * Rigid3DPerspectiveTransform. */
  const VersorType & GetRotation() const
    { return m_Versor; }

  /** Set/Get the transformation from a container of parameters.
   * This is typically used by optimizers.
   * There are 6 parameters. The first three represent the
   * versor and the last three represents the offset. */
  void SetParameters( const ParametersType & parameters );
  const ParametersType & GetParameters() const;

  /** This method sets the offset of an Rigid3DPerspectiveTransform to a
   * value specified by the user. */
  void SetOffset(const OffsetType &offset)
    { m_Offset = offset; return; }

  /** This method sets the rotation of an Rigid3DPerspectiveTransform to a
   * value specified by the user.  */
  void SetRotation(const VersorType &rotation);

  /** Set Rotation of the Rigid transform.
   * This method sets the rotation of an Rigid3DTransform to a
   * value specified by the user using the axis of rotation an
   * the angle. */
  void SetRotation(const Vector<TScalarType,3> &axis, double angle);

  /** Set the Focal Distance of the projection
   * This method sets the focal distance for the perspective
   * projection to a value specified by the user. */
  void SetFocalDistance( TScalarType focalDistance )
    { m_FocalDistance = focalDistance; }

  /** Return the Focal Distance */
  double GetFocalDistance( void ) const
    { return m_FocalDistance; }

  /** Transform by a Rigid3DPerspectiveTransform. This method 
   *  applies the transform given by self to a
   *  given point, returning the transformed point. */
  OutputPointType  TransformPoint(const InputPointType  &point ) const;

  /** Return the rotation matrix */
  const MatrixType & GetRotationMatrix() const {return m_RotationMatrix;}

  /** Compute the matrix. */
  void ComputeMatrix(void);

  /** Compute the Jacobian Matrix of the transformation at one point */
  virtual const JacobianType & GetJacobian(const InputPointType  &point ) const;
  
  /** Set a fixed offset: this allow to center the object to be transformed */
  itkGetConstReferenceMacro(FixedOffset,OffsetType);
  itkSetMacro(FixedOffset,OffsetType);

  /** Set the center of Rotation */
  itkSetMacro(CenterOfRotation,InputPointType);
  itkGetConstReferenceMacro(CenterOfRotation,InputPointType);


protected:
    Rigid3DPerspectiveTransform();
    ~Rigid3DPerspectiveTransform();
    void PrintSelf(std::ostream &os, Indent indent) const;

private:
  Rigid3DPerspectiveTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Offset of the transformation. */
  OffsetType          m_Offset;   

  /** Rotation of the transformation. */
  VersorType          m_Versor; 

  /** Set Focal distance of the projection. */
  TScalarType         m_FocalDistance;  

  /** Matrix representation of the rotation. */
  MatrixType          m_RotationMatrix;   

  
  /** Fixed offset*/
  OffsetType m_FixedOffset;

  /** Center of rotation */
  InputPointType m_CenterOfRotation;


}; //class Rigid3DPerspectiveTransform:



}  // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_Rigid3DPerspectiveTransform(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT Rigid3DPerspectiveTransform< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef Rigid3DPerspectiveTransform< ITK_TEMPLATE_1 x > \
                                                  Rigid3DPerspectiveTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkRigid3DPerspectiveTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkRigid3DPerspectiveTransform.txx"
#endif

#endif /* __itkRigid3DPerspectiveTransform_h */
