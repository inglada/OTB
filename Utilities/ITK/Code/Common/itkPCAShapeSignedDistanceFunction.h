/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPCAShapeSignedDistanceFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-02-19 19:41:22 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPCAShapeSignedDistanceFunction_h
#define __itkPCAShapeSignedDistanceFunction_h

#include "itkShapeSignedDistanceFunction.h"
#include "itkImage.h"
#include "itkInterpolateImageFunction.h"
#include "itkExtrapolateImageFunction.h"
#include "itkTransform.h"

namespace itk
{


/** \class PCAShapeSignedDistanceFunction
 * \brief Compute the signed distance from a N-dimensional PCA Shape.
 *
 * This class computes the signed distance from a N-dimensional shape defined
 * by:
 * (1) a mean signed distance image \f$ M(x) \f$, 
 * (2) the first \f$ q \f$ principal components images 
 * \f$ P_i(x) \f$ and 
 * (3) a transform \f$ T(x) \f$ to define the pose 
 * (i.e. position or orientation of the shape).
 *
 * A particular instance of the shape is defined by a set of parameters \f$ p \f$. 
 * The first \f$ q \f$ parameters defines the weights applied to each principal components 
 * and the remaining parameters is used to define the transform. The user
 * should refer to the documentation of the particular Transform class being used.
 * The first set of parameters are called the ShapeParameters and the remaining
 * parameters the PoseParameters.
 *
 * The method Evaluate( point x ) returns the approximate signed to the 
 * shape at point x such that:
 *
 * \f[ s = M(T(x)) + \sum_i^{q} p[i] * \sigma[i] * P_i(T(x)) \f]
 * 
 * Where \sigma[i] are the square root of the eigenvalues. These are defined using
 * method SetPrincipalComponentStandardDeviations().
 *
 * This class is templated over the coordinate representation type 
 * (e.g. float or double) and the space dimension.
 *
 * \sa ShapeSignedDistanceFunction
 * \sa Transform
 * \ingroup ImageFunctions
 * 
 * */
template< typename TCoordRep, 
          unsigned int VSpaceDimension,
          typename TImage = Image<double,VSpaceDimension> >
class ITK_EXPORT PCAShapeSignedDistanceFunction : 
  public ShapeSignedDistanceFunction<TCoordRep, VSpaceDimension>
{

public:
  /** Standard class typedefs. */
  typedef PCAShapeSignedDistanceFunction          Self;
  typedef ShapeSignedDistanceFunction<
    TCoordRep, VSpaceDimension>                   Superclass;
  typedef SmartPointer<Self>                      Pointer;
  typedef SmartPointer<const Self>                ConstPointer;
  

  /** Run-time type information (and related methods). */
  itkTypeMacro(PCAShapeSignedDistanceFunction, ShapeSignedDistancFunction);

  /** New macro for creation of through the object factory. */
  itkNewMacro(Self);

  /** Dimension underlying input image. */
  itkStaticConstMacro(SpaceDimension,unsigned int,Superclass::SpaceDimension);


  /** CoordRep typedef support. */
  typedef typename Superclass::CoordRepType       CoordRepType;

  /** InputeType typedef support. */
  typedef typename Superclass::InputType          InputType;

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType         OutputType;
  
  /** Point typedef support. */
  typedef typename Superclass::PointType          PointType;

  /** Parameters typedef support. */
  typedef typename Superclass::ParametersType     ParametersType;


  /** Image typedef support. */
  typedef TImage                                  ImageType;
  typedef typename ImageType::Pointer             ImagePointer;
  typedef std::vector<ImagePointer>               ImagePointerVector;

  /** Transform typedef support. */
  typedef Transform<CoordRepType, 
                    itkGetStaticConstMacro(SpaceDimension), 
                    itkGetStaticConstMacro(SpaceDimension)> TransformType;

  /** Interpolator typedef support. */
  typedef InterpolateImageFunction<ImageType, CoordRepType> InterpolatorType;
  typedef typename InterpolatorType::Pointer                InterpolatorPointer;
  typedef std::vector<InterpolatorPointer>                  InterpolatorPointerVector;

  /** extrapolator typedef support. */
  typedef ExtrapolateImageFunction<ImageType, CoordRepType> ExtrapolatorType;
  typedef typename ExtrapolatorType::Pointer                ExtrapolatorPointer;
  typedef std::vector<ExtrapolatorPointer>                  ExtrapolatorPointerVector;

  /** function typedef support. */
  typedef ImageFunction<ImageType, double, CoordRepType> FunctionType;
  typedef typename FunctionType::Pointer                 FunctionPointer;
  typedef std::vector<FunctionPointer>                   FunctionPointerVector;


  /** Set/Get the number of principal components 
   * SetNumberOfPrincipalComponents must be called before SetParameters */
  void SetNumberOfPrincipalComponents(unsigned int n);
  itkGetMacro(NumberOfPrincipalComponents, unsigned int);

  /** Set/Get the mean image. */
  itkSetObjectMacro(MeanImage, ImageType);
  itkGetObjectMacro(MeanImage, ImageType);

  /** Set/Get the principal component images. */
  void SetPrincipalComponentImages(ImagePointerVector v)
    { m_PrincipalComponentImages = v; }
//  ImagePointerVector & GetPrincipalComponentImages()
//    { return m_PrincipalComponentImages; }

  /** Set/Get the principal component standard deviations. These values corresponds
   * to the square root of the eigenvalues of the principal components. */
  itkSetMacro(PrincipalComponentStandardDeviations, ParametersType);
  itkGetMacro(PrincipalComponentStandardDeviations, ParametersType);

  /** Set/Get transform. */
  itkSetObjectMacro(Transform, TransformType);
  itkGetObjectMacro(Transform, TransformType);


  /** A PCAShape is defined by a set of shape and pose parameters. */
  virtual void SetParameters( const ParametersType & );
  virtual unsigned int GetNumberOfShapeParameters(void) const
    { return m_NumberOfPrincipalComponents; }
  virtual unsigned int GetNumberOfPoseParameters(void) const
    { return m_Transform ? m_Transform->GetNumberOfParameters() : 0; }

  /** Evaluate the signed distance from a shape at a given position. */
  virtual OutputType Evaluate(const PointType& point) const;

  /** Initialize must be called before the first call of  
   Evaluate() to allow the class to validate any inputs. */
  virtual void Initialize() throw ( ExceptionObject );


protected:
  PCAShapeSignedDistanceFunction();
  ~PCAShapeSignedDistanceFunction(){};

  void PrintSelf(std::ostream& os, Indent indent) const;


private:
  PCAShapeSignedDistanceFunction(const Self&); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented


  /** intrinsic data members */
  unsigned int                  m_NumberOfPrincipalComponents;
  unsigned int                  m_NumberOfTransformParameters;

  ImagePointer                  m_MeanImage;
  ImagePointerVector            m_PrincipalComponentImages;
  ParametersType                m_PrincipalComponentStandardDeviations;

  /** transform and interpolator/extrapolator for image interpolation */
  typename TransformType::Pointer        m_Transform;
  InterpolatorPointerVector              m_Interpolators;
  ExtrapolatorPointerVector              m_Extrapolators;
  mutable FunctionPointerVector          m_Selectors;

  /** shape and pose parameters */
  ParametersType                m_WeightOfPrincipalComponents;
  ParametersType                m_TransformParameters;

};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_PCAShapeSignedDistanceFunction(_, EXPORT, x, y) namespace itk { \
  _(3(class EXPORT PCAShapeSignedDistanceFunction< ITK_TEMPLATE_3 x >)) \
  namespace Templates { typedef PCAShapeSignedDistanceFunction< ITK_TEMPLATE_3 x > \
                                          PCAShapeSignedDistanceFunction##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkPCAShapeSignedDistanceFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkPCAShapeSignedDistanceFunction.txx"
#endif

#endif
