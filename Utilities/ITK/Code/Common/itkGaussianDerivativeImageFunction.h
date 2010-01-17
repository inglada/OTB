/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGaussianDerivativeImageFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-02-05 19:04:56 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGaussianDerivativeImageFunction_h
#define __itkGaussianDerivativeImageFunction_h

#include "itkNeighborhoodOperatorImageFunction.h"
#include "itkImageFunction.h"
#include "itkGaussianDerivativeSpatialFunction.h"
#include "itkGaussianSpatialFunction.h"

namespace itk
{

/**
 * \class GaussianDerivativeImageFunction
 * \brief Compute the gaussian derivatives of an the image
 *        at a specific location in space, i.e. point, index or continuous
 *        index.
 * This class is templated over the input image type.
 * \sa NeighborhoodOperator
 * \sa ImageFunction
 */
template <class TInputImage,class TOutput=double>
class ITK_EXPORT GaussianDerivativeImageFunction :
  public ImageFunction< TInputImage,
    Vector<TOutput,::itk::GetImageDimension<TInputImage>::ImageDimension>,
    TOutput >
{
public:

  /**Standard "Self" typedef */
  typedef GaussianDerivativeImageFunction Self;

  /** Standard "Superclass" typedef */
  typedef ImageFunction<TInputImage,
    Vector<TOutput,::itk::GetImageDimension<TInputImage>::ImageDimension>,
    TOutput > Superclass;

  /** Smart pointer typedef support. */
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( GaussianDerivativeImageFunction, ImageFunction );

  /** InputImageType typedef support. */
  typedef TInputImage                                 InputImageType;
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::IndexType          IndexType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension2, unsigned int,
                      InputImageType::ImageDimension);
  
  typedef ContinuousIndex<TOutput,itkGetStaticConstMacro(ImageDimension2)>
          ContinuousIndexType;


  typedef Neighborhood<InputPixelType, itkGetStaticConstMacro(ImageDimension2)> NeighborhoodType;
  typedef Neighborhood<TOutput, itkGetStaticConstMacro(ImageDimension2)> OperatorNeighborhoodType;

  typedef Vector<TOutput,itkGetStaticConstMacro(ImageDimension2)>  VectorType;
  typedef typename Superclass::OutputType  OutputType;
  typedef FixedArray<OperatorNeighborhoodType,2*itkGetStaticConstMacro(ImageDimension2)> OperatorArrayType;
  typedef NeighborhoodOperatorImageFunction<InputImageType,
                                             TOutput> OperatorImageFunctionType;
  typedef typename OperatorImageFunctionType::Pointer OperatorImageFunctionPointer;

  typedef GaussianDerivativeSpatialFunction<TOutput,1>  GaussianDerivativeFunctionType;
  typedef typename GaussianDerivativeFunctionType::Pointer GaussianDerivativeFunctionPointer;

  typedef GaussianSpatialFunction<TOutput,1>        GaussianFunctionType;
  typedef typename GaussianFunctionType::Pointer    GaussianFunctionPointer;

  /** Point typedef support. */
  typedef Point<TOutput,itkGetStaticConstMacro(ImageDimension2)> PointType;
  
  /** Evalutate the  in the given dimension at specified point */
  virtual OutputType Evaluate(const PointType& point) const;


  /** Evaluate the function at specified Index position */
  virtual OutputType EvaluateAtIndex( const IndexType & index ) const;

  /** Evaluate the function at specified ContinousIndex position. */
  virtual OutputType EvaluateAtContinuousIndex( 
    const ContinuousIndexType & index ) const;

  /** The variance for the discrete Gaussian kernel.  Sets the variance
   * independently for each dimension, but 
   * see also SetVariance(const double v). The default is 0.0 in each
   * dimension. If UseImageSpacing is true, the units are the physical units
   * of your image.  If UseImageSpacing is false then the units are
   * pixels. */
  void SetSigma( const double* sigma);
  void SetSigma( const double sigma);
  const double* GetSigma() const {return m_Sigma;}
 
  /** Set the extent of the kernel */
  void SetExtent( const double* extent);
  void SetExtent( const double extent);
  const double* GetExtent() const {return m_Extent;}

  /** Set the input image.
   * \warning this method caches BufferedRegion information.
   * If the BufferedRegion has changed, user must call
   * SetInputImage again to update cached values. */
  virtual void SetInputImage( const InputImageType * ptr );

protected:
  GaussianDerivativeImageFunction();
  GaussianDerivativeImageFunction( const Self& ){};

  ~GaussianDerivativeImageFunction(){};

  void operator=( const Self& ){};
  void PrintSelf(std::ostream& os, Indent indent) const;

  void RecomputeGaussianKernel();
  void RecomputeContinuousGaussianKernel(
           const double* offset) const;


private:

  double                        m_Sigma[ImageDimension2];

  /** Array of 1D operators. Contains a derivative kernel and a gaussian kernel for
   *  each dimension */
  mutable OperatorArrayType     m_OperatorArray;
  mutable OperatorArrayType     m_ContinuousOperatorArray;

  /** OperatorImageFunction */
  OperatorImageFunctionPointer  m_OperatorImageFunction;
  double m_Extent[ImageDimension2];

  /** Flag to indicate whether to use image spacing */
  bool m_UseImageSpacing;

  /** Neighborhood Image Function */
  GaussianDerivativeFunctionPointer m_GaussianDerivativeFunction;
  GaussianFunctionPointer           m_GaussianFunction;

};

} // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_GaussianDerivativeImageFunction(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT GaussianDerivativeImageFunction< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef GaussianDerivativeImageFunction< ITK_TEMPLATE_2 x > \
                                                  GaussianDerivativeImageFunction##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkGaussianDerivativeImageFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkGaussianDerivativeImageFunction.txx"
#endif

#endif
