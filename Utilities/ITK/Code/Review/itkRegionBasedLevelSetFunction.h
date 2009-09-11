/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegionBasedLevelSetFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-05-13 15:27:45 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkRegionBasedLevelSetFunction_h
#define __itkRegionBasedLevelSetFunction_h

#include "itkFiniteDifferenceFunction.h"
#include "itkRegularizedHeavisideStepFunction.h"
#include "vnl/vnl_matrix_fixed.h"

namespace itk {

/** \class RegionBasedLevelSetFunction
 *
 * \brief LevelSet function that computes a speed image based on regional integrals
 *
 * This class implements a level set function that computes the speed image by
 * integrating values on the image domain.
 *
 * Based on the paper:
 *
 *        "An active contour model without edges"
 *         T. Chan and L. Vese.
 *         In Scale-Space Theories in Computer Vision, pages 141–151, 1999.
 *
 * \author Mosaliganti K., Smith B., Gelas A., Gouaillard A., Megason S.
 *
 *  This code was taken from the Insight Journal paper:
 *
 *      "Cell Tracking using Coupled Active Surfaces for Nuclei and Membranes"
 *      http://www.insight-journal.org/browse/publication/642
 *      http://hdl.handle.net/10380/3055
 *
 *  That is based on the papers:
 *
 *      "Level Set Segmentation: Active Contours without edge"
 *      http://www.insight-journal.org/browse/publication/322
 *      http://hdl.handle.net/1926/1532
 *
 *      and
 *
 *      "Level set segmentation using coupled active surfaces"
 *      http://www.insight-journal.org/browse/publication/323
 *      http://hdl.handle.net/1926/1533
 *
 *
 */
template < class TInput, // LevelSetImageType
  class TFeature, // FeatureImageType
  class TSharedData >
class ITK_EXPORT RegionBasedLevelSetFunction: public
FiniteDifferenceFunction< TInput >
{
public:
  /** Standard class typedefs. */
  typedef RegionBasedLevelSetFunction                Self;
  typedef FiniteDifferenceFunction< TInput >         Superclass;
  typedef SmartPointer<Self>                         Pointer;
  typedef SmartPointer<const Self>                   ConstPointer;

  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  // itkNewMacro() is purposely not provided since this is an abstract class.

  /** Run-time type information (and related methods) */
  itkTypeMacro( RegionBasedLevelSetFunction, FiniteDifferenceFunction );

  /** Extract some parameters from the superclass. */
  typedef double                                          TimeStepType;
  typedef typename Superclass::ImageType                  ImageType;
  typedef typename Superclass::PixelType                  PixelType;
  typedef PixelType                                       ScalarValueType;
  typedef typename Superclass::RadiusType                 RadiusType;
  typedef typename Superclass::NeighborhoodType           NeighborhoodType;
  typedef typename Superclass::NeighborhoodScalesType     NeighborhoodScalesType;
  typedef typename Superclass::FloatOffsetType            FloatOffsetType;
  typedef FixedArray< ScalarValueType, itkGetStaticConstMacro(ImageDimension) >
                                                          VectorType;

  /* This structure is derived from LevelSetFunction and stores intermediate
  values for computing time step sizes */
  struct GlobalDataStruct
    {
    GlobalDataStruct()
      {
      ScalarValueType null_value = NumericTraits<ScalarValueType>::Zero;

      m_MaxCurvatureChange   = null_value;
      m_MaxGlobalChange      = null_value;
      }

    ~GlobalDataStruct() {}

//     ScalarValueType m_MaxAdvectionChange; // not used
//     ScalarValueType m_MaxPropagationChange; // not used
    ScalarValueType m_MaxCurvatureChange;

    vnl_matrix_fixed<ScalarValueType,
      itkGetStaticConstMacro(ImageDimension),
      itkGetStaticConstMacro(ImageDimension)> m_dxy;

    ScalarValueType m_dx[itkGetStaticConstMacro(ImageDimension)];

    ScalarValueType m_dx_forward[itkGetStaticConstMacro(ImageDimension)];
    ScalarValueType m_dx_backward[itkGetStaticConstMacro(ImageDimension)];

    ScalarValueType m_GradMagSqr;
    ScalarValueType m_MaxGlobalChange;
    };


  typedef TInput                                    InputImageType;
  typedef typename InputImageType::ConstPointer     InputImageConstPointer;
  typedef typename InputImageType::Pointer          InputImagePointer;
  typedef typename InputImageType::PixelType        InputPixelType;
  typedef typename InputImageType::IndexType        InputIndexType;
  typedef typename InputImageType::IndexValueType   InputIndexValueType;
  typedef typename InputImageType::SizeType         InputSizeType;
  typedef typename InputImageType::SizeValueType    InputSizeValueType;
  typedef typename InputImageType::RegionType       InputRegionType;
  typedef typename InputImageType::PointType        InputPointType;

  typedef TFeature                                  FeatureImageType;
  typedef typename FeatureImageType::ConstPointer   FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType      FeaturePixelType;
  typedef typename FeatureImageType::IndexType      FeatureIndexType;
  typedef typename FeatureImageType::OffsetType     FeatureOffsetType;

  typedef TSharedData                               SharedDataType;
  typedef typename SharedDataType::Pointer          SharedDataPointer;

  typedef HeavisideStepFunctionBase< InputPixelType, InputPixelType > HeavisideFunctionType;
  typedef typename HeavisideFunctionType::ConstPointer HeavisideFunctionConstPointer;

  void SetDomainFunction( const HeavisideFunctionType * f )
    {
    this->m_DomainFunction = f;
    }

  virtual void Initialize(const RadiusType &r)
    {
    this->SetRadius(r);

    // Dummy neighborhood.
    NeighborhoodType it;
    it.SetRadius( r );

    // Find the center index of the neighborhood.
    m_Center =  it.Size() / 2;

    // Get the stride length for each axis.
    for(unsigned int i = 0; i < ImageDimension; i++)
      {
      m_xStride[i] = it.GetStride(i);
      }
    }


  void SetSharedData( SharedDataPointer sharedDataIn )
    {
    this->m_SharedData = sharedDataIn;
    }

  void UpdateSharedData( bool forceUpdate );

  void *GetGlobalDataPointer() const
    {
    return new GlobalDataStruct;
    }

  TimeStepType ComputeGlobalTimeStep(void *GlobalData) const;

  /** Compute the equation value. */
  PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
    void *globalData, const FloatOffsetType& = FloatOffsetType(0.0));

  void SetInitialImage(InputImageType *f)
    {
    m_InitialImage = f;
    }

  virtual const FeatureImageType *GetFeatureImage() const
    { return m_FeatureImage.GetPointer(); }
  virtual void SetFeatureImage(const FeatureImageType *f)
    {    m_FeatureImage = f;  }

  /** Nu. Area regularization values */
  void SetAreaWeight( const ScalarValueType& nu)
    { this->m_AreaWeight = nu; }
  ScalarValueType GetAreaWeight() const
    { return this->m_AreaWeight; }

  /** Lambda1. Internal intensity difference weight */
  void SetLambda1( const ScalarValueType& lambda1 )
    { this->m_Lambda1 = lambda1; }
  ScalarValueType GetLambda1() const
    { return this->m_Lambda1; }

  /** Lambda2. External intensity difference weight */
  void SetLambda2( const ScalarValueType& lambda2 )
    { this->m_Lambda2 = lambda2; }
  ScalarValueType GetLambda2() const
    { return this->m_Lambda2; }

  /** Gamma. Overlap penalty */
  void SetOverlapPenaltyWeight( const ScalarValueType& gamma )
    { this->m_OverlapPenaltyWeight = gamma; }
  ScalarValueType GetOverlapPenaltyWeight() const
    { return this->m_OverlapPenaltyWeight; }

  /** Gamma. Scales all curvature weight values */
  virtual void SetCurvatureWeight(const ScalarValueType c)
    { m_CurvatureWeight = c; }
  ScalarValueType GetCurvatureWeight() const
    { return m_CurvatureWeight; }

  /** Weight of the laplacian smoothing term */
  void SetLaplacianSmoothingWeight(const ScalarValueType c)
    { m_LaplacianSmoothingWeight = c; }
  ScalarValueType GetLaplacianSmoothingWeight() const
    { return m_LaplacianSmoothingWeight; }

  /** Volume matching weight.  */
  void SetVolumeMatchingWeight( const ScalarValueType& tau )
    { this->m_VolumeMatchingWeight = tau; }
  ScalarValueType GetVolumeMatchingWeight() const
    { return this->m_VolumeMatchingWeight; }

  /** Pixel Volume = Number of pixels inside the level-set  */
  void SetVolume( const ScalarValueType& volume )
    { this->m_Volume = volume; }
  ScalarValueType GetVolume() const
    { return this->m_Volume; }

  /** Set function id.  */
  void SetFunctionId( const unsigned int& iFid )
    { this->m_FunctionId = iFid; }

  virtual void ReleaseGlobalDataPointer(void *GlobalData) const
  { delete (GlobalDataStruct *) GlobalData; }

  virtual ScalarValueType ComputeCurvatureTerm(const NeighborhoodType &,
    const FloatOffsetType &, GlobalDataStruct *gd );

  /** \brief Laplacian smoothing speed can be used to spatially modify the
    effects of laplacian smoothing of the level set function */
  virtual ScalarValueType LaplacianSmoothingSpeed(
    const NeighborhoodType &,
    const FloatOffsetType &, GlobalDataStruct * = 0) const
    { return NumericTraits<ScalarValueType>::One; }

  /** \brief Curvature speed can be used to spatially modify the effects of
    curvature . The default implementation returns one. */
  virtual ScalarValueType CurvatureSpeed(const NeighborhoodType &,
                                         const FloatOffsetType &, GlobalDataStruct * = 0
                                         ) const
    { return NumericTraits<ScalarValueType>::One; }

protected:

  RegionBasedLevelSetFunction();
  virtual ~RegionBasedLevelSetFunction() {}

  /** The initial level set image */
  InputImageConstPointer m_InitialImage;

  /** The feature image */
  FeatureImageConstPointer m_FeatureImage;

  SharedDataPointer                 m_SharedData;
  HeavisideFunctionConstPointer     m_DomainFunction;

  /** Area regularization weight */
  ScalarValueType           m_AreaWeight;

  /** Internal functional of the level set weight */
  ScalarValueType           m_Lambda1;

  /** External functional of the level set weight */
  ScalarValueType           m_Lambda2;

  /** Overlap Penalty Weight */
  ScalarValueType           m_OverlapPenaltyWeight;

  /** Volume Regularization Weight */
  ScalarValueType           m_VolumeMatchingWeight;

  /** Volume Constraint in pixels */
  ScalarValueType           m_Volume;

  /** Curvature Regularization Weight */
  ScalarValueType           m_CurvatureWeight;

  /** Laplacian Regularization Weight */
  ScalarValueType           m_LaplacianSmoothingWeight;

  unsigned int              m_FunctionId;

  std::slice x_slice[itkGetStaticConstMacro(ImageDimension)];
  ::size_t m_Center;
  ::size_t m_xStride[itkGetStaticConstMacro(ImageDimension)];

  static double m_WaveDT;
  static double m_DT;

  void ComputeHImage();

  /** \brief Compute the global term as a combination of the internal, external,
    overlapping and volume regularization terms.  */
  ScalarValueType ComputeGlobalTerm(
    const ScalarValueType& imagePixel,
    const InputIndexType& inputIndex );

  /** \brief Compute the internal term
  \param[in] iValue Feature Image Value
  \param[in] iIdx Feature Image Index
  \param[in] fId Index of the LevelSet Function  */
  virtual ScalarValueType ComputeInternalTerm(const FeaturePixelType& iValue,
    const FeatureIndexType& iIdx ) = 0;

  /** \brief Compute the external term
  \param[in] iValue Feature Image Value
  \param[in] iIdx Feature Image Index
  \param[in] pr Product of Heaviside Functions
  \note after discussion with kishore, pr is not and unsigned int */
  virtual ScalarValueType ComputeExternalTerm(const FeaturePixelType& iValue,
    const FeatureIndexType& iIdx ) = 0;

  /** \brief Compute the overlap term
  \param[in] featIndex
  \param[out] pr = \f$ \prod_{i \neq j} H(\phi_i)\f$
  \return OverlapTerm = \f$ \sum_{i \neq j} H(\phi_i)\f$ */
  virtual ScalarValueType ComputeOverlapParameters( const FeatureIndexType& featIndex,
    ScalarValueType& pr ) = 0;

  /** \brief Compute the overlap term
      \return \f$ \int_{p \in \Omega} H(\phi_i) dp - this->Volume \f$
      \note the volume regularization does not depend on the spacing.
        So the volume must be set in number of pixels (not in real world unit). */
  ScalarValueType ComputeVolumeRegularizationTerm( );

  /** \brief Compute the laplacian term
      \return \f$ \Delta \phi - \div(\frac{\nabla \phi}{|\nabla \phi|}) \f$
      \verbatim For details see
  @inproceedings{bb30335,
  AUTHOR = "Li, C.M. and Xu, C.Y. and Gui, C. and Fox, M.D.",
  TITLE = "Level Set Evolution without Re-Initialization: A New Variational Formulation",
  BOOKTITLE = CVPR05,
  YEAR = "2005",
  PAGES = "I: 430-436"}
  \endverbatim  */
  ScalarValueType ComputeLaplacianTerm( const NeighborhoodType &,
    const FloatOffsetType &, GlobalDataStruct *gd  );

  /** \brief Compute the laplacian
  \return \f$ \Delta \phi \f$ */
  ScalarValueType ComputeLaplacian( GlobalDataStruct *gd );

  /** \brief Compute Hessian Matrix */
  void ComputeHessian( const NeighborhoodType &it,
    GlobalDataStruct *globalData );

  /** \brief Compute Parameters for the inner and outer parts. */
  virtual void ComputeParameters() = 0;

  /** \brief Update and save the inner and outer parameters in the shared data
    data structure. */
  virtual void UpdateSharedDataParameters() = 0;

private:
  RegionBasedLevelSetFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRegionBasedLevelSetFunction.txx"
#endif

#endif
