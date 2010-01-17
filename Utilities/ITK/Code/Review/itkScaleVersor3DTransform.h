/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScaleVersor3DTransform.h,v $
  Language:  C++
  Date:      $Date: 2009-08-09 16:35:47 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScaleVersor3DTransform_h
#define __itkScaleVersor3DTransform_h

#include <iostream>
#include "itkVersorRigid3DTransform.h"

namespace itk
{

/** \class ScaleVersor3DTransform
 * 
 * \brief This transform applies a Versor rotation, translation and
 * anisotropic scale to the space.
 * 
 * This Transform provides the next set of degrees of freedom after a
 * Similarity transform. In a Similarity transform it is possible to do
 * rotation, translation and uniform scaling. In this current transform we can
 * do rotation, translation and anisotropic scaling.
 *
 * \author Johnson H.J., Harris G., Williams K. University of Iow Carver
 * College of Medicine, Department of Psychiatry NeuroImaging Center
 * 
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/1291  or
 * http://www.insight-journal.org/browse/publication/180
 * 
 * \ingroup Transforms
 */
template < class TScalarType=double >  // Data type for scalars:float or double
class ITK_EXPORT ScaleVersor3DTransform : public VersorRigid3DTransform< TScalarType >
{
public:
  /** Standard class typedefs. */
  typedef ScaleVersor3DTransform                  Self;
  typedef VersorRigid3DTransform< TScalarType >   Superclass;
  typedef SmartPointer<Self>                      Pointer;
  typedef SmartPointer<const Self>                ConstPointer;

  /** New macro for creation of through a Smart Pointer. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ScaleVersor3DTransform, VersorRigid3DTransform );

  /** Dimension of parameters. */
  itkStaticConstMacro(InputSpaceDimension,  unsigned int,  3);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int,  3);
  itkStaticConstMacro(ParametersDimension,  unsigned int,  9);

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

  typedef typename Superclass::VersorType             VersorType;
  typedef typename Superclass::AxisType               AxisType;
  typedef typename Superclass::AngleType              AngleType;

  /** Scale Vector Type. */
  typedef Vector<TScalarType, 3>                      ScaleVectorType;

  /** Directly set the matrix of the transform.
   *
   * \sa MatrixOffsetTransformBase::SetMatrix() */
  virtual void SetMatrix(const MatrixType &matrix);

  /** Set the transformation from a container of parameters
   * This is typically used by optimizers.
   * There are 9 parameters:
   *   0-2   versor
   *   3-5   translation
   *   6-8   Scale
   **  */
  virtual void SetParameters( const ParametersType & parameters );
  virtual const ParametersType& GetParameters(void) const;

  void SetScale( const ScaleVectorType & scale );
  itkGetConstReferenceMacro( Scale, ScaleVectorType );

  void SetIdentity();

  /** This method computes the Jacobian matrix of the transformation.
   * given point or vector, returning the transformed point or
   * vector. The rank of the Jacobian will also indicate if the
   * transform is invertible at this point. */
  const JacobianType & GetJacobian(const InputPointType  &point ) const;

protected:
  ScaleVersor3DTransform();
  ScaleVersor3DTransform(const MatrixType &matrix,
    const OutputVectorType &offset);
  ScaleVersor3DTransform(unsigned int outputDims,
    unsigned int paramDims);
  ~ScaleVersor3DTransform()
    {
    };

  void PrintSelf(std::ostream &os, Indent indent) const;

  void SetVarScale(const ScaleVectorType & scale)
    {
    m_Scale = scale;
    }

  /** Compute the components of the rotation matrix in the superclass. */
  void ComputeMatrix(void);
  void ComputeMatrixParameters(void);

private:
  ScaleVersor3DTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /**  Vector containing the scale. */
  ScaleVectorType          m_Scale;

}; //class ScaleVersor3DTransform


}  // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScaleVersor3DTransform.txx"
#endif

#endif /* __ScaleVersor3DTransform_h */
