/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAutoCropLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/07/11 14:56:38 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAutoCropLabelMapFilter_h
#define __itkAutoCropLabelMapFilter_h

#include "itkChangeRegionLabelMapFilter.h"

namespace itk
{
  
/** \class AutoCropLabelMapFilter
 * \brief Crop a LabelMap image to fit exactly the objects in the LabelMap
 *
 * The CropBorder can be used to add a border which will never be larger than
 * the input image. To add a border of size independant of the input image,
 * PadLabelMapFilter can be used.
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa PadLabelMapFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template <class TInputImage>
class ITK_EXPORT AutoCropLabelMapFilter : public ChangeRegionLabelMapFilter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef AutoCropLabelMapFilter  Self;
  typedef ChangeRegionLabelMapFilter<TInputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(AutoCropLabelMapFilter, ChangeRegionImageFilter);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::OutputImagePointer OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType OutputImagePixelType;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename InputImageType::RegionType     InputImageRegionType; 
  typedef typename InputImageType::PixelType      InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;

  typedef typename InputImageType::PixelType       PixelType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef typename InputImageType::SizeType        SizeType;
  typedef typename InputImageType::RegionType      RegionType;
  
  typedef TInputImage TOutputImage;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);


  /**
   * Set/Get the boder added to the mask before the crop. The default is 0 on
   * all the axes.
   */
  itkSetMacro(CropBorder, SizeType);
  itkGetConstReferenceMacro(CropBorder, SizeType);

 protected:
  AutoCropLabelMapFilter();
  ~AutoCropLabelMapFilter() {};

  virtual void GenerateOutputInformation();

private:
  AutoCropLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  SizeType m_CropBorder;
  TimeStamp m_CropTimeStamp;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAutoCropLabelMapFilter.txx"
#endif

#endif
