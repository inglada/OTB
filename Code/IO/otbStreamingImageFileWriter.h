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
#ifndef __otbStreamingImageFileWriter_h
#define __otbStreamingImageFileWriter_h

#include "otbMacro.h"
#include "itkImageIOBase.h"
#include "itkImageToImageFilter.h"
#include "otbStreamingManager.h"

namespace otb
{

/** \class StreamingImageFileWriter
 * \brief Writes image data to a single file with streaming process.
 *
 * StreamingImageFileWriter writes its input data to a single output file.
 * StreamingImageFileWriter interfaces with an ImageIO class to write out the
 * data whith streaming process.
 *
 * StreamingImageFileWriter will divide the output into several pieces
 * (controlled by SetNumberOfDivisionsStrippedStreaming, SetNumberOfLinesStrippedStreaming,
 * SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming or SetAutomaticTiledStreaming),
 * and call the upstream pipeline for each piece, tiling the individual outputs into one large
 * output. This reduces the memory footprint for the application since
 * each filter does not have to process the entire dataset at once.
 *
 * StreamingImageFileWriter will write directly the streaming buffer in the image file, so
 * that the output image never needs to be completely allocated
 *
 * \sa ImageFileWriter
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 */
template <class TInputImage>
class ITK_EXPORT StreamingImageFileWriter : public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingImageFileWriter                          Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingImageFileWriter, itk::ImageToImageFilter);

  /** Some typedefs for the input and output. */
  typedef TInputImage                            InputImageType;
  typedef typename InputImageType::Pointer       InputImagePointer;
  typedef typename InputImageType::RegionType    InputImageRegionType;
  typedef typename InputImageType::PixelType     InputImagePixelType;
  typedef TInputImage                            OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;
  typedef typename Superclass::DataObjectPointer DataObjectPointer;

  /** Dimension of input image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Streaming manager base class pointer */
  typedef StreamingManager<InputImageType>       StreamingManagerType;
  typedef typename StreamingManagerType::Pointer StreamingManagerPointerType;

  /**  Return the StreamingManager object responsible for dividing
   *   the region to write */
  StreamingManagerType* GetStreamingManager(void)
    {
    return m_StreamingManager;
    }

  /**  Set a user-specified implementation of StreamingManager
   *   used to divide the largest possible region in several divisions */
  void SetStreamingManager(StreamingManagerType* streamingManager)
    {
    m_StreamingManager = streamingManager;
    }

  /**  Set the streaming mode to 'stripped' and configure the number of strips
   *   which will be used to stream the image */
  void SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions);

  /**  Set the streaming mode to 'tiled' and configure the number of tiles
   *   which will be used to stream the image */
  void SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions);

  /**  Set the streaming mode to 'stripped' and configure the number of strips
   *   which will be used to stream the image with respect to a number of line
   *   per strip */
  void SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip);

  /**  Set the streaming mode to 'stripped' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option */
  void SetAutomaticStrippedStreaming(unsigned int availableRAM = 0);

  /**  Set the streaming mode to 'tiled' and configure the dimension of the tiles
   *   in pixels for each dimension (square tiles will be generated) */
  void SetTileDimensionTiledStreaming(unsigned int tileDimension);

  /**  Set the streaming mode to 'tiled' and configure the number of MB
   *   available. The actual number of divisions is computed automatically
   *   by estimating the memory consumption of the pipeline.
   *   Tiles will be square.
   *   Setting the availableRAM parameter to 0 means that the available RAM
   *   is set from the CMake configuration option */
  void SetAutomaticTiledStreaming(unsigned int availableRAM = 0);

  /**  Set buffer memory size (in bytes) use to calculate the number of stream divisions */
  itkLegacyMacro( void SetBufferMemorySize(unsigned long) );

  /**  Set the buffer number of lines use to calculate the number of stream divisions */
  itkLegacyMacro( void SetBufferNumberOfLinesDivisions(unsigned long) );

  /**  The number of stream divisions is calculate by using
   * OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING and
   * OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING cmake variables.
   */
  itkLegacyMacro( void SetAutomaticNumberOfStreamDivisions(void) );

  /** Set the tiling automatic mode for streaming division */
  itkLegacyMacro( void SetTilingStreamDivisions(void) );
  /** Choose number of divisions in tiling streaming division */
  itkLegacyMacro( void SetTilingStreamDivisions(unsigned long) );

  /** Return the string to indicate the method use to calculate number of stream divisions. */
  itkLegacyMacro( std::string GetMethodUseToCalculateNumberOfStreamDivisions(void) );

  /** Set the number of pieces to divide the input.  The upstream pipeline
   * will be executed this many times. */
  itkLegacyMacro( void SetNumberOfStreamDivisions(unsigned long) );

  /** Get the number of pieces to divide the input. The upstream pipeline
   * will be executed this many times. */
  itkLegacyMacro( unsigned long GetNumberOfStreamDivisions(void) );

  /** Override UpdateOutputData() from ProcessObject to divide upstream
   * updates into pieces. This filter does not have a GenerateData()
   * or ThreadedGenerateData() method.  Instead, all the work is done
   * in UpdateOutputData() since it must update a little, execute a little,
   * update some more, execute some more, etc. */
  virtual void UpdateOutputData(itk::DataObject * itkNotUsed(output));

  /** ImageFileWriter Methods */

  /** Specify the name of the output file to write. */
  itkGetStringMacro(FileName);

  /**
   * Set the filename and destroy the current driver.
   * \param filename the name of the file.
   */
  virtual void SetFileName(std::string filename)
  {
    m_FileName = filename;
    m_ImageIO = NULL;
    this->Modified();
  }

  /** Specify the region to write. If left NULL, then the whole image
   * is written. */
  void SetIORegion(const itk::ImageIORegion& region);
  itkGetConstReferenceMacro(IORegion, itk::ImageIORegion);

  /** Set the compression On or Off */
  itkSetMacro(UseCompression, bool);
  itkGetConstReferenceMacro(UseCompression, bool);
  itkBooleanMacro(UseCompression);

  /** By default the MetaDataDictionary is taken from the input image and
   *  passed to the ImageIO. In some cases, however, a user may prefer to
   *  introduce her/his own MetaDataDictionary. This is often the case of
   *  the ImageSeriesWriter. This flag defined whether the MetaDataDictionary
   *  to use will be the one from the input image or the one already set in
   *  the ImageIO object. */
  itkSetMacro(UseInputMetaDataDictionary, bool);
  itkGetConstReferenceMacro(UseInputMetaDataDictionary, bool);
  itkBooleanMacro(UseInputMetaDataDictionary);

  itkSetObjectMacro(ImageIO, itk::ImageIOBase);
  itkGetObjectMacro(ImageIO, itk::ImageIOBase);
  itkGetConstObjectMacro(ImageIO, itk::ImageIOBase);

  /**
   * Enable/disable writing of a .geom file with the ossim keyword list along with the written image
   */
  itkSetMacro(WriteGeomFile, bool);
  itkGetMacro(WriteGeomFile, bool);
  itkBooleanMacro(WriteGeomFile);

protected:
  StreamingImageFileWriter();
  virtual ~StreamingImageFileWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Does the real work. */
  virtual void GenerateData(void);

  virtual void GenerateInputRequestedRegion();


private:
  StreamingImageFileWriter(const StreamingImageFileWriter &); //purposely not implemented
  void operator =(const StreamingImageFileWriter&); //purposely not implemented

  void ObserveSourceFilterProgress(itk::Object* object, const itk::EventObject & event )
  {
    if (typeid(event) != typeid(itk::ProgressEvent))
      {
      return;
      }

    itk::ProcessObject* processObject = dynamic_cast<itk::ProcessObject*>(object);
    if (processObject)
      {
      m_DivisionProgress = processObject->GetProgress();
      }

    this->UpdateFilterProgress();
  }

  void UpdateFilterProgress()
  {
    this->UpdateProgress( (m_DivisionProgress + m_CurrentDivision) / m_NumberOfDivisions );
  }

  unsigned int m_NumberOfDivisions;
  unsigned int m_CurrentDivision;
  float m_DivisionProgress;

  /** ImageFileWriter Parameters */
  std::string m_FileName;

  itk::ImageIOBase::Pointer m_ImageIO;

  bool m_UserSpecifiedImageIO; //track whether the ImageIO is user specified

  itk::ImageIORegion m_IORegion;
  bool               m_UserSpecifiedIORegion; // track whether the region is user specified
  bool m_FactorySpecifiedImageIO; //track whether the factory mechanism set the ImageIO
  bool m_UseCompression;
  bool m_UseInputMetaDataDictionary; // whether to use the
                                     // MetaDataDictionary from the
                                     // input or not.
  
  bool m_WriteGeomFile;              // Write a geom file to store the kwl

  StreamingManagerPointerType m_StreamingManager;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageFileWriter.txx"
#endif

#endif
