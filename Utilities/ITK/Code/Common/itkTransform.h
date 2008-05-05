/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransform.h,v $
  Language:  C++
  Date:      $Date: 2008-01-23 22:44:36 $
  Version:   $Revision: 1.57 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTransform_h
#define __itkTransform_h

#include "itkTransformBase.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkCovariantVector.h"
#include "vnl/vnl_vector_fixed.h"
#include "itkArray.h"
#include "itkArray2D.h"

#include "itkObjectFactory.h"


namespace itk
{
  
/** \class Transform
 * \brief Transform points and vector from an input space to an output space.
 *
 * This abstract class define the generic interface for a geometrical 
 * transformation from one space to another. The class provides methods
 * for mapping points, vectors and covariant vectors from the input space 
 * to the output space. 
 *
 * Given that transformation are not necesarily invertible, this basic
 * class does not provide the methods for back transfromation. Back transform
 * methods are implemented in derived classes where appropriate.
 * 
 * \par Registration Framework Support
 * Typically a Transform class has several methods for setting its 
 * parameters. For use in the registration framework, the parameters must
 * also be represented by an array of doubles to allow communication
 * with generic optimizers. The Array of transformation parameters is set using
 * the SetParameters() method.
 *
 * Another requirement of the registration framework is the computation
 * of the transform Jacobian. In general, a ImageToImageMetric requires
 * the knowledge of the Jacobian in order to compute the metric derivatives.
 * The Jacobian is a matrix whose element are the partial derivatives
 * of the output point with respect to the array of parameters that defines
 * the transform.
 *
 * \ingroup Transforms
 *
 */
template <class TScalarType,
          unsigned int NInputDimensions=3, 
          unsigned int NOutputDimensions=3>
class ITK_EXPORT  Transform  : public TransformBase
{
public:
  /** Standard class typedefs. */
  typedef Transform  Self;
  typedef TransformBase Superclass;
  typedef SmartPointer< Self >   Pointer;
  typedef SmartPointer< const Self >  ConstPointer;
  
  /** New method for creating an object using a factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( Transform, TransformBase );

  /** Dimension of the domain space. */
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);

  /** Get the size of the input space */
  unsigned int GetInputSpaceDimension(void) const {return NInputDimensions;}

  /** Get the size of the output space */
  unsigned int GetOutputSpaceDimension(void) const {return NOutputDimensions;}

  /** Type of the scalar representing coordinate and vector elements. */
  typedef  TScalarType     ScalarType;

  /** Type of the input parameters. */
  typedef  typename Superclass::ParametersType         ParametersType;

  /** Type of the Jacobian matrix. */
  typedef  Array2D< double >                           JacobianType;

  /** Standard vector type for this class. */
  typedef Vector<TScalarType, NInputDimensions>  InputVectorType;
  typedef Vector<TScalarType, NOutputDimensions> OutputVectorType;
  
  /** Standard covariant vector type for this class */
  typedef CovariantVector<TScalarType, NInputDimensions>  InputCovariantVectorType;
  typedef CovariantVector<TScalarType, NOutputDimensions> OutputCovariantVectorType;
  
  /** Standard vnl_vector type for this class. */
  typedef vnl_vector_fixed<TScalarType, NInputDimensions>  InputVnlVectorType;
  typedef vnl_vector_fixed<TScalarType, NOutputDimensions> OutputVnlVectorType;
  
  /** Standard coordinate point type for this class */
  typedef Point<TScalarType, NInputDimensions> InputPointType;
  typedef Point<TScalarType, NOutputDimensions> OutputPointType;
  
  /**  Method to transform a point. */
  virtual OutputPointType TransformPoint(const InputPointType  & ) const
    { return OutputPointType(); } 

  /**  Method to transform a vector. */
  virtual OutputVectorType    TransformVector(const InputVectorType &) const
    { return OutputVectorType(); }

  /**  Method to transform a vnl_vector. */
  virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const
    { return OutputVnlVectorType(); }

  /**  Method to transform a CovariantVector. */
  virtual OutputCovariantVectorType TransformCovariantVector(
    const InputCovariantVectorType &) const
    { return OutputCovariantVectorType(); } 

  /** Set the transformation parameters and update internal transformation.
   * SetParameters gives the transform the option to set it's
   * parameters by keeping a reference to the parameters, or by
   * copying.  To force the transform to copy it's parameters call
   * SetParametersByValue.
   * \sa SetParametersByValue
   */
  virtual void SetParameters( const ParametersType & ) 
    { itkExceptionMacro( << "Subclasses should override this method" ) };

  /** Set the transformation parameters and update internal transformation. 
   * This method forces the transform to copy the parameters.  The
   * default implementation is to call SetParameters.  This call must
   * be overridden if the transform normally implements SetParameters
   * by keeping a reference to the parameters.
   * \sa SetParameters
   */
  virtual void SetParametersByValue ( const ParametersType & p ) 
    { this->SetParameters ( p ); };

  /** Get the Transformation Parameters. */
  virtual const ParametersType& GetParameters(void) const
    { itkExceptionMacro( << "Subclasses should override this method" );
      return m_Parameters; };

  /** Set the fixed parameters and update internal transformation. */
  virtual void SetFixedParameters( const ParametersType & ) 
    { itkExceptionMacro( << "Subclasses should override this method" ) };

  /** Get the Fixed Parameters. */
  virtual const ParametersType& GetFixedParameters(void) const
    { itkExceptionMacro( << "Subclasses should override this method" );
      return m_Parameters; };

  /** Compute the Jacobian of the transformation
   *
   * This method computes the Jacobian matrix of the transformation
   * at a given input point. The rank of the Jacobian will also indicate 
   * if the transform is invertible at this point.
   *
   * The Jacobian is be expressed as a matrix of partial derivatives of the
   * output point components with respect to the parameters that defined
   * the transform:
   *
   * \f[
   *
      J=\left[ \begin{array}{cccc}
      \frac{\partial x_{1}}{\partial p_{1}} & 
      \frac{\partial x_{1}}{\partial p_{2}} & 
      \cdots  & \frac{\partial x_{1}}{\partial p_{m}}\\
      \frac{\partial x_{2}}{\partial p_{1}} & 
      \frac{\partial x_{2}}{\partial p_{2}} & 
      \cdots  & \frac{\partial x_{2}}{\partial p_{m}}\\
      \vdots  & \vdots  & \ddots  & \vdots \\
      \frac{\partial x_{n}}{\partial p_{1}} & 
      \frac{\partial x_{n}}{\partial p_{2}} & 
      \cdots  & \frac{\partial x_{n}}{\partial p_{m}}
      \end{array}\right] 
   *
   * \f]
   * **/
  virtual const JacobianType & GetJacobian(const InputPointType  &) const
    { itkExceptionMacro( << "Subclass should override this method" );
      return m_Jacobian; }; 


  /** Return the number of parameters that completely define the Transfom  */
  virtual unsigned int GetNumberOfParameters(void) const 
                      { return m_Parameters.Size(); }

  /** Returns a boolean indicating whether it is possible or not to compute the
   * inverse of this current Transform. If it is possible, then the inverse of
   * the transform is returned in the inverseTransform variable passed by the
   * user.  The inverse is recomputed if this current transform has been modified.
   * This method is intended to be overriden by derived classes.
   * */
  bool GetInverse(Self * inverseTransform) const {return false;}

  /** Generate a platform independant name */
  virtual std::string GetTransformTypeAsString() const;

  /** Indicates if this transform is linear. A transform is defined to be
   * linear if the transform of a linear combination of points is equal to the
   * linear combination (with the same coefficients) of the individual
   * transforms of each point. The transform T will be linear if given two
   * points P and Q, and scalar coefficients a and b, then
   *
   *           T( a*P + b*Q ) = a * T(P) + b * T(Q)
   * 
   * By default, we assume this to NOT be the case for most transforms.
   * However, transforms for which this is true will overload and reimplement
   * this method accordingly.
   * 
   **/
  virtual bool IsLinear() const { return false; }

protected:
  Transform(); 
  Transform(unsigned int Dimension, unsigned int NumberOfParameters);
  virtual ~Transform() {};


  mutable ParametersType     m_Parameters;
  mutable ParametersType     m_FixedParameters;
  mutable JacobianType       m_Jacobian;

private:
  Transform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_Transform(_, EXPORT, x, y) namespace itk { \
  _(3(class EXPORT Transform< ITK_TEMPLATE_3 x >)) \
  namespace Templates { typedef Transform< ITK_TEMPLATE_3 x > Transform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkTransform.txx"
#endif

#endif
