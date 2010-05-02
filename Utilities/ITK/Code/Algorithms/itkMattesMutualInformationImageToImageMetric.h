/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMattesMutualInformationImageToImageMetric.h,v $
  Language:  C++
  Date:      $Date: 2009-11-02 18:46:33 $
  Version:   $Revision: 1.29 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMattesMutualInformationImageToImageMetric_h
#define __itkMattesMutualInformationImageToImageMetric_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
#include "itkOptMattesMutualInformationImageToImageMetric.h"
#else

#include "itkImageToImageMetric.h"
#include "itkCovariantVector.h"
#include "itkPoint.h"
#include "itkIndex.h"
#include "itkBSplineKernelFunction.h"
#include "itkBSplineDerivativeKernelFunction.h"
#include "itkCentralDifferenceImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkBSplineDeformableTransform.h"
#include "itkArray2D.h"

namespace itk
{

/** \class MattesMutualInformationImageToImageMetric
 * \brief Computes the mutual information between two images to be 
 * registered using the method of Mattes et al.
 *
 * MattesMutualInformationImageToImageMetric computes the mutual 
 * information between a fixed and moving image to be registered.
 *
 * This class is templated over the FixedImage type and the MovingImage 
 * type.
 *
 * The fixed and moving images are set via methods SetFixedImage() and
 * SetMovingImage(). This metric makes use of user specified Transform and
 * Interpolator. The Transform is used to map points from the fixed image to
 * the moving image domain. The Interpolator is used to evaluate the image
 * intensity at user specified geometric points in the moving image.
 * The Transform and Interpolator are set via methods SetTransform() and
 * SetInterpolator().
 *
 * If a BSplineInterpolationFunction is used, this class obtain
 * image derivatives from the BSpline interpolator. Otherwise, 
 * image derivatives are computed using central differencing.
 *
 * \warning This metric assumes that the moving image has already been
 * connected to the interpolator outside of this class. 
 *
 * The method GetValue() computes of the mutual information
 * while method GetValueAndDerivative() computes
 * both the mutual information and its derivatives with respect to the
 * transform parameters.
 *
 * The calculations are based on the method of Mattes et al [1,2]
 * where the probability density distribution are estimated using
 * Parzen histograms. Since the fixed image PDF does not contribute
 * to the derivatives, it does not need to be smooth. Hence, 
 * a zero order (box car) BSpline kernel is used
 * for the fixed image intensity PDF. On the other hand, to ensure
 * smoothness a third order BSpline kernel is used for the 
 * moving image intensity PDF.
 *
 * On Initialize(), the FixedImage is uniformly sampled within
 * the FixedImageRegion. The number of samples used can be set
 * via SetNumberOfSpatialSamples(). Typically, the number of
 * spatial samples used should increase with the image size.
 *
 * The option UseAllPixelOn() disables the random sampling and uses
 * all the pixels of the FixedImageRegion in order to estimate the 
 * joint intensity PDF.
 * 
 * During each call of GetValue(), GetDerivatives(),
 * GetValueAndDerivatives(), marginal and joint intensity PDF's
 * values are estimated at discrete position or bins. 
 * The number of bins used can be set via SetNumberOfHistogramBins().
 * To handle data with arbitray magnitude and dynamic range, 
 * the image intensity is scale such that any contribution to the
 * histogram will fall into a valid bin.
 *
 * One the PDF's have been contructed, the mutual information
 * is obtained by doubling summing over the discrete PDF values.
 *
 *
 * Notes: 
 * 1. This class returns the negative mutual information value.
 * 2. This class in not thread safe due the private data structures
 *     used to the store the sampled points and the marginal and joint pdfs.
 *
 * References:
 * [1] "Nonrigid multimodality image registration"
 *      D. Mattes, D. R. Haynor, H. Vesselle, T. Lewellen and W. Eubank
 *      Medical Imaging 2001: Image Processing, 2001, pp. 1609-1620.
 * [2] "PET-CT Image Registration in the Chest Using Free-form Deformations"
 *      D. Mattes, D. R. Haynor, H. Vesselle, T. Lewellen and W. Eubank
 *      IEEE Transactions in Medical Imaging. Vol.22, No.1, 
        January 2003. pp.120-128.
 * [3] "Optimization of Mutual Information for MultiResolution Image
 *      Registration"
 *      P. Thevenaz and M. Unser
 *      IEEE Transactions in Image Processing, 9(12) December 2000.
 *
 * \ingroup RegistrationMetrics
 * \ingroup ThreadUnSafe
 */
template <class TFixedImage,class TMovingImage >
class ITK_EXPORT MattesMutualInformationImageToImageMetric :
    public ImageToImageMetric< TFixedImage, TMovingImage >
{
public:

  /** Standard class typedefs. */
  typedef MattesMutualInformationImageToImageMetric           Self;
  typedef ImageToImageMetric< TFixedImage, TMovingImage >     Superclass;
  typedef SmartPointer<Self>                                  Pointer;
  typedef SmartPointer<const Self>                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MattesMutualInformationImageToImageMetric, ImageToImageMetric);

  /** Types inherited from Superclass. */
  typedef typename Superclass::TransformType            TransformType;
  typedef typename Superclass::TransformPointer         TransformPointer;
  typedef typename Superclass::TransformJacobianType    TransformJacobianType;
  typedef typename Superclass::InterpolatorType         InterpolatorType;
  typedef typename Superclass::MeasureType              MeasureType;
  typedef typename Superclass::DerivativeType           DerivativeType;
  typedef typename Superclass::ParametersType           ParametersType;
  typedef typename Superclass::FixedImageType           FixedImageType;
  typedef typename Superclass::MovingImageType          MovingImageType;
  typedef typename Superclass::FixedImageConstPointer   FixedImageConstPointer;
  typedef typename Superclass::MovingImageConstPointer  MovingImageCosntPointer;
  typedef typename Superclass::InputPointType           InputPointType;
  typedef typename Superclass::OutputPointType          OutputPointType;

  typedef typename Superclass::CoordinateRepresentationType
  CoordinateRepresentationType;

  /** Index and Point typedef support. */
  typedef typename FixedImageType::IndexType           FixedImageIndexType;
  typedef typename FixedImageType::OffsetValueType     OffsetValueType;
  typedef typename FixedImageIndexType::IndexValueType FixedImageIndexValueType;
  typedef typename MovingImageType::IndexType          MovingImageIndexType;
  typedef typename TransformType::InputPointType       FixedImagePointType;
  typedef typename TransformType::OutputPointType      MovingImagePointType;

  /** The moving image dimension. */
  itkStaticConstMacro( MovingImageDimension, unsigned int,
                       MovingImageType::ImageDimension );

  /** 
   *  Initialize the Metric by 
   *  (1) making sure that all the components are present and plugged 
   *      together correctly,
   *  (2) uniformly select NumberOfSpatialSamples within
   *      the FixedImageRegion, and 
   *  (3) allocate memory for pdf data structures. */
  virtual void Initialize(void) throw ( ExceptionObject );

  /** Get the derivatives of the match measure. */
  void GetDerivative( const ParametersType& parameters,
                      DerivativeType & Derivative ) const;

  /**  Get the value. */
  MeasureType GetValue( const ParametersType& parameters ) const;

  /**  Get the value and derivatives for single valued optimizers. */
  void GetValueAndDerivative( const ParametersType& parameters, 
                              MeasureType& Value,
                              DerivativeType& Derivative ) const;

  /** Number of spatial samples to used to compute metric */
  itkSetClampMacro( NumberOfSpatialSamples, unsigned long,
                    1, NumericTraits<unsigned long>::max() );
  itkGetConstReferenceMacro( NumberOfSpatialSamples, unsigned long); 

  /** Number of bins to used in the histogram. Typical value is 50. The minimum
   * value is 5 due to the padding required by the Parzen windowing with a
   * cubic-BSpline kernel. Note that even if the metric is used on binary images,
   * the number of bins should at least be equal to five. */
  itkSetClampMacro( NumberOfHistogramBins, unsigned long,
                    5, NumericTraits<unsigned long>::max() );
  itkGetConstReferenceMacro( NumberOfHistogramBins, unsigned long);   

  /** Reinitialize the seed of the random number generator that selects the
   * sample of pixels used for estimating the image histograms and the joint
   * histogram. By nature, this metric is not deterministic, since at each run
   * it may select a different set of pixels. By initializing the random number
   * generator seed to the same value you can restore determinism. On the other
   * hand, calling the method ReinitializeSeed() without arguments will use the
   * clock from your machine in order to have a very random initialization of
   * the seed. This will indeed increase the non-deterministic behavior of the
   * metric. */
  void ReinitializeSeed();
  void ReinitializeSeed(int);  

  /** Select whether the metric will be computed using all the pixels on the
   * fixed image region, or only using a set of randomly selected pixels. */ 
  itkSetMacro(UseAllPixels,bool);
  itkGetConstReferenceMacro(UseAllPixels,bool);
  itkBooleanMacro(UseAllPixels);

  /** This variable selects the method to be used for computing the Metric
   * derivatives with respect to the Transform parameters. Two modes of
   * computation are available. The choice between one and the other is a
   * trade-off between computation speed and memory allocations. The two modes
   * are described in detail below: 
   *
   * UseExplicitPDFDerivatives = True 
   * will compute the Metric derivative by first calculating the derivatives of
   * each one of the Joint PDF bins with respect to each one of the Transform
   * parameters and then accumulating these contributions in the final metric
   * derivative array by using a bin-specific weight.  The memory required for
   * storing the intermediate derivatives is a 3D array of doubles with size
   * equals to the product of (number of histogram bins)^2 times number of
   * transform parameters. This method is well suited for Transform with a small
   * number of parameters.
   *
   * UseExplicitPDFDerivatives = False will compute the Metric derivative by
   * first computing the weights for each one of the Joint PDF bins and caching
   * them into an array. Then it will revisit each one of the PDF bins for
   * computing its weighted contribution to the full derivative array. In this
   * method an extra 2D array is used for storing the weights of each one of
   * the PDF bins. This is an array of doubles with size equals to (number of
   * histogram bins)^2. This method is well suited for Transforms with a large
   * number of parameters, such as, BSplineDeformableTransforms. */
  itkSetMacro(UseExplicitPDFDerivatives,bool);
  itkGetConstReferenceMacro(UseExplicitPDFDerivatives,bool);
  itkBooleanMacro(UseExplicitPDFDerivatives);

  /** This boolean flag is only relevant when this metric is used along
   * with a BSplineDeformableTransform. The flag enables/disables the
   * caching of values computed when a physical point is mapped through
   * the BSplineDeformableTransform. In particular it will cache the
   * values of the BSpline weights for that points, and the indexes
   * indicating what BSpline-grid nodes are relevant for that specific
   * point. This caching is made optional due to the fact that the
   * memory arrays used for the caching can reach large sizes even for
   * moderate image size problems. For example, for a 3D image of
   * 256^3, using 20% of pixels, these arrays will take about 1
   * Gigabyte of RAM for storage. The ratio of computing time between
   * using the cache or not using the cache can reach 1:5, meaning that
   * using the caching can provide a five times speed up. It is
   * therefore, interesting to enable the caching, if enough memory is
   * available for it. The caching is enabled by default, in order to
   * preserve backward compatibility with previous versions of ITK. */
  itkSetMacro(UseCachingOfBSplineWeights,bool);
  itkGetConstReferenceMacro(UseCachingOfBSplineWeights,bool);
  itkBooleanMacro(UseCachingOfBSplineWeights);

protected:

  MattesMutualInformationImageToImageMetric();
  virtual ~MattesMutualInformationImageToImageMetric() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** \class FixedImageSpatialSample
   * A fixed image spatial sample consists of the fixed domain point 
   * and the fixed image value at that point. */
  /// @cond 
  class FixedImageSpatialSample
    {
    public:
    FixedImageSpatialSample():FixedImageValue(0.0)
      { FixedImagePointValue.Fill(0.0); }
    ~FixedImageSpatialSample() {};
  
    FixedImagePointType           FixedImagePointValue;
    double                        FixedImageValue;
    unsigned int                  FixedImageParzenWindowIndex;
    };
  /// @endcond 
  
  /** FixedImageSpatialSample typedef support. */
  typedef std::vector<FixedImageSpatialSample>  
  FixedImageSpatialSampleContainer;

  /** Container to store a set of points and fixed image values. */
  FixedImageSpatialSampleContainer    m_FixedImageSamples;

  /** Uniformly select a sample set from the fixed image domain. */
  virtual void SampleFixedImageDomain( 
    FixedImageSpatialSampleContainer& samples);

  /** Gather all the pixels from the fixed image domain. */
  virtual void SampleFullFixedImageDomain( 
    FixedImageSpatialSampleContainer& samples);

  /** Transform a point from FixedImage domain to MovingImage domain.
   * This function also checks if mapped point is within support region. */
  virtual void TransformPoint( unsigned int sampleNumber,
                               const ParametersType& parameters,
                               MovingImagePointType& mappedPoint,
                               bool& sampleWithinSupportRegion,
                               double& movingImageValue ) const;

private:

  //purposely not implemented
  MattesMutualInformationImageToImageMetric(const Self&); 
  //purposely not implemented
  void operator=(const Self&); 


  /** The marginal PDFs are stored as std::vector. */
  typedef float                       PDFValueType;
  typedef std::vector<PDFValueType>   MarginalPDFType;

  /** The fixed image marginal PDF. */
  mutable MarginalPDFType             m_FixedImageMarginalPDF;

  /** The moving image marginal PDF. */
  mutable MarginalPDFType             m_MovingImageMarginalPDF;

  /** Helper array for storing the values of the JointPDF ratios. */
  typedef double                      PRatioType;
  typedef Array2D< PRatioType >       PRatioArrayType;
  mutable PRatioArrayType             m_PRatioArray;

  /** Helper variable for accumulating the derivative of the metric. */
  mutable DerivativeType              m_MetricDerivative;

  /** Typedef for the joint PDF and PDF derivatives are stored as ITK Images. */
  typedef Image<PDFValueType,2>                 JointPDFType;
  typedef JointPDFType::IndexType               JointPDFIndexType;
  typedef JointPDFType::PixelType               JointPDFValueType;
  typedef JointPDFType::RegionType              JointPDFRegionType;
  typedef JointPDFType::SizeType                JointPDFSizeType;
  typedef Image<PDFValueType,3>                 JointPDFDerivativesType;
  typedef JointPDFDerivativesType::IndexType    JointPDFDerivativesIndexType;
  typedef JointPDFDerivativesType::PixelType    JointPDFDerivativesValueType;
  typedef JointPDFDerivativesType::RegionType   JointPDFDerivativesRegionType;
  typedef JointPDFDerivativesType::SizeType     JointPDFDerivativesSizeType;

  /** The joint PDF and PDF derivatives. */
  typename JointPDFType::Pointer                m_JointPDF;
  typename JointPDFDerivativesType::Pointer     m_JointPDFDerivatives;

  unsigned long                                 m_NumberOfSpatialSamples;
  unsigned long                                 m_NumberOfParameters;

  /** Variables to define the marginal and joint histograms. */
  unsigned long  m_NumberOfHistogramBins;
  double         m_MovingImageNormalizedMin;
  double         m_FixedImageNormalizedMin;
  double         m_MovingImageTrueMin;
  double         m_MovingImageTrueMax;
  double         m_FixedImageBinSize;
  double         m_MovingImageBinSize;

  /** Typedefs for BSpline kernel and derivative functions. */
  typedef BSplineKernelFunction<3>           CubicBSplineFunctionType;
  typedef BSplineDerivativeKernelFunction<3> CubicBSplineDerivativeFunctionType;

  /** Cubic BSpline kernel for computing Parzen histograms. */
  typename CubicBSplineFunctionType::Pointer   m_CubicBSplineKernel;
  typename CubicBSplineDerivativeFunctionType::Pointer 
  m_CubicBSplineDerivativeKernel;

  /** Precompute fixed image parzen window indices. */
  virtual void ComputeFixedImageParzenWindowIndices( 
                   FixedImageSpatialSampleContainer& samples );

  /**
   * Types and variables related to image derivative calculations.
   * If a BSplineInterpolationFunction is used, this class obtain
   * image derivatives from the BSpline interpolator. Otherwise, 
   * image derivatives are computed using central differencing.
   */
  typedef CovariantVector< double,
                           itkGetStaticConstMacro(MovingImageDimension) >
                                                         ImageDerivativesType;

  /** Compute image derivatives at a point. */
  virtual void ComputeImageDerivatives( const MovingImagePointType& mappedPoint,
                                        ImageDerivativesType& gradient ) const;

  /** Boolean to indicate if the interpolator BSpline. */
  bool m_InterpolatorIsBSpline;

  /** Typedefs for using BSpline interpolator. */
  typedef
  BSplineInterpolateImageFunction<MovingImageType,
                                  CoordinateRepresentationType> 
                                                      BSplineInterpolatorType;

  /** Pointer to BSplineInterpolator. */
  typename BSplineInterpolatorType::Pointer m_BSplineInterpolator;

  /** Typedefs for using central difference calculator. */
  typedef CentralDifferenceImageFunction<MovingImageType,
                                         CoordinateRepresentationType> 
                                                       DerivativeFunctionType;

  /** Pointer to central difference calculator. */
  typename DerivativeFunctionType::Pointer m_DerivativeCalculator;


  /** Compute PDF derivative contribution for each parameter. */
  virtual void ComputePDFDerivatives( unsigned int sampleNumber,
                                      int movingImageParzenWindowIndex,
                                      const ImageDerivativesType&
                                                movingImageGradientValue,
                                      double cubicBSplineDerivativeValue
                                      ) const;

  /**
   * Types and variables related to BSpline deformable transforms.
   * If the transform is of type third order BSplineDeformableTransform,
   * then we can speed up the metric derivative calculation by
   * only inspecting the parameters within the support region
   * of a mapped point.
   */

  /** Boolean to indicate if the transform is BSpline deformable. */
  bool m_TransformIsBSpline;

  /** The number of BSpline parameters per image dimension. */
  long m_NumParametersPerDim;

  /** 
   * The number of BSpline transform weights is the number of
   * of parameter in the support region (per dimension ). */   
  unsigned long m_NumBSplineWeights;

  /** The fixed image dimension. */
  itkStaticConstMacro( FixedImageDimension, unsigned int,
                       FixedImageType::ImageDimension );

  /** 
   * Enum of the deformabtion field spline order. 
   */
  enum { DeformationSplineOrder = 3 };

  /**
   * Typedefs for the BSplineDeformableTransform.
   */
  typedef BSplineDeformableTransform<
    CoordinateRepresentationType,
    ::itk::GetImageDimension<FixedImageType>::ImageDimension,
    DeformationSplineOrder> BSplineTransformType;
  typedef typename BSplineTransformType::WeightsType 
  BSplineTransformWeightsType;
  typedef typename BSplineTransformType::ParameterIndexArrayType 
  BSplineTransformIndexArrayType;

  /**
   * Variables used when transform is of type BSpline deformable.
   */
  typename BSplineTransformType::Pointer m_BSplineTransform;

  /**
   * Cache pre-transformed points, weights, indices and 
   * within support region flag.
   */
  typedef typename BSplineTransformWeightsType::ValueType    WeightsValueType;
  typedef          Array2D<WeightsValueType>                 BSplineTransformWeightsArrayType;
  typedef typename BSplineTransformIndexArrayType::ValueType IndexValueType;
  typedef          Array2D<IndexValueType>                   BSplineTransformIndicesArrayType;
  typedef          std::vector<MovingImagePointType>         MovingImagePointArrayType;
  typedef          std::vector<bool>                         BooleanArrayType;

  BSplineTransformWeightsArrayType      m_BSplineTransformWeightsArray;
  BSplineTransformIndicesArrayType      m_BSplineTransformIndicesArray;
  MovingImagePointArrayType             m_PreTransformPointsArray;
  BooleanArrayType                      m_WithinSupportRegionArray;
  
  typedef FixedArray<unsigned long, 
                     ::itk::GetImageDimension<FixedImageType>::ImageDimension>
                                                          ParametersOffsetType;
  ParametersOffsetType                  m_ParametersOffset;

  bool             m_UseAllPixels;

  virtual void PreComputeTransformValues();

  bool             m_ReseedIterator;
  int              m_RandomSeed;
  
  // Selection of explicit or implicit computation of PDF derivatives
  // with respect to Transform parameters.
  bool             m_UseExplicitPDFDerivatives;

  // Variables needed for optionally caching values when using a BSpline transform.
  bool                                    m_UseCachingOfBSplineWeights;
  mutable BSplineTransformWeightsType     m_Weights;
  mutable BSplineTransformIndexArrayType  m_Indices;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMattesMutualInformationImageToImageMetric.txx"
#endif

#endif

#endif
