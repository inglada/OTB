/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageShapeModelEstimatorBase.h,v $
  Language:  C++
  Date:      $Date: 2009-01-24 20:02:56 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageShapeModelEstimatorBase_h
#define __itkImageShapeModelEstimatorBase_h

#include "itkImageToImageFilter.h"

namespace itk
{

/** \class ImageShapeModelEstimatorBase
 * \brief Base class for statistical shape model estimation.
 * 
 * itkImageShapeModelEstimatorBase is the base class for the
 * ImageShapeModelEstimator objects. It provides the basic function
 * definitions that are inherent to a ImageShapeModelEstimator objects.
 *
 * This is the SuperClass for the ImageShapeModelEstimator
 * framework. This is an abstract class defining an interface for all
 * such objects available through the ImageShapeModelEstimator
 * framework in the ITK toolkit.
 *
 * The basic functionality of the ImageShapeModelEstimator framework
 * base class is to generate the ShapeModels. It requires input image
 * to be provided by the user. EstimateShapeModels() is a pure virtual
 * function making this an abstract class. Classes deriving from this
 * class are required to implement the EstimateShapeModels function.
 *
 *  \ingroup ImageFeatureExtraction */

template <class TInputImage,
          class TOutputImage = Image<double, ::itk::GetImageDimension<TInputImage>::ImageDimension> >
class ITK_EXPORT ImageShapeModelEstimatorBase: 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageShapeModelEstimatorBase                  Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageShapeModelEstimatorBase,ImageToImageFilter);


  /** Type definitions for the training image. */
  typedef          TInputImage          InputImageType;
  typedef typename TInputImage::Pointer InputImagePointer;

protected:
  ImageShapeModelEstimatorBase();
  ~ImageShapeModelEstimatorBase();
  void PrintSelf(std::ostream& os, Indent indent) const;

  virtual void GenerateData();

private:

  ImageShapeModelEstimatorBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /**Container for holding the training image */
  InputImagePointer               m_InputImage;

  /** The core virtual function to perform ShapeModelling of the input data */
  virtual void EstimateShapeModels() = 0;

}; // class ImageShapeModelEstimator


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageShapeModelEstimatorBase.txx"
#endif

#endif
