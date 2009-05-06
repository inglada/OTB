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
#ifndef __otbLineSegmentDetector_h
#define __otbLineSegmentDetector_h


#include "otbImageToLineSpatialObjectListFilter.h"
#include "otbLineSpatialObjectList.h"
#include "otbImage.h"
#include "itkPointSet.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkGradientImageFilter.h"


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
            return static_cast<TOutputPixel>(2*vcl_sqrt(input[0]*input[0] + input[1]*input[1] ));
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
            TOutputPixel resp = static_cast<TOutputPixel>(vcl_atan2(input[0],-input[1]));
            
             if (resp< itk::NumericTraits<TOutputPixel>::Zero)
               {
                 resp = -resp;
               }
            
            return resp;
          }
      };
  }// end namespace Functor

/** \class LineSegmentDetector
 *  \brief this class implement a fast line detector with false detection control using 
 *         the a contrario method
 *  
 *  \sa ImageLineSegmentDetector (streamed version)
 *
 *  See Publication : " LSD: A line segment detector ", R. Grompone, J.Jackubowicz, J-M.Morel, G.Randall 
 * 
 */

template <class TInputImage,class TPrecision = double>
class ITK_EXPORT LineSegmentDetector :
      public otb::ImageToLineSpatialObjectListFilter< TInputImage >
{
  public:
  
  /** typedef for the classes standards. */
  typedef LineSegmentDetector                               Self;
  typedef ImageToLineSpatialObjectListFilter< TInputImage>  Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  
  /** Method for management of the object factory. */
  itkNewMacro(Self);
  
  /** Return the name of the class. */
  itkTypeMacro(LineSegmentDetector,ImageToLineSpatialObjectListFilter );

  /** Definition of the input image and the output ObjectList*/
  typedef TInputImage                                                   InputImageType;
  typedef typename InputImageType::PixelType                            InputPixelType;
  typedef typename InputImageType::IndexType                            InputIndexType;
  typedef typename InputImageType::SizeType                             SizeType;

  /** Definition of the list of lines. */
  typedef LineSpatialObjectList                                         LineSpatialObjectListType;
  typedef LineSpatialObjectListType::Pointer                            LineSpatialObjectListPointer;
  typedef LineSpatialObjectListType::LineType                           LineSpatialObjectType;
  typedef LineSpatialObjectType::PointListType                          PointListType;
  typedef LineSpatialObjectType::LinePointType                          PointType;
  

  /** Definition of temporary image ised to store LABELS*/
  typedef Image<TPrecision ,2>                                          OutputImageType;
  typedef typename OutputImageType::PixelType                           OutputPixelType;
  typedef typename OutputImageType::IndexType                           OutputIndexType;
  typedef typename OutputImageType::SizeType                            OutputSizeType;
  
  /** Histogram to store the coordinate of ordered pixels*/
  typedef std::vector<OutputIndexType>                                  IndexVectorType;
  typedef typename IndexVectorType::iterator                            IndexVectorIteratorType;
  typedef std::vector<IndexVectorType >                                 CoordinateHistogramType;
  typedef typename CoordinateHistogramType::iterator                    CoordinateHistogramIteratorType;

   
  /** typedef structure to store REGION*/
  typedef std::vector<IndexVectorType>                                  VectorOfIndexVectorType;
  typedef std::vector<float>                                            DirectionVectorType;
  typedef typename DirectionVectorType::iterator                        DirectionVectorIteratorType; 

  /** */ 
  typedef itk::GradientRecursiveGaussianImageFilter<OutputImageType > GradientFilterType;
  //typedef itk::GradientImageFilter<InputImageType > GradientFilterType;
  typedef typename GradientFilterType::Pointer GradientFilterPointerType;
  typedef typename GradientFilterType::OutputImageType GradientOutputImageType;


  typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,OutputImageType,
  Functor::MagnitudeFunctor<typename GradientOutputImageType::PixelType,TPrecision> > MagnitudeFilterType;
  typedef typename MagnitudeFilterType::Pointer                                   MagnitudeFilterPointerType;
  typedef typename MagnitudeFilterType::OutputImageType::PixelType                MagnitudePixelType;
  typedef typename MagnitudeFilterType::OutputImageType                           MagnitudeImageType;
            
  typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,OutputImageType,
  Functor::OrientationFunctor<typename GradientOutputImageType::PixelType,TPrecision> > OrientationFilterType;
  typedef typename OrientationFilterType::Pointer OrientationFilterPointerType;
  typedef typename OrientationFilterType::OutputImageType                         OutputImageDirType;
  typedef typename OutputImageDirType::RegionType                                 OutputImageDirRegionType;

  /** Create an image to store the label USED(1) or notUsed (0)*/
  typedef otb::Image<unsigned char, 2>                              LabelImageType;
  typedef typename LabelImageType::Pointer                          LabelImagePointerType;
  
  /** Vector to store the rectangle characteization  center, width, orientation ,( begin ,end ) of the central line*/
  typedef std::vector<double>                                       RectangleType;
  typedef typename RectangleType::iterator                          RectangleIteratorType;
  typedef std::vector< RectangleType>                               RectangleListType;
  typedef typename RectangleListType::iterator                      RectangleListTypeIterator; 


  itkSetMacro(ImageSize, SizeType);
  itkGetMacro(ImageSize, SizeType);

protected:
  LineSegmentDetector();
  virtual ~LineSegmentDetector() {};

  /** Before Generate Data method*/
  virtual void BeforeGenerateData();
  /** Generate Data method*/
  virtual void GenerateData();

  /** Sort the image and store the coordinates in a histogram
   *  this method is used to determine the seeds where to begin the search segments
   *  Points with large gradient modulus are more able to belong to a segment
   */
  virtual  CoordinateHistogramType SortImageByModulusValue(OutputImageType * modulusImage);

  /** */
  virtual void LineSegmentDetection(CoordinateHistogramType * CoordinateHistogram);
  
  /** */
  virtual bool IsUsed(InputIndexType  index);
  
   /** Set Pixel flag to USED*/
  virtual void SetPixelToUsed(InputIndexType  index);
  
 
  /** search for a segment which begins from a seed "index "*/
  virtual void GrowRegion(InputIndexType  index);
  
  /** Define if two are aligned */
  virtual bool IsAligned(double Angle, double regionAngle, double prec);
  
  /** For each region of the region List it builds a rectangle */
  virtual int ComputeRectangles();
  
  /** */
  virtual void Region2Rect(IndexVectorType  region , double angleRegion);
  
  /** */
  virtual double ComputeRegionOrientation(IndexVectorType  region , double x, double y , double angleRegion);

  /** */
  virtual double angle_diff(double a, double b);

  /**  Compute the Number Of False Alarm for a rectangle*/
  virtual double ComputeRectNFA(RectangleType  rec);

  /** */
  virtual double ImproveRectangle(RectangleType  * rec);

  /** NFA For a rectangle*/
  virtual double NFA(int n, int k, double p, double logNT);
  
  /** Create a copy of a rectangle*/
  virtual void CopyRectangle(RectangleType * rDst , RectangleType  *rSrc );
  

  /** Rutines from numerical recipes*/
  virtual double betacf(double a, double b, double x);
  virtual double gammln(double xx);
  virtual double betai(double a, double b, double x);
  virtual double factln(int n);

  /** Printself method*/
  void PrintSelf(std::ostream& os, itk::Indent indent) const;



private:
  LineSegmentDetector(const Self&);  //purposely not implemented
  void operator=(const Self&);      //purposely not implemented

  VectorOfIndexVectorType           m_RegionList;
  DirectionVectorType               m_DirectionVector;
  LabelImagePointerType             m_UsedPointImage;
  RectangleListType                 m_RectangleList;
  
  double                             m_Threshold;
  double                             m_Prec;
  double                             m_DirectionsAllowed;
  unsigned int                       m_MinimumRegionSize;
  unsigned int                       m_NumberOfImagePixels;

  /** The image size has to be the LargestPosssibleRegion in the case
   * (streaming uses (streaming decorator).
   */
  SizeType                           m_ImageSize;
  
  /** Gradient filter */
  GradientFilterPointerType m_GradientFilter;

  /** Magnitude filter */
  MagnitudeFilterPointerType m_MagnitudeFilter;

  /** Orientation filter */
  OrientationFilterPointerType m_OrientationFilter;

  /** Output*/
  LineSpatialObjectListPointer      m_LineList;

  

  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSegmentDetector.txx"
#endif


#endif

