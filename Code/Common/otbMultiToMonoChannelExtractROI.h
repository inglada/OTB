/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   24 janvier 2005
  Version   :   
  Role      :   Classe d'extraction d'une ROI d'une image multi-canal en pr�cisant le num�ro de canal a traiter
  $Id$

=========================================================================*/
#ifndef __otbMultiToMonoChannelExtractROI_h
#define __otbMultiToMonoChannelExtractROI_h

#include "otbExtractROIBase.h"
#include "itkImage.h"
#include "itkVectorImage.h"

#include "itkMacro.h"

namespace otb
{

/** \class MultiToMonoChannelExtractROI
 * \brief Extrait une partie d'un seul canal, d'une image multi-canal.
 *
 * Il est possible d'extraire tous les canaux de l'image ou seulement ceux pr�cis�s par l'utilisateur.
 * La m�thode SetChannel permet de s�lectionner le canal � traiter.
 * \note Si aucun canal n'est sp�cifi� par l'utilisateur, alors le premier canal est trait�.
 * \note Ces classe est param�tr� par le type de pixel des images d'entr�e et de sortie. 
 * L'image d'entr�e est une image de type ype "itk::VectorImage". L'image produite en sortie est de type "itk::Image".
 *
 */
template <class TInputPixelType, class TOutputPixelType, unsigned int VImageDimension=2>
class ITK_EXPORT MultiToMonoChannelExtractROI:
    public ExtractROIBase< itk::VectorImage<TInputPixelType,VImageDimension> , itk::Image<TOutputPixelType,VImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef MultiToMonoChannelExtractROI                Self;
  typedef ExtractROIBase< itk::VectorImage<TInputPixelType,VImageDimension> , itk::Image<TOutputPixelType,VImageDimension> > Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiToMonoChannelExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef itk::VectorImage<TInputPixelType,VImageDimension>     InputImageType;
  typedef itk::Image<TOutputPixelType,VImageDimension>          OutputImageType;

  /** Typedef to describe the output and input image region types. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::RegionType InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename InputImageType::PixelType InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename InputImageType::IndexType InputImageIndexType;
  typedef typename OutputImageType::SizeType OutputImageSizeType;
  typedef typename InputImageType::SizeType InputImageSizeType;

  /** Selectionne le canal a traiter */
  itkSetMacro(Channel,unsigned int);
  itkGetConstMacro(Channel,unsigned int);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

protected:
  MultiToMonoChannelExtractROI();
  ~MultiToMonoChannelExtractROI() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ExtractImageFilter can produce an image which is a different
   * resolution than its input image.  As such, ExtractImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** ExtractImageFilter peut etre implementee comme un filtre multithreaded.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  MultiToMonoChannelExtractROI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  /** Canal � traiter  [1...] */
  unsigned int  m_Channel;
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiToMonoChannelExtractROI.txx"
#endif
  
#endif
