//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for TiffOverviewBuilder
// 
//*******************************************************************
//  $Id: ossimTiffOverviewBuilder.cpp 18073 2010-09-13 15:55:48Z dburken $

#include <algorithm> /* for std::fill */
// #include <cstring>
#include <sstream>
using namespace std;
#include <xtiffio.h>

#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/parallel/ossimMpiMasterOverviewSequencer.h>
#include <ossim/parallel/ossimMpiSlaveOverviewSequencer.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageMetaData.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/support_data/ossimGeoTiff.h>

RTTI_DEF1(ossimTiffOverviewBuilder,
          "ossimTiffOverviewBuilder",
          ossimOverviewBuilderBase)

static ossimTrace traceDebug("ossimTiffOverviewBuilder:degug");

// Property keywords.
static const char COPY_ALL_KW[] = "copy_all_flag";

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimTiffOverviewBuilder.cpp 18073 2010-09-13 15:55:48Z dburken $";
#endif


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTiffOverviewBuilder::ossimTiffOverviewBuilder()
   :
      ossimOverviewBuilderBase(),
      m_imageHandler(0),
      m_outputFile(ossimFilename::NIL),
      m_nullDataBuffer(0),
      m_bytesPerPixel(1),
      m_bitsPerSample(8),
      m_tileWidth(0),
      m_tileHeight(0),
      m_tileSizeInBytes(0),
      m_sampleFormat(0),
      m_currentTiffDir(0),
      m_tiffCompressType(COMPRESSION_NONE),
      m_jpegCompressQuality(DEFAULT_COMPRESS_QUALITY),
      m_resampleType(ossimFilterResampler::ossimFilterResampler_BOX),
      m_nullPixelValues(),
      m_copyAllFlag(false),
      m_outputTileSizeSetFlag(false)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::ossimTiffOverviewBuilder DEBUG:\n";
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  "
         << OSSIM_ID
         << "\n";
#endif
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "overview stop dimension: " << m_overviewStopDimension
         << std::endl;
   }
}

ossimTiffOverviewBuilder::~ossimTiffOverviewBuilder()
{
   m_imageHandler = 0;
   m_alphaMask.clear();
}

void ossimTiffOverviewBuilder::setResampleType(
   ossimFilterResampler::ossimFilterResamplerType resampleType)
{
   m_resampleType = resampleType;
}

bool ossimTiffOverviewBuilder::buildOverview(
   const ossimFilename& overview_file, bool copy_all)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::buildOverview DEBUG:"
         << "\noverview file: " << overview_file.c_str()
         << "\ncopy_all flag: " << (copy_all?"true":"false")
         << std::endl;
   }


   m_outputFile  = overview_file;
   m_copyAllFlag = copy_all;

   return execute();
}


bool ossimTiffOverviewBuilder::execute()
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::execute";
   
   if (theErrorStatus == ossimErrorCodes::OSSIM_ERROR)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError status has been previously set!  Returning..."
         << std::endl;
      return false;
   }

   if ( !m_imageHandler )
   {
      return false;
   }

   m_outputFile = getOutputFile();
   if (m_outputFile == ossimFilename::NIL)
   {
      return false;
   }

   // Check the file.  Disallow same file overview building.
   if (m_imageHandler->getFilename() == m_outputFile)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Source image file and overview file cannot be the same!"
         << std::endl;
      return false;
   }

   // Add .tmp in case process gets aborted to avoid leaving bad .ovr file.
   ossimFilename outputFileTemp = m_outputFile + ".tmp";

   // Required number of levels needed including r0.
   ossim_uint32 requiedResLevels = getRequiredResLevels(m_imageHandler.get());

   // Zero based starting resLevel.
   ossim_uint32 startingResLevel  = 0;
   if (!m_copyAllFlag)
   {
      startingResLevel = m_imageHandler->getNumberOfDecimationLevels();
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE
         << "\nCurrent number of reduced res sets: "
         << m_imageHandler->getNumberOfDecimationLevels()
         << "\nNumber of required reduced res sets:  " << requiedResLevels
         << "\nStarting reduced res set:    " << startingResLevel
         << "\nResampling type:  " << getOverviewType().c_str()
         << std::endl;
   }

   if ( startingResLevel >= requiedResLevels )
   {
      ossimNotify(ossimNotifyLevel_INFO)
         << MODULE << " NOTICE:"
         << "\nImage has required reduced resolution data sets.\nReturning..."
         << std::endl;
      return false;
   }
   
   ossimStdOutProgress* progressListener = 0; // Only used on master.
   TIFF* tif = 0;                             // Only used on master.

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   setPercentComplete(0.0);

   if (ossimMpi::instance()->getRank() == 0 )
   {
      ossim_uint64 fourGigs = (static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(1024)*
                               static_cast<ossim_uint64>(4));
      ossimIrect bounds = m_imageHandler->getBoundingRect();
      ossim_uint64 byteCheck = (static_cast<ossim_uint64>(bounds.width())*
                                static_cast<ossim_uint64>(bounds.height())*
                                static_cast<ossim_uint64>(m_imageHandler->getNumberOfOutputBands())*
                                static_cast<ossim_uint64>(ossim::scalarSizeInBytes(m_imageHandler->getOutputScalarType())));
      ossimString openMode = "w";
      if((byteCheck*static_cast<ossim_uint64>(2))>fourGigs)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << " Big tiff activated\n";
         }
         openMode += "8";
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << " No big tiff activated\n";
         }
      }
      
      //---
      // See if the file can be opened for writing.
      // Note:  If this file existed previously it will be overwritten.
      //---
      tif = openTiff(outputFileTemp, openMode);
      if (!tif)
      {
         // Set the error...
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
                       "%s file %s line %d\nCannot open file:  %s",
                       MODULE,
                       __FILE__,
                       __LINE__,
                       outputFileTemp.c_str());      
         return false;
      } 

      //---
      // Check for a listeners.  If the list is empty, add a standard out
      // listener so that command line apps like img2rr will get some progress.
      //---
      ossimStdOutProgress* progressListener = 0;
      if (theListenerList.empty())
      {
         progressListener = new ossimStdOutProgress(0, true);
         addListener(progressListener);
      }

      if (startingResLevel == 0)
      {       
         if (!writeR0(tif))
         {
            // Set the error...
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_ERROR,
                          "File %s line %d\nError copying image!",
                          __FILE__,
                          __LINE__);
            closeTiff(tif);
            if (progressListener)
            {
               removeListener(progressListener);
               delete progressListener;
               progressListener = 0;
            }
            return false;
         }

         ++startingResLevel; // Go to r1.
      }

      if (needsAborting())
      {
         closeTiff(tif);
         if (progressListener)
         {
            removeListener(progressListener);
            delete progressListener;
            progressListener = 0;
         }
         return false;
      }
      
      TIFFFlush(tif);
      
   } // End of master only write of r0.
        
   for (ossim_uint32 i = startingResLevel; i < requiedResLevels; ++i)
   {
      if (ossimMpi::instance()->getRank() == 0 )
      {
         TIFFFlush(tif);
      }
      // Sync all processes...
      ossimMpi::instance()->barrier();
      
      ossimRefPtr<ossimImageHandler> ih = 0;

      //---
      // If we copied r0 to the overview file use it instead of the
      // original image handler as it is probably faster.
      //---
      if ( i <= m_imageHandler->getNumberOfDecimationLevels())
      {
         ih = m_imageHandler;
      }
      else
      {
         ih = ossimImageHandlerRegistry::instance()->open(outputFileTemp);
         if (!ih)
         {
            // Set the error...
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_OPEN_FILE_ERROR,
                          "%s file %s line %d\nCannot open file:  %s",
                          MODULE,
                          __FILE__,
                          __LINE__,
                          outputFileTemp.c_str());   
            return false;
         }
      }
      
      if ( !writeRn( ih.get(), tif, i, (i==startingResLevel) ) )
      {
         // Set the error...
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_WRITE_FILE_ERROR,
                       "\
%s file %s line %d\nError creating reduced res set!",
                       MODULE,
                       __FILE__,
                       __LINE__);
         ih = 0;
         if (tif)
         {
            closeTiff(tif);
            tif = 0;
         }
         if (progressListener)
         {
            removeListener(progressListener);
            delete progressListener;
            progressListener = 0;
         }
         return false;
      }
      
      if (needsAborting())
      {
         ih = 0;
         if (tif)
         {
            closeTiff(tif);
            tif = 0;
         }
         if (progressListener)
         {
            removeListener(progressListener);
            delete progressListener;
            progressListener = 0;
         }
         return false;
      }
      
      ih = 0;
   }

   if (ossimMpi::instance()->getRank() == 0 )
   {
      if (tif)
      {
         closeTiff(tif);
         tif = 0;
      }
      // Remove the listener if we had one.
      if (progressListener)
      {
         removeListener(progressListener);
         delete progressListener;
         progressListener = 0;
      }
      
      outputFileTemp.rename(m_outputFile);
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_INFO)
            << "Wrote file:  " << m_outputFile.c_str() << std::endl;
      }
      ossimFilename file=m_outputFile;
      file = file.setExtension("omd");
      ossimKeywordlist kwl;
      if(file.exists())
      {
         kwl.addFile(file.c_str());
         
      }
      ossimImageMetaData metaData(m_imageHandler->getOutputScalarType(),
                                  m_imageHandler->getNumberOfInputBands());
      
      uint32 i= 0;
      
      for(i = 0; i < metaData.getNumberOfBands(); ++i)
      {
         metaData.setMinPix(i,  m_imageHandler->getMinPixelValue(i));
         metaData.setMaxPix(i,  m_imageHandler->getMaxPixelValue(i));
         metaData.setNullPix(i, m_imageHandler->getNullPixelValue(i));
      }
      metaData.saveState(kwl);
      kwl.write(file.c_str());
     
      setCurrentMessage(ossimString("Finished..."));
   }

   
   return true;
}

bool ossimTiffOverviewBuilder::writeR0(TIFF* tif)
{
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeR0";

   ossimIrect rect = m_imageHandler->getImageRectangle();

   if (!setTags(tif, rect, 0))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing tags!" << std::endl;
      return false;
   }
   
   // Set the geotiff tags.
   if ( setGeotiffTags(m_imageHandler->getImageGeometry().get(),
                       m_imageHandler->getBoundingRect(),
                       0,
                       tif) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << MODULE << " NOTICE: geotiff tags not set." << std::endl;
      } 
   }
   
   ossim_int32 samples         = m_imageHandler->getNumberOfSamples();
   ossim_int32 lines           = m_imageHandler->getNumberOfLines();
   ossim_int32 tilesWide       = samples % m_tileWidth ?
                           samples / m_tileWidth + 1 : samples / m_tileWidth;
   ossim_int32 tilesHigh       = lines % m_tileHeight ?
                           lines / m_tileHeight + 1 : lines / m_tileHeight;
   ossim_int32 numberOfTiles   = tilesWide * tilesHigh;

   int tileNumber = 0;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::writeR0 DEBUG:"
         << "\nsamples:        " << samples
         << "\nlines:          " << lines
         << "\ntilesWide:      " << tilesWide
         << "\ntilesHigh:      " << tilesHigh
         << "\nnumberOfTiles:  " << numberOfTiles
         << std::endl;
   }

   setCurrentMessage(ossimString("Copying r0..."));
   
   //***
   // Tile loop in the line direction.
   //***
   for(int i = 0; i < tilesHigh; ++i)
   {
      ossimIpt origin(0, 0);
      origin.y = i * m_tileHeight;
      
      //***
      // Tile loop in the sample (width) direction.
      //***
      for(int j = 0; j < tilesWide; ++j)
      {
         origin.x = j * m_tileWidth;

         ossimRefPtr<ossimImageData> t =
            m_imageHandler->getTile(ossimIrect(origin.x,
                                                origin.y,
                                                origin.x +(m_tileWidth-1),
                                                origin.y +(m_tileHeight-1)));
	 
         //***
         // Band loop.
         //***
         for (uint32 band=0;
              band<m_imageHandler->getNumberOfInputBands();
              ++band)
         {
            tdata_t data;
            
            if ( t.valid() && (t->getDataObjectStatus() != OSSIM_NULL) )
            {
               // Grab a pointer to the tile for the band.
               data = static_cast<tdata_t>(t->getBuf(band));
            }
            else
            {
               data = static_cast<tdata_t>(&(m_nullDataBuffer.front()));
            }

            // Write the tile.
            int bytesWritten = 0;
            bytesWritten = TIFFWriteTile(tif,
                                         data,
                                         origin.x,
                                         origin.y,
                                         0,        // z
                                         band);    // sample

            if (bytesWritten != m_tileSizeInBytes)
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE << " ERROR:"
                  << "Error returned writing tiff tile:  " << i
                  << "\nExpected bytes written:  " << m_tileSizeInBytes
                  << "\nBytes written:  " << bytesWritten
                  << std::endl;
               theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
               return false;
            }

         } // End of band loop.  
        
         ++tileNumber;

      } // End of tile loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double tile = tileNumber;
         double numTiles = numberOfTiles;
         setPercentComplete(tile / numTiles * 100.0);
      }

   } // End of tile loop in the line (height) direction.

   //***
   // Write the current dirctory.
   //***
   if (!TIFFWriteDirectory(tif))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing directory!" << std::endl;
      return false;
   }

   ++m_currentTiffDir;

   return true;
}

bool ossimTiffOverviewBuilder::writeRn(ossimImageHandler* imageHandler,
                                       TIFF* tif,
                                       ossim_uint32 resLevel,
                                       bool firstResLevel)
{
   //---
   // Set up the sequencer.  This will be one of three depending on if we're
   // running mpi and if we are a master process or a slave process.
   //---
   ossimRefPtr<ossimOverviewSequencer> sequencer;
   
   if(ossimMpi::instance()->getNumberOfProcessors() > 1)
   {
      if ( ossimMpi::instance()->getRank() == 0 )
      {
         sequencer = new ossimMpiMasterOverviewSequencer();
      }
      else
      {
         sequencer = new ossimMpiSlaveOverviewSequencer();
      }
   }
   else
   {
      sequencer = new ossimOverviewSequencer();
   }

   sequencer->setImageHandler(imageHandler);

   ossim_uint32 sourceResLevel = imageHandler->getNumberOfDecimationLevels() - 1;

   sequencer->setSourceLevel(sourceResLevel);
   sequencer->setResampleType(m_resampleType);
   sequencer->setTileSize( ossimIpt(m_tileWidth, m_tileHeight) );
   
   if ( firstResLevel && ( getHistogramMode() != OSSIM_HISTO_MODE_UNKNOWN) )
   {
      // Accumulate a histogram.  Can't do with mpi/multi-process.
      if(ossimMpi::instance()->getNumberOfProcessors() == 1)
      {
         sequencer->setHistogramMode(getHistogramMode());
      }
      //---
      // else{} Not sure if we want an error thrown here.  For now will handle at the
      // application level.
      //---
   }
   sequencer->initialize();

   // If we are a slave process start the resampling of tiles.
   if (ossimMpi::instance()->getRank() != 0 )
   {
      sequencer->slaveProcessTiles();
      return true;
   }

   //---
   // The rest of the method on master node only.
   //---
   static const char MODULE[] = "ossimTiffOverviewBuilder::writeRn";

   ostringstream os;
   os << "creating r" << resLevel << "...";
   setCurrentMessage(os.str());

   if (resLevel == 0)
   {
      return false;
   }

   ossimIrect rect;
   sequencer->getOutputImageRectangle(rect);

   if (!setTags(tif, rect, resLevel))
   {
      closeTiff(tif);
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing tags!" << std::endl;
      return false;
   }

   if ( !m_copyAllFlag && (resLevel == 1) )
   {
      //---
      // Set the geotif tags for the first layer.
      // Note this is done in writeR0 method if m_copyAllFlag is set.
      //---
      if ( setGeotiffTags(m_imageHandler->getImageGeometry().get(),
                          ossimDrect(rect),
                          resLevel,
                          tif) == false )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_NOTICE)
               << MODULE << " NOTICE: geotiff tags not set." << std::endl;
         } 
      }
   }

   ossim_uint32 outputTilesWide = sequencer->getNumberOfTilesHorizontal();
   ossim_uint32 outputTilesHigh = sequencer->getNumberOfTilesVertical();
   ossim_uint32 numberOfTiles   = sequencer->getNumberOfTiles();
   ossim_uint32 tileNumber      = 0;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::writeRn DEBUG:"
         << "\noutputTilesWide:  " << outputTilesWide
         << "\noutputTilesHigh:  " << outputTilesHigh
         << "\nnumberOfTiles:    " << numberOfTiles
         << std::endl;
   }
 
   // Tile loop in the line direction.
   ossim_uint32 y = 0;

   if (resLevel == 1 && m_maskBuildFlag)
   {
      m_alphaMask.clear();
      openMaskFile();
   }

   for(ossim_uint32 i = 0; i < outputTilesHigh; ++i)
   {
      // Tile loop in the sample (width) direction.
      ossim_uint32 x = 0;
      for(ossim_uint32 j = 0; j < outputTilesWide; ++j)
      {
         // Grab the resampled tile.
         ossimRefPtr<ossimImageData> t = sequencer->getNextTile();
         if (t.valid() && ( t->getDataObjectStatus() != OSSIM_NULL ) )
         {
            // Write it to the tiff.
            for (ossim_uint32 band = 0; band < t->getNumberOfBands(); ++band)
            {
               // Write the tile.
               int bytesWritten = 0;
               bytesWritten = TIFFWriteTile(tif,
                                            t->getBuf(band),
                                            x,
                                            y,
                                            0,        // z
                                            band);    // sample
               
               if (bytesWritten != m_tileSizeInBytes)
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE << " ERROR:"
                     << "Error returned writing tiff tile:  " << i
                     << "\nExpected bytes written:  " << m_tileSizeInBytes
                     << "\nBytes written:  " << bytesWritten
                     << std::endl;
                  theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
                  
                  return false;
               }
            }
            if (resLevel == 1 && m_maskBuildFlag)
            {
              t->computeAlphaChannel();
              ossim_uint8* alphaValues = t->getAlphaBuf();
              packAlphaValues(alphaValues, t->getImageRectangle(), rect.width());
            }
         }
         x += m_tileWidth; // Increment x for next TIFFWriteTile.
         ++tileNumber;      // Increment tile number for percent complete.

      } // End of tile loop in the sample (width) direction.

      if (needsAborting())
      {
         setPercentComplete(100.0);
         break;
      }
      else
      {
         double tile = tileNumber;
         double numTiles = numberOfTiles;
         setPercentComplete(tile / numTiles * 100.0);
      }

      y += m_tileHeight; // Increment y for next TIFFWriteTile.

   } // End of tile loop in the line (height) direction.

   if (resLevel == 1 && m_maskBuildFlag)
   {
     closeMaskFile();
   }

   //---
   // Write the current dirctory.
   //---
   if (!TIFFWriteDirectory(tif))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Error writing directory!" << std::endl;
      return false;
   }

   if ( firstResLevel && ( getHistogramMode() != OSSIM_HISTO_MODE_UNKNOWN) )
   {
      // Write the histogram.
      if(ossimMpi::instance()->getNumberOfProcessors() == 1)
      {
         ossimFilename histoFilename = getOutputFile();
         histoFilename.setExtension("his");
         sequencer->writeHistogram(histoFilename);
      }
   }

   ++m_currentTiffDir;

   return true;
}

//*******************************************************************
// Private Method:
//*******************************************************************
bool ossimTiffOverviewBuilder::setTags(TIFF* tif,
                                       const ossimIrect& outputRect,
                                       ossim_int32 resLevel) const
{
   if (outputRect.hasNans())
   {
      return false;
   }
   
   ossim_int32   imageWidth      = outputRect.width();
   ossim_int32   imageHeight     = outputRect.height();
   int16         samplesPerPixel = m_imageHandler->getNumberOfOutputBands();
   ossim_float64 minSampleValue  = m_imageHandler->getMinPixelValue();
   ossim_float64 maxSampleValue  = m_imageHandler->getMaxPixelValue();

   if (resLevel)
   {
      TIFFSetField( tif, TIFFTAG_SUBFILETYPE, FILETYPE_REDUCEDIMAGE );
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTiffOverviewBuilder::setTags DEBUG:"
         << "\nrrds_level:      " << resLevel
         << "\nimageWidth:      " << imageWidth
         << "\nimageHeight:     " << imageHeight
         << "\nminSampleValue:  " << minSampleValue
         << "\nmaxSampleValue:  " << maxSampleValue
         << std::endl;
   }
   TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE );
   TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, imageWidth);
   TIFFSetField( tif, TIFFTAG_IMAGELENGTH, imageHeight);
   TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, m_bitsPerSample );
   TIFFSetField( tif, TIFFTAG_SAMPLEFORMAT, m_sampleFormat );
   TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel );

   if( m_imageHandler->getNumberOfInputBands() == 3 )
      TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
   else
      TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );
   
   TIFFSetField( tif, TIFFTAG_TILEWIDTH,  m_tileWidth  );
   TIFFSetField( tif, TIFFTAG_TILELENGTH, m_tileHeight );
   
   // Set the compression related tags...
   TIFFSetField( tif, TIFFTAG_COMPRESSION, m_tiffCompressType );
   if (m_tiffCompressType == COMPRESSION_JPEG)
   {
      TIFFSetField( tif, TIFFTAG_JPEGQUALITY,  m_jpegCompressQuality);
   }
   
   // Set the min/max values.
   switch( m_imageHandler->getOutputScalarType() )
   {
      case OSSIM_SINT16:
      case OSSIM_FLOAT32:
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
         TIFFSetField( tif, TIFFTAG_SMINSAMPLEVALUE, minSampleValue );
         TIFFSetField( tif, TIFFTAG_SMAXSAMPLEVALUE, maxSampleValue );
         break;

      case OSSIM_UINT8:
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      case OSSIM_UINT32:
      default:
         TIFFSetField( tif, TIFFTAG_MINSAMPLEVALUE,
                       static_cast<int>(minSampleValue) );
         TIFFSetField( tif, TIFFTAG_MAXSAMPLEVALUE,
                       static_cast<int>(maxSampleValue) );
         break;
   }
   
    return true;
}

bool ossimTiffOverviewBuilder::setGeotiffTags(const ossimImageGeometry* geom,
                                              const ossimDrect& boundingRect,
                                              ossim_uint32 resLevel,
                                              TIFF* tif)
{
   bool result = false;

   if ( geom && tif )
   {
      const ossimProjection* proj = geom->getProjection();
      if (proj)
      {
         // Must duplicate if changing scale.
         ossimObject* obj = proj->dup();
         ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, obj);
         if ( mapProj )
         {
            if ( mapProj->hasModelTransform() == false )
            {
               if (resLevel)
               {
                  ossim_float64 factor = (ossim_float64)(1 << resLevel);
                  mapProj->applyScale(ossimDpt(factor, factor), true);
               }
               ossimRefPtr<ossimMapProjectionInfo> projInfo =
                  new ossimMapProjectionInfo(mapProj, boundingRect);
               result = ossimGeoTiff::writeTags(tif, projInfo);
            }
         }
         delete obj; // Cleanup from dup.
         obj = 0;
      }
   }
   
   return result;
}

TIFF* ossimTiffOverviewBuilder::openTiff(const ossimString& filename,
                                         const ossimString& openMode)
{
// #ifdef OSSIM_HAS_GEOTIFF
// #  if OSSIM_HAS_GEOTIFF
   return XTIFFOpen( filename.c_str(), openMode.c_str() );
// #  else
//    return TIFFOpen( filename.c_str(), openMode.c_str() );
// #  endif
// #else
//    return TIFFOpen( filename.c_str(), openMode.c_str() );
// #endif
}

void ossimTiffOverviewBuilder::closeTiff(TIFF* tif)
{
// #ifdef OSSIM_HAS_GEOTIFF
// #  if OSSIM_HAS_GEOTIFF 
      XTIFFClose( tif );
// #  else
//       TIFFClose( tif );
// #  endif
// #else
//       TIFFClose( tif );   
// #endif
}


void ossimTiffOverviewBuilder::setCompressionType(ossim_uint16 compression_type)
{
   switch (compression_type)
   {
   case COMPRESSION_JPEG:
   case COMPRESSION_LZW:
   case COMPRESSION_DEFLATE:
   case COMPRESSION_PACKBITS:
      m_tiffCompressType = compression_type;
      break;
   default:
      m_tiffCompressType = COMPRESSION_NONE;
      if (traceDebug())
      {
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_WARNING,
                       "\
ossimTiffOverviewBuilder::setCompressionType\nfile %s line %d\n\
Unsupported compression type: %d  Defaulting to none.",
                       __FILE__,
                       __LINE__,
                       compression_type);
      }
      break;
   }
}

void ossimTiffOverviewBuilder::setJpegCompressionQuality(ossim_int32 quality)
{
   if (quality > 1 && quality < 101)
   {
      m_jpegCompressQuality = quality;
   }
   else
   {
      m_jpegCompressQuality = DEFAULT_COMPRESS_QUALITY;

      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_WARNING,
                    "\
ossimTiffOverviewBuilder::setJpegCompressionQuality\n%s file %s \
line %d Compression quality of %d is out of range!\nis out of range!\n\
Range is 100 to 1.  Current quality set to default of 75.",
                    __FILE__,
                    __LINE__,
                    quality);
   }
}

bool ossimTiffOverviewBuilder::getCopyAllFlag() const
{
   return m_copyAllFlag;
}

void ossimTiffOverviewBuilder::setCopyAllFlag(bool flag)
{
   m_copyAllFlag = flag;
}

ossimObject* ossimTiffOverviewBuilder::getObject()
{
   return this;
}

const ossimObject* ossimTiffOverviewBuilder::getObject() const
{
   return this;
}

void ossimTiffOverviewBuilder::setOutputFile(const ossimFilename& file)
{
   m_outputFile = file;
}

ossimFilename ossimTiffOverviewBuilder::getOutputFile() const
{
   ossimFilename result = m_outputFile;
   if (m_outputFile == ossimFilename::NIL)
   {
      if (m_imageHandler.valid())
      {
         bool usePrefix = (m_imageHandler->getNumberOfEntries()>1?true:false);
         result = m_imageHandler->
            getFilenameWithThisExtension(ossimString("ovr"), usePrefix);
      }
   }
   return result;
}

void ossimTiffOverviewBuilder::setOutputTileSize(const ossimIpt& tileSize)
{
   m_tileWidth  = tileSize.x;
   m_tileHeight = tileSize.y;
   m_outputTileSizeSetFlag = true;
}

bool ossimTiffOverviewBuilder::setInputSource(ossimImageHandler* imageSource)
{
   static const char MODULE[] =
      "ossimTiffOverviewBuilder::initializeFromHandler";

   m_imageHandler         = imageSource;

   if (!m_imageHandler)
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNull image handler pointer passed to constructor! Returning..."
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to constructor!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return false;
   }
   else if (m_imageHandler->getErrorStatus() ==
            ossimErrorCodes::OSSIM_ERROR)
   {
      // Set the error...
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nError detected in image handler!  Returning..."
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s file %s line %d\nImageHandler error detected!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return false;
   }

   if(!m_outputTileSizeSetFlag)
   {
   // Note:  Need a default overview tile size in preferences...
      ossimIpt tileSize;
      ossim::defaultTileSize(tileSize);
//       if(m_imageHandler->isImageTiled())
//       {
//          if(m_imageHandler->getBoundingRect().width() != m_imageHandler->getImageTileWidth())
//          {
//             tileSize = ossimIpt(m_imageHandler->getImageTileWidth(),
//                                 m_imageHandler->getImageTileHeight());
//          }
//       }

      m_tileWidth  = tileSize.x;
      m_tileHeight = tileSize.y;
   }
   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nm_tileWidth:   " << m_tileWidth
           << "\nm_tileHeight:  " << m_tileHeight
           << "\nSource image is tiled:  "
           << (m_imageHandler->isImageTiled()?"true":"false")
           << "\nm_imageHandler->getTileWidth():  "
           << m_imageHandler->getTileWidth()
           << "\nm_imageHandler->getTileHeight():  "
           << m_imageHandler->getTileHeight()
           << "\nm_imageHandler->getImageTileWidth():  "
           << m_imageHandler->getImageTileWidth()
           << "\nm_imageHandler->getImageTileHeight():  "
           << m_imageHandler->getImageTileHeight()
           << std::endl;
   }

   switch(m_imageHandler->getOutputScalarType())
   {
      case OSSIM_UINT8:
         m_bitsPerSample = 8;
         m_bytesPerPixel = 1;
         m_sampleFormat  = SAMPLEFORMAT_UINT;
         break;
         
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
         m_bitsPerSample = 16;
         m_bytesPerPixel = 2;
         m_sampleFormat  = SAMPLEFORMAT_UINT;
         break;
      
      case OSSIM_SINT16:
         m_bitsPerSample = 16;
         m_bytesPerPixel = 2;
         m_sampleFormat  = SAMPLEFORMAT_INT;
         break;
         
      case OSSIM_SINT32:
         m_bitsPerSample = 32;
         m_bytesPerPixel = 4;
         m_sampleFormat  = SAMPLEFORMAT_INT;
         break;
         
      case OSSIM_UINT32:
         m_bitsPerSample = 32;
         m_bytesPerPixel = 4;
         m_sampleFormat  = SAMPLEFORMAT_UINT;
         break;
      
      case OSSIM_FLOAT32:
         m_bitsPerSample = 32;
         m_bytesPerPixel = 4;
         m_sampleFormat  = SAMPLEFORMAT_IEEEFP;
         break;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         m_bitsPerSample = 64;
         m_bytesPerPixel = 8;
         m_sampleFormat  = SAMPLEFORMAT_IEEEFP;
         break;
         
      default:
         // Set the error...
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:"
            << "\nUnknow pixel type:  "
            << (ossimScalarTypeLut::instance()->
                getEntryString(m_imageHandler->getOutputScalarType()))
            << std::endl;
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "Unknown pixel type!",
                       __FILE__,
                       __LINE__);
         return false;
   }

   m_tileSizeInBytes = m_tileWidth * m_tileHeight * m_bytesPerPixel;

   //---
   // Make a buffer to pass to pass to the write tile methods when an image
   // handler returns a null tile.
   //---
   m_nullDataBuffer.resize(m_tileSizeInBytes);

   // Fill it with zeroes.
   std::fill(m_nullDataBuffer.begin(), m_nullDataBuffer.end(), 0);

   return true;
}

bool ossimTiffOverviewBuilder::setOverviewType(const ossimString& type)
{
   bool result = true;
   if (type == "ossim_tiff_nearest")
   {
      m_resampleType =
         ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR;
   }
   else if (type == "ossim_tiff_box")
   {
      m_resampleType = ossimFilterResampler::ossimFilterResampler_BOX;
   }
   else
   {
      result = false;
   }
   return result;
}

ossimString ossimTiffOverviewBuilder::getOverviewType() const
{
   ossimString type;
   if (m_resampleType == ossimFilterResampler::ossimFilterResampler_NEAREST_NEIGHBOR)
   {
      type = "ossim_tiff_nearest";
   }
   else
   {
      type = "ossim_tiff_box"; // This is default...
   }
   return type;
}

void ossimTiffOverviewBuilder::getTypeNameList(
   std::vector<ossimString>& typeList)const
{
   typeList.push_back(ossimString("ossim_tiff_box"));
   typeList.push_back(ossimString("ossim_tiff_nearest"));
}

void ossimTiffOverviewBuilder::setProperty(ossimRefPtr<ossimProperty> property)
{
   if ( property.valid() )
   {
      if(property->getName() == ossimKeywordNames::COMPRESSION_QUALITY_KW)
      {
         m_jpegCompressQuality = property->valueToString().toInt32();
      }
      else if(property->getName() == ossimKeywordNames::COMPRESSION_TYPE_KW)
      {
      ossimString value = property->valueToString();
      value = value.downcase();
      if(value == "jpeg")
      {
         m_tiffCompressType =  COMPRESSION_JPEG;
      }
      else if(value == "lzw")
      {
         m_tiffCompressType =  COMPRESSION_LZW;
         
      }
      else if(value == "deflate")
      {
         m_tiffCompressType =  COMPRESSION_DEFLATE;
      }
      else if(value == "packbits")
      {
         m_tiffCompressType =  COMPRESSION_PACKBITS;
      }
      else
      {
         m_tiffCompressType = COMPRESSION_NONE;
      }
      }
      else if(property->getName() == COPY_ALL_KW)
      {
         m_copyAllFlag = property->valueToString().toBool();
      }
      else if(property->getName() ==
              ossimKeywordNames::OVERVIEW_STOP_DIMENSION_KW)
      {
         m_overviewStopDimension = property->valueToString().toUInt32();
      }
      else if(property->getName() == ossimKeywordNames::OUTPUT_TILE_SIZE_KW)
      {
         ossimIpt ipt;
         
         ipt.toPoint(property->valueToString());
         
         setOutputTileSize(ipt);
      }
   }
}

void ossimTiffOverviewBuilder::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(ossimKeywordNames::COMPRESSION_QUALITY_KW);
   propertyNames.push_back(ossimKeywordNames::COMPRESSION_TYPE_KW);
   propertyNames.push_back(COPY_ALL_KW);
   propertyNames.push_back(ossimKeywordNames::OVERVIEW_STOP_DIMENSION_KW);
}

bool ossimTiffOverviewBuilder::canConnectMyInputTo(
   ossim_int32 index,
   const ossimConnectableObject* obj) const
{
   if ( (index == 0) &&
        PTR_CAST(ossimImageHandler, obj) )
   {
      return true;
   }

   return false;
}

void ossimTiffOverviewBuilder::packAlphaValues(ossim_uint8* alphaValues, 
                                               ossimIrect rect,
                                               ossim_uint32 maskWidth)
{
   ossim_uint32 tileCount = std::ceil((double)maskWidth/(double)rect.width());
   maskWidth = rect.width() * tileCount;

   ossim_uint32 rowNum = rect.ul().y;
   ossim_uint32 index = 0;
   bool isMaskWidth = false;
   for (ossim_uint32 line = 0; line < rect.height(); ++line)
   {
      std::vector<ossim_uint8> pixelValues;
      pixelValues.reserve(rect.width());
      for (ossim_uint32 sample = 0; sample < rect.width(); ++sample)
      {
         if (alphaValues[index+sample] == 255)
         {
            pixelValues.push_back(1);
         }
         else
         {
            pixelValues.push_back(0);
         }
      }
      index += rect.width();

      std::map<ossim_uint32, std::vector<ossim_uint8> >::iterator it = m_alphaMask.find(rowNum);
      if (it != m_alphaMask.end())
      {
         std::vector<ossim_uint8> fullVector;
         fullVector.reserve(it->second.size() + pixelValues.size());
         fullVector.insert(fullVector.end(), it->second.begin(), it->second.end());
         fullVector.insert(fullVector.end(), pixelValues.begin(), pixelValues.end());
         m_alphaMask[rowNum] = fullVector;
         if (fullVector.size() == maskWidth)
         {
            isMaskWidth = true;
         }
         fullVector.clear();
      }
      else
      {
         m_alphaMask[rowNum] = pixelValues;
      }

      pixelValues.clear();
      rowNum++;
   }

   if (isMaskWidth)
   {
      writeMaskStrip();
   }
}

bool ossimTiffOverviewBuilder::openMaskFile()
{
   const char* M = "ossimTiffOverviewBuilder::openMaskFile() -- ";

   // Open new mask file:
   setErrorStatus();
   ossimFilename file = m_outputFile;
   file.setExtension("mask");
   m_maskFileStream.open(file.chars(), ios::out | ios::binary);
   if(!m_maskFileStream.is_open())
   {
      ossimNotify(ossimNotifyLevel_WARN)<<M<<"Error encountered trying to create mask file <"<<
         file<<">. Cannot write mask.";
      return false;
   }
   clearErrorStatus();
   return true;
}

bool ossimTiffOverviewBuilder::closeMaskFile()
{
   if (m_maskFileStream.is_open())
   {
      m_maskFileStream.flush();
      m_maskFileStream.close();
   }
   m_maskFileStream.clear();
   return true;
}

bool ossimTiffOverviewBuilder::writeMaskStrip()
{
   const char* M = "ossimTiffOverviewBuilder::writeMaskStrip() -- ";

   if (!m_maskFileStream.is_open())
   {
      return false;
   }

   std::map<ossim_uint32, std::vector<ossim_uint8> >::iterator it = m_alphaMask.begin();
   while (it != m_alphaMask.end())
   {
      std::vector<ossim_uint8> bitPixels = it->second; 
      ossim_uint32 rowSize = (ossim_uint32)bitPixels.size();
      
      //Calculate the buffer size
      ossim_uint32 bufferSize = rowSize/8;
      ossim_uint32 remainPixels = bufferSize%8;
      if (remainPixels > 0)
      {
         bufferSize = bufferSize + 1;
      }
      
      char* mask = new char[rowSize];
      char* buffer = new char[bufferSize];
      
      for (ossim_uint32 i = 0; i < bitPixels.size(); i++)
      {
         mask[i] = bitPixels[i];
      }
      it++;
      
      // Fill buffer byte with 8 pixels worth of mask data, starting from first pixel:
      ossim_uint32 mask_index = 0;
      for (ossim_uint32 i = 0; i < bufferSize; i++)
      {
         if (mask_index > rowSize)//make index equal to mask size if it is bigger
         {
            mask_index = rowSize;
         }
         
         buffer[i] = 0;
         buffer[i] |= mask[mask_index++] << 7;
         buffer[i] |= mask[mask_index++] << 6;
         buffer[i] |= mask[mask_index++] << 5;
         buffer[i] |= mask[mask_index++] << 4;
         buffer[i] |= mask[mask_index++] << 3;
         buffer[i] |= mask[mask_index++] << 2;
         buffer[i] |= mask[mask_index++] << 1;
         buffer[i] |= mask[mask_index++];
      }
      
      // Write the alpha mask to general raster:
      m_maskFileStream.write(buffer, bufferSize);
      
      if (m_maskFileStream.fail())
      {
         ossimNotify(ossimNotifyLevel_FATAL)<<M<<"ERROR encountered writing mask file!";
         delete [] mask;
         delete [] buffer;
         return false;
      }
      
      delete [] mask;
      delete [] buffer;
   }
   
   m_alphaMask.clear();
   
   return true;
}
