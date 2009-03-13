/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapFilter.h,v $
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
#ifndef __itkLabelMapFilter_h
#define __itkLabelMapFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{
  
/** \class LabelMapFilter
 * \brief Base class for filters that take an image as input and overwrite that image as the output
 *
 * LabelMapFilter is the base class for all process objects whose
 * are using a LabelMapFilter as input. It manage several threads,
 * and run a method TreadedGenerateData() for each object in the LabelMapFilter.
 * With that class, the developer don't need to take care of iterating over all the objects in
 * the image, or to manage by hand the threads.
 * 
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapToLabelImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelMapFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapFilter  Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapFilter,ImageToImageFilter);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType; 
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType; 
  typedef typename OutputImageType::PixelType      OutputImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** LabelMapFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelMapFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
 protected:
  LabelMapFilter();
  ~LabelMapFilter();

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );

  virtual void ThreadedGenerateData( LabelObjectType * labelObject );

  /**
   * Return the label collection image to use. This method may be overloaded
   * if the label collection image to use is not the input image.
   */
  virtual InputImageType * GetLabelMap()
    {
    return static_cast<InputImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(0)));
    }

  typename FastMutexLock::Pointer m_LabelObjectContainerLock;

private:
  LabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typename InputImageType::LabelObjectContainerType::const_iterator m_LabelObjectIterator;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelMapFilter.txx"
#endif

#endif
