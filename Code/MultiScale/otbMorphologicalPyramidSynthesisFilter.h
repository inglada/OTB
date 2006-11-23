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
#ifndef _otbMorphologicalPyramidSynthesisFilter_h
#define _otbMorphologicalPyramidSynthesisFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/**
 * \class MorphologicalPyramidSynthesisFilter
 * \brief Recomposition of the multiscale analysis performed with 
 * a MorphologicalPyramidAnalyseFilter.
 *
 * Once a morphological pyramidal decompisition has been made, it is
 * possible to rebuild without any loss of information the original
 * image at full resolution. The MorphologicalPyramidsynthesisFilter
 * provides such a functionality. At each iteration, it perfoms the 
 * up-sampling of the current image using the 
 * otb::morphologicalPyramid::Resampler filter, the addition of 
 * the details lost in down-sampling, and the addition of the details 
 * lost in smoothing.
 *
 * \ingroup MultiScale
 * \sa MorphologicalPyramidAnalyseFilter
 * \sa MorphologicalPyramidResampler
 */
template <class TInputImage, class TOutputImage>
class MorphologicalPyramidSynthesisFilter
  : public ImageToImageListFilter<TInputImage,TOutputImage>
{
public:
/** Standard typedefs */
  typedef MorphologicalPyramidSynthesisFilter                  Self;
  typedef ImageToImageListFilter<TInputImage,TOutputImage>     Superclass;
  typedef itk::SmartPointer<Self>                              Pointer;
  typedef itk::SmartPointer<const Self>                        ConstPointer;  
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(MorphologicalPyramidSynthesisFilter,MorphologicalPyramidSynthesisFilter);
  /** Input image dimension constant macro */
  itkStaticConstMacro(InputImageDimension, unsigned int,TInputImage::ImageDimension);
  /** Output related typedefs */
  typedef typename Superclass::OutputImageType                          OutputImageType;
  typedef typename Superclass::OutputImageListType                      OutputImageListType;
  typedef typename OutputImageListType::Pointer                         OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType                   OutputImagePointerType;
  typedef typename Superclass::OutputImageListType::Iterator            ImageListIterator;
  typedef typename Superclass::OutputImageListType::ReverseIterator     ImageListReverseIterator;
  /** Input related typedefs */
  typedef typename Superclass::InputImageType                           InputImageType;          
  typedef typename Superclass::InputImageRegionType                     InputImageRegionType;
  typedef typename InputImageType::Pointer                              InputImagePointer;
  typedef typename InputImageType::ConstPointer                         InputImageConstPointer;
  typedef typename InputImageType::SizeType                             SizeType;                 
  typedef typename InputImageType::ValueType                            ValueType;  
  typedef typename InputImageType::PixelType                            PixelType;
  typedef typename InputImageType::SpacingType                          SpacingType;
  /** Size vector typedefs */
  typedef std::vector<SizeType>                                         SizeVectorType;
  typedef typename SizeVectorType::iterator                             SizeIterator;
  typedef typename SizeVectorType::reverse_iterator                     SizeReverseIterator;
  /** Details images lists setters */
  itkSetMacro(SupFiltre,OutputImageListPointerType);
  itkSetMacro(InfFiltre,OutputImageListPointerType);
  itkSetMacro(SupDeci,OutputImageListPointerType);
  itkSetMacro(InfDeci,OutputImageListPointerType);  
  /**
   * Set the vector of sizes
   * \param size The vector of sizes
   */
  virtual  void  SetSize(std::vector<typename TOutputImage::SizeType> size);    

protected:
  /** Constructor */
  MorphologicalPyramidSynthesisFilter();
  /** Destructor */
  ~MorphologicalPyramidSynthesisFilter();
  virtual void GenerateOutputInformation(){}; // does nothing
  /** Main computation method */
  virtual void GenerateData();
  /** Printself method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;  
   /** Sup details from filtering operations */
  OutputImageListPointerType m_SupFiltre;
  /** Inf details from filtering operations */
  OutputImageListPointerType m_InfFiltre;
  /** Sup details from subsampling operations */
  OutputImageListPointerType m_SupDeci;
  /** Inf details from subsampling operations */
  OutputImageListPointerType m_InfDeci;
  /** Sizes vector */
  SizeVectorType m_Size;
};
}// End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidSynthesisFilter.txx"
#endif

#endif
