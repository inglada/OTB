//**************************************************************************
// FILE: ossimElevManager.cpp
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimElevManager. This object 
//   provides a single interface for accessing multiple elevation
//   sources. This object owns one or more elevation sources in an ordered
//   list. When queried for an elevation at a particular point, it searches
//   the available sources for the best result, instantiating new sources if
//   necessary.
//
// SOFTWARE HISTORY:
//>
//   23Apr2001  Oscar Kramer
//              Initial coding.
//<
//**************************************************************************
// $Id: ossimElevManager.cpp 17699 2010-07-08 18:59:00Z gpotts $

#include <algorithm>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/elevation/ossimElevationDatabaseRegistry.h>
#include <ossim/base/ossimKeywordNames.h>

ossimElevManager* ossimElevManager::m_instance = 0;
static ossimTrace traceDebug("ossimElevManager:debug");

ossimElevManager* ossimElevManager::instance()
{
   if (!m_instance)
   {
      m_instance = new ossimElevManager();
      m_instance->loadStandardElevationPaths();
   }
   
   return m_instance;
}
ossimElevManager::ossimElevManager()
:ossimElevSource()
{
   m_instance = this;
   m_defaultHeightAboveEllipsoid = ossim::nan();
   m_elevationOffset = ossim::nan();
   m_useGeoidIfNullFlag = false;
}

ossimElevManager::~ossimElevManager()
{
   
}

double ossimElevManager::getHeightAboveEllipsoid(const ossimGpt& gpt)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   if(!isSourceEnabled()) return ossim::nan();

   double result = ossim::nan();
   ossim_uint32 idx = 0;
   for(;(idx < m_elevationDatabaseList.size())&&ossim::isnan(result); ++idx)
   {
      result = m_elevationDatabaseList[idx]->getHeightAboveEllipsoid(gpt);
   }
   if(ossim::isnan(result))
   {
      result = m_defaultHeightAboveEllipsoid;
   }
   if(m_useGeoidIfNullFlag&&ossim::isnan(result))
   {
      result = ossimGeoidManager::instance()->offsetFromEllipsoid(gpt);
   }
   if(!ossim::isnan(result)&&!ossim::isnan(m_elevationOffset))
   {
      result += m_elevationOffset;
   }
   return result;
}

double ossimElevManager::getHeightAboveMSL(const ossimGpt& gpt)
{
  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   if(!isSourceEnabled()) return ossim::nan();
   double result = ossim::nan();
   ossim_uint32 idx = 0;
   for(;(idx < m_elevationDatabaseList.size())&&ossim::isnan(result); ++idx)
   {
      result = m_elevationDatabaseList[idx]->getHeightAboveMSL(gpt);
   }
   if(ossim::isnan(result)&&!ossim::isnan(m_defaultHeightAboveEllipsoid))
   {
      result = m_defaultHeightAboveEllipsoid;
      // Use first available geoid to offset the ellipsoid height. This at least gets us close: 
      double dh = ossimGeoidManager::instance()->offsetFromEllipsoid(gpt);
      if (!ossim::isnan(dh))
         result -= dh;
   }
   if(!ossim::isnan(result)&&(!ossim::isnan(m_elevationOffset)))
   {
      result += m_elevationOffset;
   }
   return result;
}

void ossimElevManager::loadStandardElevationPaths()
{
   ossimFilename userDir    = ossimEnvironmentUtility::instance()->getUserOssimSupportDir();
   ossimFilename installDir = ossimEnvironmentUtility::instance()->getInstalledOssimSupportDir();
   
   userDir = userDir.dirCat("elevation");
   installDir = installDir.dirCat("elevation");
   
   loadElevationPath(userDir);
   loadElevationPath(installDir);
   
   ossimString paths = ossimEnvironmentUtility::instance()->getEnvironmentVariable("OSSIM_ELEVATION_PATH");
   std::vector<ossimString> pathArray;
   ossimString pathSeparator = ":";
#if defined(WIN32) && !defined(CYGWIN)
   pathSeparator = ";";
#endif
   
   if(!paths.empty())
   {
      paths.split(pathArray, pathSeparator);
      if(!pathArray.empty())
      {
         ossim_uint32 idx = 0;
         for(idx = 0; idx < pathArray.size(); ++idx)
         {
            ossimFilename file(pathArray[idx]);
            
            if(file.exists())
            {
               loadElevationPath(file);
            }
         }
      }
   }
}

bool ossimElevManager::loadElevationPath(const ossimFilename& path)
{
  bool result = false;
   ossimElevationDatabase* database = ossimElevationDatabaseRegistry::instance()->open(path);
   
   if(!database&&path.isDir())
   {
      ossimDirectory dir;
      
      if(dir.open(path))
      {
         ossimFilename file;
         dir.getFirst(file, ossimDirectory::OSSIM_DIR_DIRS);
         do
         {
            database = ossimElevationDatabaseRegistry::instance()->open(file);
            if(database)
            {
               result = true;
               addDatabase(database);
            }
         }while(dir.getNext(file));
      }
   }
   else if(database)
   {
      result = true;
      addDatabase(database);
   }
   
   return result;
}

void ossimElevManager::getOpenCellList(std::vector<ossimFilename>& list) const
{
   ossim_uint32 idx = 0;
   for(;idx < m_elevationDatabaseList.size(); ++idx)
   {
      m_elevationDatabaseList[idx]->getOpenCellList(list);
   }
}

void ossimElevManager::clear()
{
  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);

  m_elevationDatabaseList.clear();
}

bool ossimElevManager::saveState(ossimKeywordlist& kwl,
                                 const char* prefix) const
{
   return ossimElevSource::saveState(kwl, prefix);
}

/**
 * Method to the load (recreate) the state of an object from a keyword
 * list.  Return true if ok or false on error.
 */
bool ossimElevManager::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimElevManager::loadState: Entered..."
      << std::endl;
   }
   if(!ossimElevSource::loadState(kwl, prefix))
   {
      return false;
   }
   ossimString copyPrefix(prefix);
   ossimString elevationOffset = kwl.find(copyPrefix, "elevation_offset");
   ossimString defaultHeightAboveEllipsoid = kwl.find(copyPrefix, "default_height_above_ellipsoid");
   ossimString useGeoidIfNull = kwl.find(copyPrefix, "use_geoid_if_null");

   if(!elevationOffset.empty())
   {
      m_elevationOffset = elevationOffset.toDouble();
   }
   if(!defaultHeightAboveEllipsoid.empty())
   {
      m_defaultHeightAboveEllipsoid = defaultHeightAboveEllipsoid.toDouble();
   }
   if(!useGeoidIfNull.empty())
   {
      m_useGeoidIfNullFlag = useGeoidIfNull.toBool();
   }
   ossimString regExpression =  ossimString("^(") + copyPrefix + "elevation_source[0-9]+.)";
   vector<ossimString> keys =
   kwl.getSubstringKeyList( regExpression );
   long numberOfSources = (long)keys.size();
   ossim_uint32 offset = (ossim_uint32)(copyPrefix+"elevation_source").size();
   ossim_uint32 idx = 0;
   std::vector<int> theNumberList(numberOfSources);
   for(idx = 0; idx < theNumberList.size();++idx)
   {
      ossimString numberStr(keys[idx].begin() + offset,
                            keys[idx].end());
      theNumberList[idx] = numberStr.toInt();
   }
   std::sort(theNumberList.begin(), theNumberList.end());
   
   for(idx=0;idx < theNumberList.size();++idx)
   {
      ossimString newPrefix = copyPrefix;
      newPrefix += ossimString("elevation_source");
      newPrefix += ossimString::toString(theNumberList[idx]);
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimElevManager::loadState:"
         << "\nLooking for key:  " << newPrefix
         << std::endl;
      }
      // first check if new way is supported
      //
      ossimRefPtr<ossimElevationDatabase> database = ossimElevationDatabaseRegistry::instance()->createDatabase(kwl, newPrefix+".");
      if(database.valid())
      {
        if (traceDebug())
        {
           ossimNotify(ossimNotifyLevel_DEBUG)
           << "DEBUG ossimElevManager::loadState:"
           << "\nadding elevation database:  " << database->getConnectionString()
           << std::endl;
        }  
        addDatabase(database.get());
      }
      else
      {
         // if not new elevation load verify the old way by
         // looking at the filename
         //
         ossimString fileKey = newPrefix;
         fileKey += ".";
         fileKey += ossimKeywordNames::FILENAME_KW;
         ossimString lookup = kwl.find(prefix, fileKey.c_str());
         if (!lookup.empty())
         {
            loadElevationPath(ossimFilename(lookup));
         } // end if lookup
      }
   } // end for loop
   return true;
}

void ossimElevManager::addDatabase(ossimElevationDatabase* database)
{
   if(!database) return;
   ossimRefPtr<ossimElevationDatabase> tempDb = database;
  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
   if(std::find(m_elevationDatabaseList.begin(), 
                m_elevationDatabaseList.end(),
                database) == m_elevationDatabaseList.end())
   {
      m_elevationDatabaseList.push_back(database);
   }
}

