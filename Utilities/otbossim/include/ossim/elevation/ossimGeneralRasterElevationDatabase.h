#ifndef ossimGeneralRasterElevationDatabase_HEADER
#define ossimGeneralRasterElevationDatabase_HEADER
#include "ossimElevationDatabase.h"
#include <ossim/base/ossimFilename.h>
#include <ossim/elevation/ossimGeneralRasterElevHandler.h>
#include <OpenThreads/Mutex>

class OSSIM_DLL ossimGeneralRasterElevationDatabase : public ossimElevationDatabase
{
public:
   typedef std::vector<ossimRefPtr<CellInfo> > DirectMap; // 360x180 cell grid
   ossimGeneralRasterElevationDatabase()
   :ossimElevationDatabase()   
   {
   }
   ossimGeneralRasterElevationDatabase(const ossimGeneralRasterElevationDatabase& rhs)
   :ossimElevationDatabase(rhs)
   {
   }
   virtual ~ossimGeneralRasterElevationDatabase()
   {
      if(m_cellHandler.valid())
      {
         m_cellHandler->close();
      }
      m_cellHandler = 0;
   }
   ossimObject* dup()const
   {
      return new ossimGeneralRasterElevationDatabase(*this);
   }
   virtual bool open(const ossimString& connectionString);
   virtual bool pointHasCoverage(const ossimGpt& gpt) const
   {
      ossimFilename filename;
      createRelativePath(filename, gpt);
      
      return filename.exists();
   }
   /**
    * METHODS: accuracyLE90(), accuracyCE90()
    * Returns the vertical and horizontal accuracy (90% confidence) in the
    * region of gpt:
    */
   virtual double getAccuracyLE90(const ossimGpt& gpt) const
   {
      std::cout << "ossimGeneralElevationDatabase::getAccuracyLE90 \n";
      return 0.0;
   }
   virtual double getAccuracyCE90(const ossimGpt& gpt) const
   {
      std::cout << "ossimGeneralElevationDatabase::getAccuracyCE90 \n";
      return 0.0;
   }
   virtual double getHeightAboveMSL(const ossimGpt&);
   virtual double getHeightAboveEllipsoid(const ossimGpt& gpt);
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix = 0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix = 0)const;
   
protected:
   ossimRefPtr<ossimGeneralRasterElevHandler> m_cellHandler;
   bool openGeneralRasterDirectory(const ossimFilename& dir);
   void createRelativePath(ossimFilename& file, const ossimGpt& gpt)const;
   void createFullPath(ossimFilename& file, const ossimGpt& gpt)const
   {
      ossimFilename relativeFile;
      createRelativePath(relativeFile, gpt);
      file = ossimFilename(m_connectionString).dirCat(relativeFile);
   }
   ossimRefPtr<ossimElevCellHandler> getOrCreateHandler(const ossimGpt& gpt)
   {
      return m_cellHandler.get();
   }
   TYPE_DATA
};
#endif