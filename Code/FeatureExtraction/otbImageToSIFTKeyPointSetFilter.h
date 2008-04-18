/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbImageToSIFTKeyPointSetFilter_h
#define _otbImageToSIFTKeyPointSetFilter_h

#include "itkExpandImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkShrinkImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkVector.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientImageFilter.h"

#include "otbImageToPointSetFilter.h"
#include "otbImageList.h"

namespace otb
{

  namespace Functor
    {
      /** \class MagnitudeFunctor
       *  \brief This functor computes the magnitude of a covariant vector.
       */
      template <class TInputPixel,class TOutputPixel>
	class MagnitudeFunctor
	{
	public:
	  
	  inline TOutputPixel operator()(const TInputPixel& input)
	    {
	      return vcl_sqrt(input[0]*input[0]+input[1]*input[1]);
	    }
	};
      
      /** \class OrientationFunctor
       *  \brief This functor computes the orientation of a cavariant vector<br>
       *   Orientation values lies between 0 and 2*Pi.
       */
      template <class TInputPixel,class TOutputPixel>
	class OrientationFunctor
	{
	public:
	  
	  inline TOutputPixel operator()(const TInputPixel& input)
	    {
	      TOutputPixel resp = vcl_atan2(input[1],input[0]);
	      if(resp<0)
		{
		  resp+=2*M_PI;
		}

	      return resp;
	    }
	};
    }// end namespace Functor
  
  /** \class ImageToSIFTKeyPointSetFilter
   *  \brief This class extracts key points from an input image, trough a pyramidal decomposition.
   *
   * This class implements the SIFT key point detector proposed by David G. Lowe in proceedings of International
   * Conference on Computer Vision, Corfu, september 1999.
   *
   * \li Input image is downsample by factor 2
   * \li Pyramid of gaussians is obtained by filtering input image with
   *     variable length sigma, each separated by a constant k = 2^(1/S)
   *     where S is the number of scales
   * \li Difference of gaussian are compute with the difference of two
   *     adjacent gaussian image results
   * \li Detector is laucnched on each difference of gaussian to find key point
   * \li A key point is a local minimum or a local maximum on 3*3 neighborhood
   * \li An interpolated location of the maximum key point is compute with fitting
   *     a 3D quadratic function
   * \li DoG low constrast are discarded (DoG < 3%)
   * \li High edge responses key point are discarded
   *
   * Selected key points are stored in a itk::PointSet structure.
   * Data points contains a list of (magnitude, main orientation) for each level
   * of the pyramidal decomposition.
   * 
   * Orientation is expressed in degree in the range [0,360] with a precision of 10 degrees.
   *
   */
  template <class TInputImage, class TOutputPointSet>
    class ITK_EXPORT ImageToSIFTKeyPointSetFilter
    : public ImageToPointSetFilter<TInputImage,TOutputPointSet>
    {
    public:
      /** Standard typedefs */
      typedef ImageToSIFTKeyPointSetFilter                       Self;
      typedef ImageToPointSetFilter<TInputImage,TOutputPointSet> Superclass;
      typedef itk::SmartPointer<Self>                            Pointer;
      typedef itk::SmartPointer<const Self>                      ConstPointer;
  
      /** Creation through object factory macro */
      itkNewMacro(Self);  

      /** Type macro */
      itkTypeMacro(ImageToSIFTKeyPointSetFilter,ImageToPointSetFilter);
  
      /** Template parameters typedefs */
      typedef TInputImage InputImageType;
      typedef typename TInputImage::Pointer InputImagePointerType;
      typedef typename TInputImage::PixelType PixelType;
      
      typedef TOutputPointSet OutputPointSetType;
      typedef typename TOutputPointSet::Pointer OutputPointSetPointerType;
      typedef typename TOutputPointSet::PixelType OutputPixelType;
      typedef typename TOutputPointSet::PointType OutputPointType;
      typedef typename TOutputPointSet::PointIdentifier OutputPointIdentifierType;
      
      typedef itk::Vector<PixelType,3> VectorPointType;
      
      /** Set/Get the number of octaves */
      itkSetMacro(OctavesNumber, unsigned int);
      itkGetMacro(OctavesNumber, unsigned int);
      
      /** Set/Get the number of scales */
      itkSetMacro(ScalesNumber, unsigned int);
      itkGetMacro(ScalesNumber, unsigned int);

      /** Set/Get the expand factors */
      itkSetMacro(ExpandFactors, unsigned int);
      itkGetMacro(ExpandFactors, unsigned int);

      /** Set/Get the shrink factors */
      itkSetMacro(ShrinkFactors, unsigned int);
      itkGetMacro(ShrinkFactors, unsigned int);
      
      /** Set/Get the sigma 0 */
      itkSetMacro(Sigma0, double);
      itkGetMacro(Sigma0, double);

      /** Set/Get the Difference of gaussian threshold
       * eliminating low contrast key point
       */
      itkSetMacro(DoGThreshold, double);
      itkGetMacro(DoGThreshold, double);
      
      /** Set/Get Edgethreshold
       *  Eliminating edge responses
       */
      itkSetMacro(EdgeThreshold, double);
      itkGetMacro(EdgeThreshold, double);
      
      /** Internal typedefs */
      typedef itk::ExpandImageFilter<TInputImage, TInputImage> ExpandFilterType;
      typedef typename ExpandFilterType::Pointer ExpandFilterPointerType;

      typedef itk::ShrinkImageFilter<InputImageType, InputImageType> ShrinkFilterType;
      typedef typename ShrinkFilterType::Pointer ShrinkFilterPointerType;
      
      typedef itk::RecursiveGaussianImageFilter<InputImageType,InputImageType> GaussianFilterType;
      typedef typename GaussianFilterType::Pointer GaussianFilterPointerType;
      
      typedef otb::ImageList<InputImageType> ImageListType;
      typedef typename ImageListType::Pointer ImageListPointerType;
      
      typedef itk::SubtractImageFilter<InputImageType,InputImageType,InputImageType> SubtractFilterType;
      typedef typename SubtractFilterType::Pointer SubtractFilterPointerType;
      
      typedef itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
      typedef typename NeighborhoodIteratorType::NeighborhoodType NeighborhoodType;
      typedef typename NeighborhoodType::OffsetType OffsetType;
      
      typedef itk::MinimumMaximumImageCalculator<InputImageType> MinimumMaximumCalculatorType;
      typedef typename MinimumMaximumCalculatorType::Pointer MinimumMaximumCalculatorPointerType;
      
      typedef itk::GradientImageFilter<InputImageType,PixelType,PixelType> GradientFilterType;      
      typedef typename GradientFilterType::Pointer GradientFilterPointerType;
      typedef typename GradientFilterType::OutputImageType GradientOutputImageType;
      
      typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,InputImageType,
	Functor::MagnitudeFunctor<typename GradientOutputImageType::PixelType,typename InputImageType::PixelType> > MagnitudeFilterType;
      typedef typename MagnitudeFilterType::Pointer MagnitudeFilterPointerType;
      
      typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,InputImageType,
	Functor::OrientationFunctor<typename GradientOutputImageType::PixelType,typename InputImageType::PixelType> > OrientationFilterType;
      typedef typename OrientationFilterType::Pointer OrientationFilterPointerType;
      
    protected:
      /** Actually process the input */
      virtual void GenerateData();
      
      /** Constructor */
      ImageToSIFTKeyPointSetFilter();
      
      /** Destructor */
      virtual ~ImageToSIFTKeyPointSetFilter() {}
      
      /** PrintSelf method */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

      /** Initialize input image */
      void InitializeInputImage();
      
      /** Compute differenec of gaussian
       * 
       *  \param input, current input in process
       */
      void ComputeDifferenceOfGaussian(InputImagePointerType input);
      
      /** Localize key point */
      void DetectKeyPoint( const unsigned int octave );
      
      /** Check local extremum for 26 neighbors (current and adjacents scales)
       *
       *  \param currentScale
       *  \param previousScale
       *  \param nextScale
       *
       *  \return true if the pixel is extremum
       */
      bool IsLocalExtremum( const NeighborhoodIteratorType& currentScale,
			    const NeighborhoodIteratorType& previousScale,
			    const NeighborhoodIteratorType& nextScale ) const;
      
      /** Refine location key point
       *  
       *  \param currentScale
       *  \param previousScale
       *  \param nextScale
       *  \param offset pixel location
       *
       *  \return true if key point is rejected, false otherwise
       */
      bool RefineLocationKeyPoint( const NeighborhoodIteratorType& currentScale,
				   const NeighborhoodIteratorType& previousScale,
				   const NeighborhoodIteratorType& nextScale,
				   const PixelType& maximumDoG,
				   VectorPointType& solution);
      
      /** Compute key point orientation
       * \param currentScale iterator pixel
       * \param scale current scale
       */
      void ComputeKeyPointDescriptor(const NeighborhoodIteratorType& currentScale,
				     const unsigned int scale,
				     const PixelType translation,
				     PixelType& magitude,
				     PixelType& orientation);
      
    private:
      ImageToSIFTKeyPointSetFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      /** Number of octaves */
      unsigned int m_OctavesNumber;
      
      /** Number of scale for each octave */
      unsigned int m_ScalesNumber;
      
      /** Expand factors */
      unsigned int m_ExpandFactors;

      /** Shrink factors */
      unsigned int m_ShrinkFactors;
      
      /** Threshold DoG */
      double m_DoGThreshold;

      /** Edge Threshold */
      double m_EdgeThreshold;
      
      /** Ratio threshold compute */
      double m_RatioEdgeThreshold;
      
      /** Sigma 0 */
      typename GaussianFilterType::ScalarRealType m_Sigma0;
      
      /** Sigma k */
      double m_Sigmak;
      
      /** Expand filter */
      ExpandFilterPointerType m_ExpandFilter;
      
      /** Shrink filter */
      ShrinkFilterPointerType m_ShrinkFilter;
      
      /** Gaussian filter */
      GaussianFilterPointerType m_XGaussianFilter;
      GaussianFilterPointerType m_YGaussianFilter;
      
      /** Gaussian image pointer */
      InputImagePointerType m_LastGaussian;
      
      /** Difference of gaussian list */
      ImageListPointerType m_DoGList;
      
      /** Magnitude image list */
      ImageListPointerType m_MagnitudeList;
      
      /** Orientation image list */
      ImageListPointerType m_OrientationList;
      
      /** Gaussian weight orientation list */
      ImageListPointerType m_GaussianWeightOrientationList;
      
      /** Subtract filter */
      SubtractFilterPointerType m_SubtractFilter;

      /** Gradient filter */
      GradientFilterPointerType m_GradientFilter;
      
      /** Magnitude filter */
      MagnitudeFilterPointerType m_MagnitudeFilter;

      /** Orientation filter */
      OrientationFilterPointerType m_OrientationFilter;

      /** Gaussian orientation filter */
      GaussianFilterPointerType m_XOrientationGaussianFilter;
      GaussianFilterPointerType m_YOrientationGaussianFilter;

      /** Number of key points */
      OutputPointIdentifierType m_ValidatedKeyPoints;
      
      /** Number of key point which offset is larger than 0.5 per octave*/
      unsigned int m_DifferentSamplePoints;
      
      /** Number of discarded key points */
      unsigned int m_DiscardedKeyPoints;
      
      /** Number of change sample max */
      unsigned int m_ChangeSamplePointsMax;

      /** Gaussian sigma for histogram smoothing */
      static const double m_HistogramGaussianWeights[73];
      
      /** Offsets vector, conveniance datas */
      static const OffsetType m_Offsets[8];
    };
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToSIFTKeyPointSetFilter.txx"
#endif

#endif
