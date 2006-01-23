/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   18 janvier 2005
  Version   :   
  Role      :   Classe d'extraction d'une ROI d'une image multi-canal en pr�cisant le(s) num�ro(s) de canal a traiter
  $Id$

=========================================================================*/
#ifndef __otbMultiChannelExtractROI_h
#define __otbMultiChannelExtractROI_h

#include "otbExtractROIBase.h"
#include "itkVectorImage.h"

#include "itkMacro.h"
#include <vector>

namespace otb
{

/** \class MultiChannelExtractROI
 * \brief Extrait une partie d'une image. Il est possible d'extraire tous les canaux de l'image ou 
 * seulement ceux pr�cis�s par l'utilisateur.
 * Cette classe s'appuie sur la classe "otb::ExtractROIBase"
 *
 */
template <class TInputPixelType, class TOutputPixelType, unsigned int VImageDimension=2>
class ITK_EXPORT MultiChannelExtractROI:
    public ExtractROIBase< itk::VectorImage<TInputPixelType,VImageDimension> , itk::VectorImage<TOutputPixelType,VImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelExtractROI                Self;
  typedef ExtractROIBase< itk::VectorImage<TInputPixelType,VImageDimension> , itk::VectorImage<TOutputPixelType,VImageDimension> > Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef itk::VectorImage<TInputPixelType,VImageDimension>      InputImageType;
  typedef itk::VectorImage<TOutputPixelType,VImageDimension>     OutputImageType;

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

  itkSetMacro(FirstChannel,unsigned int);
  itkGetConstMacro(FirstChannel,unsigned int);
  itkSetMacro(LastChannel,unsigned int);
  itkGetConstMacro(LastChannel,unsigned int);
  
  /** Typedef Liste des canaux */
  typedef std::vector<unsigned int> ChannelsType;
  /** Selectionne un canal a traiter */
  void SetChannel(unsigned int channel);
  /** Annule la selection des canaux */
  void ClearChannels(void);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

protected:
  MultiChannelExtractROI();
  ~MultiChannelExtractROI() {};
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
  MultiChannelExtractROI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  /** Premier/Dernier canal � traiter [1...] */
  unsigned int  m_FirstChannel;
  unsigned int  m_LastChannel;
  /** Liste des canaux � traiter  [1...] */
  ChannelsType  m_Channels; 
  /** Liste des canaux qui seront r�ellement trait�s [1...] */
  ChannelsType  m_ChannelsWorks;
  /** */
  bool m_ChannelsWorksBool;
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiChannelExtractROI.txx"
#endif
  
#endif
