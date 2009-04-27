//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
//----------------------------------------------------------------------------
// $Id$


#include "ossimPluginProjectionFactory.h"
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimProjection.h>
#include "ossimRadarSatModel.h"
#include "ossimEnvisatAsarModel.h"
#include "ossimTerraSarModel.h"
//#include <ossim/projection/ossimCosmoSkymedModel.h>
#include "ossimRadarSat2Model.h"
#include "ossimErsSarModel.h"

ossimPluginProjectionFactory* ossimPluginProjectionFactory::instance()
{
   static ossimPluginProjectionFactory* factoryInstance = new ossimPluginProjectionFactory();
   
   return factoryInstance;
}

ossimProjection* ossimPluginProjectionFactory::createProjection(const ossimFilename& filename,
                                                                ossim_uint32 entryIdx)const
{
   cout << "a..." << endl;
   
   ossimProjection* result = 0;
   
   ossimRadarSat2Model* model = new ossimRadarSat2Model();
   if ( model->open(filename) )
   {
      result = model;
   }
   else
   {
      delete result;
      result = 0;
   }
   
   return result;
}

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimString& name)const
{
//    if (name == STATIC_TYPE_NAME(ossimRadarSatModel))
//    {
// 	   return new ossimRadarSatModel;
//    }
//    else if (name == STATIC_TYPE_NAME(ossimEnvisatAsarModel))
//    {
// 	   return new ossimEnvisatAsarModel;
//    }
// 	else if (name == STATIC_TYPE_NAME(ossimTerraSarModel))
//    {
// 	   return new ossimTerraSarModel;
//    }
   // 	else if (name == STATIC_TYPE_NAME(ossimCosmoSkymedModel))
   //    {
   // 	   return new ossimCosmoSkymedModel;
   //   }
   if (name == STATIC_TYPE_NAME(ossimRadarSat2Model))
   {
	   return new ossimRadarSat2Model;
   }
//    else if (name == STATIC_TYPE_NAME(ossimErsSarModel))
//    {
// 	   return new ossimErsSarModel;
//    }
   return 0;
}

ossimProjection* ossimPluginProjectionFactory::createProjection(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   cout << "b..." << endl;
   ossimProjection* result = 0;

   const char* lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      ossimString type = lookup;

      if (type == "ossimRadarSat2Model")
      {
         result = new ossimRadarSat2Model();
         if ( !result->loadState(kwl, prefix) )
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;
}

ossimObject* ossimPluginProjectionFactory::createObject(const ossimString& typeName)const
{
   cout << "c..." << endl;
   return createProjection(typeName);
}

ossimObject* ossimPluginProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                        const char* prefix)const
{
   cout << "d..." << endl;    
   return createProjection(kwl, prefix);
}


void ossimPluginProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimRadarSatModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimRadarSat2Model));
   typeList.push_back(STATIC_TYPE_NAME(ossimTerraSarModel));
   //   result.push_back(STATIC_TYPE_NAME(ossimCosmoSkymedModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimEnvisatAsarModel));
   typeList.push_back(STATIC_TYPE_NAME(ossimErsSarModel));
}