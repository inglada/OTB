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
#ifndef __otbImageToCarvingPathFilter_h
#define __otbImageToCarvingPathFilter_h

#include "otbImageToPathFilter.h"
#include "itkImageSliceConstIteratorWithIndex.h"

namespace otb {
/**
 * \class ImageToCarvingPathFilter
 * \brief This filter find the minimum energy path of an image.
 *
 * This filter find the minimum energy path from top to bottom of
 * the image (if Direction is set to 0) or from left to right
 * (if direction is set to 1).
 *
 * This is one component necessary to apply the seam carving algorithm
 * published by Shai Avidan, Ariel Shamir,
 * Seam Carving for Content-Aware Image Resizing,
 * ACM Transactions on Graphics, Volume 26, Number 3,
 * SIGGRAPH 2007
 *
 * \par INPUTS
 * The input is an image, typically the output of a gradient filter.
 *
 * \par OUTPUTS
 * The output is
 * a polyline parametric path comprising one and only one pixel per line
 * (resp. column) if direction is 0 (resp. 1).
 *
 * \sa RemoveCarvingPathFilter
 *
 * \ingroup PathFilters
 * \ingroup ImageFilters
 */


template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToCarvingPathFilter
  : public ImageToPathFilter<TInputImage, TOutputPath>
{
public:
  /** standards typedefs */
  typedef ImageToCarvingPathFilter                 Self;
  typedef ImageToPathFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  /// Creation througth the object factory
  itkNewMacro(Self);
  /// Runtime information
  itkTypeMacro(ImageToCarvingPathFilter,ImageToPathFilter);

  /// Template parameters typedef
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointerType InputImagePointerType;
  typedef typename Superclass::OutputPathType        OutputPathType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;

  typedef typename InputImageType::PixelType     PixelType;



  /** Set and Get foreground value */
  itkSetMacro(ForegroundValue,PixelType);
  itkGetConstMacro(ForegroundValue,PixelType);

  /** Set and Get direction value */
  itkSetMacro(Direction ,unsigned int);
  itkGetConstMacro(Direction ,unsigned int);

  /** Get EnergyPerPix value */
  itkGetConstMacro(EnergyPerPix ,double);

protected:
  ImageToCarvingPathFilter();
  virtual ~ImageToCarvingPathFilter(){};
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  virtual void GenerateOutputInformation() {}; //does nothing
  virtual void GenerateData();

private:
  ImageToCarvingPathFilter(const Self&); // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  PixelType m_ForegroundValue;
  unsigned int m_Direction;
  double m_EnergyPerPix;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToCarvingPathFilter.txx"
#endif

#endif
