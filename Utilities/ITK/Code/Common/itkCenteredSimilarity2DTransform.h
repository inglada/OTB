/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCenteredSimilarity2DTransform.h,v $
  Language:  C++
  Date:      $Date: 2009-04-09 09:23:20 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkCenteredSimilarity2DTransform_h
#define __itkCenteredSimilarity2DTransform_h

#include <iostream>
#include "itkSimilarity2DTransform.h"

namespace itk
{

/** \class CenteredSimilarity2DTransform
 *  \brief CenteredSimilarity2DTransform of a vector space 
 *        (e.g. space coordinates)
 *
 * This transform applies a homogenous scale and rigid transform in
 * 2D space. The transform is specified as a scale and rotation around
 * a arbitrary center and is followed by a translation.
 * given one angle for rotation, a homogeneous scale and a 2D offset 
 * for translation. 
 *
 * The main difference between this class and its superclass
 * Similarity2DTransform is that the center of transformation is exposed
 * for optimization.
 *
 * The serialization of the optimizable parameters is an array of 6 elements
 * ordered as follows:
 * p[0] = scale
 * p[1] = angle
 * p[2] = x coordinate of the center
 * p[3] = y coordinate of the center
 * p[4] = x component of the translation
 * p[5] = y component of the translation
 *
 * There are no fixed parameters.
 *
 * \sa Similarity2DTransform
 *
 * \ingroup Transforms
 */
template < class TScalarType=double >    // Data type for scalars
class ITK_EXPORT CenteredSimilarity2DTransform : 
            public Similarity2DTransform< TScalarType > 
{
public:
  /** Standard class typedefs. */
  typedef CenteredSimilarity2DTransform          Self;
  typedef Similarity2DTransform< TScalarType >   Superclass;
  typedef SmartPointer<Self>                     Pointer;
  typedef SmartPointer<const Self>               ConstPointer;
    
  /** New macro for creation of through a Smart Pointer. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( CenteredSimilarity2DTransform, Similarity2DTransform );

  /** Dimension of parameters. */
  itkStaticConstMacro(SpaceDimension,           unsigned int, 2);
  itkStaticConstMacro(InputSpaceDimension,      unsigned int, 2);
  itkStaticConstMacro(OutputSpaceDimension,     unsigned int, 2);
  itkStaticConstMacro(ParametersDimension,      unsigned int, 6);

  /** Scalar type. */
  typedef typename Superclass::ScalarType  ScalarType;

  /** Parameters type. */
  typedef typename Superclass::ParametersType  ParametersType;

  /** Jacobian type. */
  typedef typename Superclass::JacobianType  JacobianType;
  
  /** Offset type. */
  typedef typename Superclass::OffsetType  OffsetType;

  /** Point type. */
  typedef typename Superclass::InputPointType   InputPointType;
  typedef typename Superclass::OutputPointType  OutputPointType;
  
  /** Vector type. */
  typedef typename Superclass::InputVectorType   InputVectorType;
  typedef typename Superclass::OutputVectorType  OutputVectorType;
  
  /** CovariantVector type. */
  typedef typename Superclass::InputCovariantVectorType   
                                                 InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType  
                                                 OutputCovariantVectorType;
  
  /** VnlVector type. */
  typedef typename Superclass::InputVnlVectorType   InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType  OutputVnlVectorType;

  /** Base inverse transform type. This type should not be changed to the
   * concrete inverse transform type or inheritance would be lost.*/
  typedef typename Superclass::InverseTransformBaseType InverseTransformBaseType;
  typedef typename InverseTransformBaseType::Pointer    InverseTransformBasePointer;
  
  /** Set the transformation from a container of parameters
    * This is typically used by optimizers.
    * There are 6 parameters. The first one represents the
    * scale, the second represents the angle of rotation, the next
    * two represent the center of the rotation
    * and the last two represent the translation.
    *
    * \sa Transform::SetParameters()
    * \sa Transform::SetFixedParameters() */
  void SetParameters( const ParametersType & parameters );

  /** Get the parameters that uniquely define the transform
   * This is typically used by optimizers.
   * There are 6 parameters. The first one represents the
   * scale, the second represents the angle of rotation, the next
   * two represent the center of the rotation
   * and the last two represent the translation.
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
   * This method creates and returns a new Rigid2DTransform object
   * which is the inverse of self. */
  void CloneInverseTo( Pointer & newinverse ) const;

  /** Get an inverse of this transform. */
  bool GetInverse(Self* inverse) const;

  /** Return an inverse of this transform. */
  virtual InverseTransformBasePointer GetInverseTransform() const;

  /**
   * This method creates and returns a new Rigid2DTransform object
   * which has the same parameters. */
  void CloneTo( Pointer & clone ) const;

protected:
  CenteredSimilarity2DTransform();
  CenteredSimilarity2DTransform( unsigned int spaceDimension, 
                                 unsigned int parametersDimension);

  ~CenteredSimilarity2DTransform(){};
  void PrintSelf(std::ostream &os, Indent indent) const;

private:
  CenteredSimilarity2DTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; //class CenteredSimilarity2DTransform


}  // namespace itk

/* Define instantiation macro for this template. */
#define ITK_TEMPLATE_CenteredSimilarity2DTransform(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT CenteredSimilarity2DTransform< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef CenteredSimilarity2DTransform< ITK_TEMPLATE_1 x > \
                                            CenteredSimilarity2DTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkCenteredSimilarity2DTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkCenteredSimilarity2DTransform.txx"
#endif

#endif /* __itkCenteredSimilarity2DTransform_h */
