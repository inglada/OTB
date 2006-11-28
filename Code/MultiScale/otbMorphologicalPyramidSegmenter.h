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
#ifndef _otbMorphologicalPyramidSegmenter_h
#define _otbMorphologicalPyramidSegmenter_h

#include "itkImageToImageFilter.h"

namespace otb
{
  namespace morphologicalPyramid
    {
      /**
       * \class Segmenter
       * \brief This class performs the segmentation of a detail image extracted from a 
       * morphological pyramid analysis.
       * 
       * This class performs the segmentation of a detail image extracted from a 
       * morphological pyramid analysis.
       * 
       * The Segmentation is perfomed using the ConnectedThresholdImageFilter. The seeds
       * are extracted from the image using the ImageToPointSetFilter. The thresolds are set
       * by using quantiles computed with the HistogramGenerator.
       * 
       * \sa MorphologicalPyramidAnalyseFilter
       * \sa MorphologicalPyramidSynthesisFilter
       * \sa ResampleImageFilter
       * \sa LinearInterpolateImageFunction
       * \sa ScaleTransform
       */
      template <class TInputImage, class TOutputImage>
	class  Segmenter
	: public itk::ImageToImageFilter<TInputImage,TOutputImage>
	{
	  public :
	    /** Standard typedefs */ 
	    typedef Segmenter                                       Self;
	  typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
	  typedef itk::SmartPointer<Self>                           Pointer;
	  typedef itk::SmartPointer<const Self>                     ConstPointer;
	  /** Creation through object factory macro */
	  itkNewMacro(Self);
	  /** Type macro */
	  itkTypeMacro(Segmenter,ImageToImageFilter);
	  /** Template parameters typedefs */
	  typedef TInputImage                           InputImageType;
	  typedef typename InputImageType::PixelType    InputPixelType;
	  typedef typename InputImageType::Pointer      InputImagePointerType;
	  typedef TOutputImage                          OutputImageType;
	  typedef typename OutputImageType::Pointer     OutputImagePointerType;
	  typedef typename OutputImageType::PixelType   OutputPixelType;
	  typedef typename OutputImageType::RegionType  OutputImageRegionType;
	  typedef typename OutputImageType::SizeType    SizeType;
	  typedef typename OutputImageType::SpacingType SpacingType;

	  /** ImageDimension constants */
	  itkStaticConstMacro(DetailsImageDimension, unsigned int,
			      TInputImage::ImageDimension);
	  itkStaticConstMacro(OriginalImageDimension, unsigned int,
			      TInputImage::ImageDimension);
	  itkStaticConstMacro(OutputImageDimension, unsigned int,
			      TOutputImage::ImageDimension);
	  /**
	   * Set the details image.
	   * \param detailsImage The details image from the morphological pyramid
	   */
	  void SetDetailsImage(const InputImageType * detailsImage);
	  /**
	   * Set the details image.
	   * \return detailsImage The input details image.
	   */
	  InputImageType * GetDetailsImage(void);
	  /**
	   * Set the original image.
	   * \param originalImage The original image to segment.
	   */
	  void SetOriginalImage(const InputImageType * originalImage);
	  /**
	   * Get the original image.
	   * \return originalImage The original image to segment.
	   */
	  InputImageType * GetOriginalImage(void); 

	  /** Min object size parameter accessor */
	  itkSetMacro(MinimumObjectSize,unsigned long);
	  itkGetMacro(MinimumObjectSize,unsigned long);
	  /** Quantile for seeds extraction */
	  itkSetMacro(SeedsQuantile,float);
	  itkGetMacro(SeedsQuantile,float);
	  /** Quantile for connected thresholding */
	  itkSetMacro(ConnectedThresholdQuantile,float);
	  itkGetMacro(ConnectedThresholdQuantile,float);
	  /** Toogle the darker detail segmentation */
	  itkSetMacro(SegmentDarkDetailsBool,bool);
	  itkGetMacro(SegmentDarkDetailsBool,bool);
	  itkBooleanMacro(SegmentDarkDetailsBool);
	  /** Get the number of segmented objects */
	  itkGetMacro(NumberOfObjects,OutputPixelType);

	protected:
	  /** Constructor */
	  Segmenter();
	  /** Destructor */
	  ~Segmenter() {};
	  /** Main computation method */
	  void GenerateData(void);
	  /** PrintSelf method */
	  void PrintSelf(std::ostream& os, itk::Indent indent) const;
	  /**
	   * Configure the input datas.
	   */
	  void GenerateInputRequestedRegion(void);
	  /**
	   * Configure the output data.
	   */
	  void EnlargeOutputRequestedRegion(void);

	  private :
	    Segmenter(const Self&); // purposely not implemented
	  void operator=(const Self&); // purposely not implemented
	  /** Minimum size for the segmented object */
	  unsigned long  m_MinimumObjectSize;
	  /** Quantile for seeds determination */
	  float m_SeedsQuantile;
	  /** Quantile to set the connectedThresholdFilter threshold */
	  float m_ConnectedThresholdQuantile;
	  /** Set to true if the details to segment are darker than background */
	  bool m_SegmentDarkDetailsBool;
	  /** Number of segmented objects */
	  OutputPixelType m_NumberOfObjects;
	};
    } // End namespace morphologicalPyramid
}// End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidSegmenter.txx"
#endif

#endif
