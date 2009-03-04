/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFFTComplexToComplexImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-12-21 19:15:48 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFFTComplexToComplexImageFilter_h
#define __itkFFTComplexToComplexImageFilter_h


#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <complex>

namespace itk
{
/** \class FFTComplexToComplexImageFilter
 *
 * \brief Implements an API to enable the Fourier transform or the inverse
 * Fourier transform of images with complex valued voxels to be computed.
 *
 * \ingroup FourierTransform
 *
 * \author Simon K. Warfield simon.warfield@childrens.harvard.edu
 *
 * \note Attribution Notice. This research work was made possible by
 * Grant Number R01 RR021885 (PI Simon K. Warfield, Ph.D.) from
 * the National Center for Research Resources (NCRR), a component of the
 * National Institutes of Health (NIH).  Its contents are solely the
 * responsibility of the authors and do not necessarily represent the
 * official view of NCRR or NIH.
 *
 * This class was taken from the Insight Journal paper:
 * http://insight-journal.org/midas/handle.php?handle=1926/326
 *
 */
template < class TPixel, unsigned int NDimension = 3 >
class FFTComplexToComplexImageFilter :
    public ImageToImageFilter< Image< std::complex< TPixel > , NDimension >,
                               Image< std::complex< TPixel > , NDimension > >
{
public:
  /** Input and output image types. */
  typedef Image< std::complex< TPixel > , NDimension > InputImageType;
  typedef Image< std::complex< TPixel > , NDimension > OutputImageType;

  /** Standard class typedefs. */
  typedef FFTComplexToComplexImageFilter                        Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef SmartPointer<Self>                                    Pointer;
  typedef SmartPointer<const Self>                              ConstPointer;

  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension );

  /** Run-time type information (and related methods). */
  itkTypeMacro(FFTComplexToComplexImageFilter, ImageToImageFilter);

  /** Customized object creation methods that support configuration-based 
    * selection of FFT implementation.
    *
    * Default implementation is FFTW.
    */
  static Pointer New(void);

  /** Transform Direction */
  typedef enum 
    {
    DIRECT = 1,
    INVERSE
    }                                             TransformDirectionType;
    

  /** Image type typedef support. */
  typedef InputImageType                          ImageType;
  typedef typename ImageType::SizeType            ImageSizeType;

  /** Set/Get the direction in which the transform will be applied.
   * By selecting DIRECT, this filter will perform a direct Fourier Transform,
   * By selecting INVERSE, this filter will perform an inverse Fourier Transform,
   */
  itkSetMacro( TransformDirection, TransformDirectionType );
  itkGetMacro( TransformDirection, TransformDirectionType );

protected:
  FFTComplexToComplexImageFilter() {}
  virtual ~FFTComplexToComplexImageFilter(){}

  /** methods needed for the image filter pipeline */
  virtual void GenerateOutputInformation(); // figure out allocation for output image
  virtual void GenerateInputRequestedRegion();

  virtual bool FullMatrix() = 0; // must be implemented in child


private:
  FFTComplexToComplexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  TransformDirectionType                          m_TransformDirection;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTComplexToComplexImageFilter.txx"
#endif

#endif
