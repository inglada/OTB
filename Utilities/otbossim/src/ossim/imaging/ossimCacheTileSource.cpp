//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts 
//
// Description:  ossimCacheTileSource
// 
//*******************************************************************
//  $Id: ossimCacheTileSource.cpp 16276 2010-01-06 01:54:47Z gpotts $

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>

static ossimTrace traceDebug("ossimCacheTileSource:debug");

static const ossimString TILE_SIZE_XY_KW("tile_size_xy");

RTTI_DEF1(ossimCacheTileSource, "ossimCacheTileSource", ossimImageSourceFilter);
   
ossimCacheTileSource::ossimCacheTileSource()
   : ossimImageSourceFilter(),
     theCacheId(ossimAppFixedTileCache::instance()->newTileCache()),
     theTile(0),
     theFixedTileSize(),
     theCachingEnabled(true),
     theEventProgressFlag(false),
     theCacheRLevel(0),
     theBoundingRect()     
{
   theBoundingRect.makeNan();
   theFixedTileSize = ossimAppFixedTileCache::instance()->getTileSize(theCacheId);
}

ossimCacheTileSource::~ossimCacheTileSource()
{
   ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   
   theCacheId = -1;
   theBoundingRect.makeNan();
}

void ossimCacheTileSource::flush()
{
   ossimAppFixedTileCache::instance()->flush(theCacheId);
}

void ossimCacheTileSource::initialize()
{
   ossimImageSourceFilter::initialize();
   flush();
   theCacheRLevel = 999999999;
   theTile = 0;
}
   
void ossimCacheTileSource::allocate()
{
   theTile = 0;
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this, this);
      theTile->initialize();
   }
}

ossimRefPtr<ossimImageData> ossimCacheTileSource::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> result = 0;
   
   if ( theInputConnection )
   {
      if ( isSourceEnabled() )
      {
         if( (resLevel != theCacheRLevel) ||  theBoundingRect.hasNans() )
         {
            flush();
            // allocate();
            
            // theCacheRLevel = 9999999;
            theCacheRLevel = resLevel;
            theBoundingRect = getBoundingRect(resLevel);
            if( !theBoundingRect.hasNans() )
            {
               theFixedTileSize = ossimAppFixedTileCache::instance()->getTileSize(theCacheId);
               theBoundingRect.stretchToTileBoundary(theFixedTileSize);
               ossimAppFixedTileCache::instance()->setRect(theCacheId,
                                                           theBoundingRect);
               // theCacheRLevel = resLevel;
            }
         }
         
         if(!theTile.valid())
         {
            allocate();
         }
         
         if (theTile.valid())
         {
            theTile->setImageRectangle(tileRect);
            theTile->makeBlank();
            
            if( !theBoundingRect.hasNans() )
            {
               result = fillTile(resLevel);
            }
            else
            {
               result = theTile;
            }
         }
      }
      else // Not enabled...
      {
         result = theInputConnection->getTile(tileRect, resLevel);
      }

   } // End:  if ( theInputConnection )

   return result;
}

ossimRefPtr<ossimImageData> ossimCacheTileSource::fillTile(
   ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> tempTile = 0;
   fireProgressEvent(0.0);
   ossimIrect boundingRect = theBoundingRect;
   ossimIrect tileRect = theTile->getImageRectangle();
   
   ossimIrect allignedRect = tileRect.clipToRect(boundingRect);
   
   if ( !allignedRect.hasNans() )
   {
      allignedRect.stretchToTileBoundary(theFixedTileSize);
      ossimAppFixedTileCache::ossimAppFixedCacheId cacheId = theCacheId;
      
      // check to see if we need to loop
      if((allignedRect == tileRect)&&
         (static_cast<ossim_int32>(tileRect.width())  == theFixedTileSize.x)&&
         (static_cast<ossim_int32>(tileRect.height()) == theFixedTileSize.y))
      {
         ossimIpt origin = tileRect.ul();
         if(theCachingEnabled)
         {
            tempTile = ossimAppFixedTileCache::instance()->getTile(cacheId,
                                                                   origin);
         }
         if(!tempTile.valid())
         {
            tempTile = theInputConnection->getTile(tileRect, resLevel);
            
            if(tempTile.valid())
            {
               if((tempTile->getBuf())&&
                  (tempTile->getDataObjectStatus()!=OSSIM_EMPTY)&&
                  theCachingEnabled)
               {
                  ossimAppFixedTileCache::instance()->addTile(cacheId,
                                                              tempTile);
               }
            }
         }
         
         if(tempTile.valid())
         {
            if((tempTile->getDataObjectStatus() != OSSIM_NULL)&&
               (tempTile->getDataObjectStatus() != OSSIM_EMPTY))
            {
               
               // theTile->setDataObjectStatus(tempTile->getDataObjectStatus());
               theTile->loadTile(tempTile.get());
            }
         }
         fireProgressEvent(100.0);
      }
      else
      {
         ossim_int32 boundaryHeight = allignedRect.height();
         ossim_int32 boundaryWidth  = allignedRect.width();
         ossimIpt origin(allignedRect.ul());
         ossim_int32 totalTiles  = (boundaryHeight/theFixedTileSize.y)*
            (boundaryWidth/theFixedTileSize.x);
         ossim_int32 currentTile = 0;
         for(ossim_int32 row = 0;
             row < boundaryHeight;
             row+=theFixedTileSize.y)
         {
            origin.x  =  allignedRect.ul().x;
            for(ossim_int32 col = 0;
                col < boundaryWidth;
                col+=theFixedTileSize.x)
            {
               ++currentTile;
               if(theCachingEnabled)
               {
                  tempTile =
                     ossimAppFixedTileCache::instance()->getTile(cacheId,
                                                                 origin);
               }
               else
               {
                  tempTile = 0;
               }
               if(!tempTile.valid())
               {
                  ossimIrect rect(origin.x,
                                  origin.y,
                                  origin.x + theFixedTileSize.x-1,
                                  origin.y + theFixedTileSize.y-1);
                  
                  tempTile = theInputConnection->getTile(rect, resLevel);
                  
                  if(tempTile.valid())
                  {
                     // drb if(theTile->getBuf()&&
                     if(tempTile->getBuf()&&                        
                        (tempTile->getDataObjectStatus()!=OSSIM_EMPTY)&&
                        theCachingEnabled)
                     {
                        ossimAppFixedTileCache::instance()->
                           addTile(cacheId, tempTile);
                     }
                  }
               }
               if(tempTile.valid())
               {
                  if(tempTile->getBuf()&&
                     (tempTile->getDataObjectStatus()!=OSSIM_EMPTY))
                  {
                     theTile->loadTile(tempTile.get());
                  }
               }
               double percent = 100.0*((double)currentTile/(double)totalTiles);
               fireProgressEvent(percent);
               origin.x   += theFixedTileSize.x;
            }
            origin.y += theFixedTileSize.y;
         }
         fireProgressEvent(100);
      }

      theTile->validate();
      
   } // End of:  if ( !allignedRect.hasNans() )
   
   return theTile;
}

ossim_uint32 ossimCacheTileSource::getTileWidth() const
{
   return theFixedTileSize.x;
}

ossim_uint32 ossimCacheTileSource::getTileHeight() const
{
   return theFixedTileSize.y;  
}

bool ossimCacheTileSource::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimAppFixedTileCache::instance()->deleteCache(theCacheId);
   
   theCacheId = ossimAppFixedTileCache::instance()->newTileCache();
   
   const char* lookup = kwl.find(prefix, ossimKeywordNames::ENABLE_CACHE_KW);
   if(lookup)
   {
      theCachingEnabled = ossimString(lookup).toBool();
   }

   lookup = kwl.find(prefix, TILE_SIZE_XY_KW);
   if (lookup)
   {
      ossimIpt pt;
      pt.toPoint(std::string(lookup));
      setTileSize(pt);
   }
   
   bool result = ossimImageSourceFilter::loadState(kwl, prefix);

   initialize();

   return result;
}

bool ossimCacheTileSource::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::ENABLE_CACHE_KW,
           theCachingEnabled,
           true);
   
   kwl.add(prefix,
           TILE_SIZE_XY_KW,
           theFixedTileSize.toString().c_str());
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

ossimRefPtr<ossimProperty> ossimCacheTileSource::getProperty(
   const ossimString& name)const
{
   // Lock for the length of this method.
   
   if (name == TILE_SIZE_XY_KW)
   {
      ossimRefPtr<ossimProperty> result =
         new ossimStringProperty(name, theFixedTileSize.toString());
      result->setCacheRefreshBit();
      return result;
   }
   else if(name == ossimKeywordNames::ENABLE_CACHE_KW)
   {
      ossimRefPtr<ossimProperty> result = new ossimBooleanProperty(name,
                                                                   theCachingEnabled);
      result->setCacheRefreshBit();
      return result;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimCacheTileSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   if (!property) return;

   ossimString name = property->getName();
   if (name == TILE_SIZE_XY_KW)
   {
      ossimIpt pt;
      pt.toPoint(property->valueToString());

      // Rule: Must be positive and at least 32.
      if ( (pt.x > 31) && (pt.y > 31) )
      {
         setTileSize(pt);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimCacheTileSource::setProperty NOTICE:"
            << "\nTile dimensions must be at least 32!"
            << "\nFormat = ( x, y )"
            << std::endl;
      }
   }
   else if(name == ossimKeywordNames::ENABLE_CACHE_KW)
   {
      setCachingEnabledFlag(property->valueToString().toBool());
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

void ossimCacheTileSource::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(TILE_SIZE_XY_KW);
   propertyNames.push_back(ossimKeywordNames::ENABLE_CACHE_KW);
   
   ossimImageSourceFilter::getPropertyNames(propertyNames);
}

ossimString ossimCacheTileSource::getLongName()const
{
   return ossimString("Tile Cache , cache for ossimImageData objects.");
}

ossimString ossimCacheTileSource::getShortName()const
{
   return ossimString("Tile Cache");
}

void ossimCacheTileSource::setCachingEnabledFlag(bool value)
{
   if(!value && theCachingEnabled)
   {
      flush();
   }
   theCachingEnabled = value;
}

void ossimCacheTileSource::setEventProgressFlag(bool value)
{
   theEventProgressFlag = value;
}

void ossimCacheTileSource::getTileSize(ossimIpt& size) const
{
   size = theFixedTileSize;
}

void ossimCacheTileSource::setTileSize(const ossimIpt& size)
{
   if (size != theFixedTileSize)
   {
      theTile = 0; // Force an allocate of new tile.
      theFixedTileSize = size;
      ossimAppFixedTileCache::instance()->setTileSize(theCacheId, size);
   }
}

void ossimCacheTileSource::fireProgressEvent(double percentComplete)
{
   if(theEventProgressFlag)
   {
      ossimProcessProgressEvent event(this, percentComplete);
      fireEvent(event);
   }
}
