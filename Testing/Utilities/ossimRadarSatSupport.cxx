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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

/*!
 *
 * PURPOSE:
 *
 * Application to reproject a region of an image in geographic coordinates
 * using an interpolation, a region extractor and an iterator.
 *
 */

// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "ossim/base/ossimKeywordlist.h"
#include "ossim/init/ossimInit.h"
//#include "ossim/imaging/ossimImageHandler.h"
//#include "ossim/projection/ossimProjection.h"
//#include "ossim/projection/ossimProjectionFactoryRegistry.h"
//#include "ossim/imaging/ossimImageHandlerRegistry.h"
//#include "ossim/imaging/ossimImageHandlerSarFactory.h"
// #include "ossim/projection/ossimTerraSarModel.h"
#include "projection/ossimProjection.h"
#include "projection/ossimProjectionFactoryRegistry.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossimRadarSatModel.h"

int ossimRadarSatSupport(int argc, char* argv[])
{

  try
  {
    ossimInit::instance()->initialize(argc, argv);

    if (argc < 2)
    {
      /*
       * Verification que l'utilisateur passe bien un fichier en parametre de l'application
       */
      std::cout << argv[0] << " <input filename> " << std::endl;

      return EXIT_FAILURE;
    }
    
    char * filename = argv[1];

    /** Don't use FactoryRegistry because of its default factory that can conflict
     * with plugins factor (cf. TSX .tif image read as QB)*/
    // test ossim plugin factory
    ossimProjection * projection = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(
                                      ossimFilename(filename), 0);
    
    // if ossim plugins factory failed, then test ossim factory
    if (!projection)
    {
      projection = ossimProjectionFactoryRegistry::instance()->createProjection(ossimFilename(filename), 0);
      if (!projection)
      {
        std::cout<<"OSSIM Instanciate projection FAILED ! ";
        return EXIT_FAILURE;
      }
    }
    
    ossimKeywordlist geom;
    std::cout << "Read ossim Keywordlist...";

    bool hasMetaData = false;

    hasMetaData = projection->saveState(geom);

    if (!hasMetaData)
    {
      std::cout << "Bad metadata parsing " << std::endl;
      return EXIT_FAILURE;
    }

    ossimGpt ossimGPoint(0, 0);
    ossimDpt ossimDPoint;
    std::cout << "Creating projection..." << std::endl;
    ossimProjection * model = NULL;
    /*
     * Creation d'un modèle de projection à partir des métadonnées
     */
    model = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
    // if ossim plugins factory failed, then test ossim factory
    if (model == NULL)
    {
      projection = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
      if (model == NULL)
      {
        std::cout << "Invalid Model * == NULL !";
        return EXIT_FAILURE;
      }
    }

    /* std::cout<<"Creating RefPtr of projection...";
     ossimRefPtr<ossimProjection> ptrmodel = model;
     if( ptrmodel.valid() == false )
     {
     std::cout<<"Invalid Model pointer .valid() == false !";
     }
     */

    const double RDR_DEUXPI = 6.28318530717958647693;

    int numero_produit = 1; // RDS : 1 ; RDS appuis : 2 ; RDS SGF : 3
    // generique 4 coins + centre TSX : 0
    if (numero_produit == 1)
    {
      {
        if (model != NULL)
        {
          int i = 2;
          int j = 3;
          // average height
          const char* averageHeight_str = geom.find("terrain_height");
          double averageHeight = atof(averageHeight_str);
          std::cout << "Altitude moyenne :" << averageHeight << std::endl;

          ossimDpt image(i, j);
          ossimDpt imageret;
          ossimGpt world;
          double height = averageHeight;
          model->lineSampleHeightToWorld(image, height, world);
          std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

          model->worldToLineSample(world, imageret);
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
          std::cout << std::endl;

          model->lineSampleToWorld(image, world);
          std::cout << "Loc directe par intersection du rayon de visee et MNT : " << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << " altitude : " << world.height()
              << std::endl;
        }
      }
    }

    if (numero_produit == 3)
    {
      //8650 3062 43.282566 1.204279 211
      /*
       * Localisation du point d'appui
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 8650;
        int j = 3062;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 211;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.282566, lon = 1.204279" << std::endl;
        std::cout << "    erreur lat =" << world.lat - 43.282566 << " , erreur lon =" << world.lon - 1.204279
            << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        ossimGpt * groundGCP = new ossimGpt(43.282566, 1.204279, 211);
        model->worldToLineSample(*groundGCP, imageret);
        std::cout << "Loc inverse des vraies coords geo : " << std::endl;
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << std::endl;
      }

      //8139 3908 43.200920 1.067617 238

      /*
       * Localisation d'un point d'appui de validation
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 8139;
        int j = 3908;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 238;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.200920, lon = 1.067617" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.200920 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - 1.067617 << std::endl;
        std::cout << std::endl;
      }

      //5807 5474 43.096737 0.700934 365
      /*
       * Localisation d'un point d'appui de validation
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 5807;
        int j = 5474;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 365;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.096737, lon = 0.700934" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.096737 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - 0.700934 << std::endl;
        std::cout << std::endl;
      }

      //7718 5438 43.077911 0.967650 307
      /*
       * Localisation d'un point d'appui de validation
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 7718;
        int j = 5438;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 307;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.077911, lon = 0.967650" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.077911 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - 0.967650 << std::endl;
        std::cout << std::endl;
      }
      //6599 2800 43.319109 0.838037 275
      /*
       * Localisation d'un point d'appui de validation
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 6599;
        int j = 2800;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 275;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.096737, lon = 0.700934" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.319109 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - 0.838037 << std::endl;
        std::cout << std::endl;
      }

      //596 3476 43.456994 -0.087414 242
      /*
       * Localisation d'un point d'appui de validation
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 596;
        int j = 3476;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 242;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.456994, lon = -0.087414" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.456994 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - -0.087414 << std::endl;
        std::cout << std::endl;
      }

      /**************************************************************************/
      /* test de la prise en compte de points d'appui                  */
      /**************************************************************************/
      std::cout << "*********** OPTIMISATION **********" << std::endl;

      ossimplugins::ossimRadarSatModel * RDSmodel = (ossimplugins::ossimRadarSatModel *) model;
      std::list<ossimGpt> listePtsSol;
      std::list<ossimDpt> listePtsImage;

      ossimDpt * imageGCP;
      ossimGpt * groundGCP;

      imageGCP = new ossimDpt(8650, 3062);
      groundGCP = new ossimGpt(43.282566 * RDR_DEUXPI / 360.0, 1.204279 * RDR_DEUXPI / 360.0, 211);
      listePtsSol.push_back(*groundGCP);
      listePtsImage.push_back(*imageGCP);

      RDSmodel->optimizeModel(listePtsSol, listePtsImage);

      //8650 3062 43.282566 1.204279 211
      /*
       * Localisation du point d'appui
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 8650;
        int j = 3062;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 211;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.282566, lon = 1.204279" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.282566 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - 1.204279 << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        ossimGpt * groundGCP = new ossimGpt(43.282566 * RDR_DEUXPI / 360.0, 1.204279 * RDR_DEUXPI / 360.0, 211);
        model->worldToLineSample(*groundGCP, imageret);
        std::cout << "Loc inverse des vraies coords geo : " << std::endl;
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << std::endl;
      }

      //8139 3908 43.200920 1.067617 238

      /*
       * Localisation d'un point d'appui de validation
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 8139;
        int j = 3908;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 238;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.200920, lon = 1.067617" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.200920 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - 1.067617 << std::endl;
        std::cout << std::endl;
      }

      //5807 5474 43.096737 0.700934 365
      /*
       * Localisation d'un point d'appui de validation
       */
      //if (argc = 4)
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        int i = 5807;
        int j = 5474;

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        double height = 365;
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat * 360.0 / RDR_DEUXPI << " longitude = " << world.lon * 360.0
            / RDR_DEUXPI << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = 43.096737, lon = 0.700934" << std::endl;
        std::cout << "    erreur lat =" << world.lat * 360.0 / RDR_DEUXPI - 43.096737 << " , erreur lon =" << world.lon
            * 360.0 / RDR_DEUXPI - 0.700934 << std::endl;
        std::cout << std::endl;
      }
    }

    if (numero_produit == 2)
    {
      {
        //5130 4283 43.734466 6.185295 506
        /*
         * Localisation du point d'appui
         */
        //if (argc = 4)
        if (model != NULL)
        {
          std::cout << "**** loc point d'appui ****" << std::endl;

          int i = 5130;
          int j = 4283;

          ossimDpt image(i, j);
          ossimDpt imageret;
          ossimGpt world;
          double height = 506;
          model->lineSampleHeightToWorld(image, height, world);
          std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

          std::cout << "altitude : " << world.height() << std::endl;
          std::cout << "Resultat attendu : " << std::endl;
          std::cout << "lat = 43.734466, lon = 6.185295" << std::endl;

          model->worldToLineSample(world, imageret);
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

          ossimGpt * groundGCP = new ossimGpt(43.734466, 6.185295, 11);
          model->worldToLineSample(*groundGCP, imageret);
          std::cout << "Loc inverse des vraies coords geo : " << std::endl;
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
          std::cout << std::endl;
        }

        //2207 9685 43.551 5.565 340

        /*
         * Localisation d'un point d'appui de validation
         */
        //if (argc = 4)
        if (model != NULL)
        {
          std::cout << "**** loc point central ****" << std::endl;

          int j = 9658;
          int i = 2207;

          ossimDpt image(i, j);
          ossimDpt imageret;
          ossimGpt world;
          double height = 340;
          model->lineSampleHeightToWorld(image, height, world);
          std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

          std::cout << "altitude : " << world.height() << std::endl;

          model->worldToLineSample(world, imageret);
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
          std::cout << "Resultat attendu : " << std::endl;
          std::cout << "lat = 43.551, lon = 5.565" << std::endl;
          std::cout << std::endl;
        }

        //2063 9966 43.542 5.537 323
        /*
         * Localisation d'un point d'appui de validation
         */
        //if (argc = 4)
        if (model != NULL)
        {
          std::cout << "**** loc point central ****" << std::endl;

          int j = 9966;
          int i = 2063;

          ossimDpt image(i, j);
          ossimDpt imageret;
          ossimGpt world;
          double height = 323;
          model->lineSampleHeightToWorld(image, height, world);
          std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

          std::cout << "altitude : " << world.height() << std::endl;

          model->worldToLineSample(world, imageret);
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
          std::cout << "Resultat attendu : " << std::endl;
          std::cout << "lat = 43.542, lon = 5.537" << std::endl;
          std::cout << std::endl;
        }

        /**************************************************************************/
        /* test de la prise en compte de points d'appui                  */
        /**************************************************************************/
        std::cout << "*********** OPTIMISATION **********" << std::endl;

        ossimplugins::ossimRadarSatModel * RDSmodel = (ossimplugins::ossimRadarSatModel *) model;
        std::list<ossimGpt> listePtsSol;
        std::list<ossimDpt> listePtsImage;

        ossimDpt * imageGCP;
        ossimGpt * groundGCP;

        imageGCP = new ossimDpt(5130, 4283);
        groundGCP = new ossimGpt(43.734466, 6.185295, 506);
        listePtsSol.push_back(*groundGCP);
        listePtsImage.push_back(*imageGCP);

        RDSmodel->optimizeModel(listePtsSol, listePtsImage);

        //5130 4283 43.734466 6.185295 506

        /*
         * Localisation du point d'appui
         */
        //if (argc = 4)
        if (model != NULL)
        {
          std::cout << "**** loc point d'appui ****" << std::endl;

          int i = 5130;
          int j = 4283;

          ossimDpt image(i, j);
          ossimDpt imageret;
          ossimGpt world;
          double height = 506;
          model->lineSampleHeightToWorld(image, height, world);
          std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

          std::cout << "altitude : " << world.height() << std::endl;
          std::cout << "Resultat attendu : " << std::endl;
          std::cout << "lat = 43.734466, lon = 6.185295" << std::endl;

          model->worldToLineSample(world, imageret);
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

          ossimGpt * groundGCP = new ossimGpt(43.734466, 6.185295, 11);
          model->worldToLineSample(*groundGCP, imageret);
          std::cout << "Loc inverse des vraies coords geo : " << std::endl;
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
          std::cout << std::endl;
        }

        //2207 9685 43.551 5.565 340

        /*
         * Localisation d'un point d'appui de validation
         */
        //if (argc = 4)
        if (model != NULL)
        {
          std::cout << "**** loc point central ****" << std::endl;

          int j = 9658;
          int i = 2207;

          ossimDpt image(i, j);
          ossimDpt imageret;
          ossimGpt world;
          double height = 340;
          model->lineSampleHeightToWorld(image, height, world);
          std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

          std::cout << "altitude : " << world.height() << std::endl;

          model->worldToLineSample(world, imageret);
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
          std::cout << "Resultat attendu : " << std::endl;
          std::cout << "lat = 43.551, lon = 5.565" << std::endl;
          std::cout << std::endl;
        }
        //2063 9966 43.542 5.537 323
        /*
         * Localisation d'un point d'appui de validation
         */
        //if (argc = 4)
        if (model != NULL)
        {
          std::cout << "**** loc point central ****" << std::endl;

          int j = 9966;
          int i = 2063;

          ossimDpt image(i, j);
          ossimDpt imageret;
          ossimGpt world;
          double height = 323;
          model->lineSampleHeightToWorld(image, height, world);
          std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
          std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

          std::cout << "altitude : " << world.height() << std::endl;

          model->worldToLineSample(world, imageret);
          std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
          std::cout << "Resultat attendu : " << std::endl;
          std::cout << "lat = 43.542, lon = 5.537" << std::endl;
          std::cout << std::endl;
        }
      }
    }

    if (numero_produit == 0)
    {
      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol0");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin0");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon0");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat0");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        ossimGpt * groundGCP = new ossimGpt(lat, lon, height);
        model->worldToLineSample(*groundGCP, imageret);
        std::cout << "Loc inverse des vraies coords geo : " << std::endl;
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << std::endl;

        model->lineSampleToWorld(image, world);
        std::cout << "Loc directe par intersection du rayon de visee et MNT : " << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << " altitude : " << world.height()
            << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol1");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin1");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon1");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat1");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol2");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin2");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon2");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat2");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol3");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin3");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon3");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat3");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol4");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin4");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon4");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat4");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }

      /**************************************************************************/
      /* test de la prise en compte de points d'appui                  */
      /**************************************************************************/
      std::cout << "*********** OPTIMISATION **********" << std::endl;

      ossimplugins::ossimRadarSatModel * RDSmodel = (ossimplugins::ossimRadarSatModel *) model;
      std::list<ossimGpt> listePtsSol;
      std::list<ossimDpt> listePtsImage;
      // le point d'appui : le centre scène
      ossimDpt * imageGCP;
      ossimGpt * groundGCP;
      const char* i_str0 = geom.find("cornersCol0");
      int i0 = atoi(i_str0);
      const char* j_str0 = geom.find("cornersLin0");
      int j0 = atoi(j_str0);
      const char* lon_str0 = geom.find("cornersLon0");
      double lon0 = atof(lon_str0);
      const char* lat_str0 = geom.find("cornersLat0");
      double lat0 = atof(lat_str0);
      const char* height_str0 = geom.find("terrain_h");
      double height0 = atof(height_str0);

      imageGCP = new ossimDpt(i0, j0);
      groundGCP = new ossimGpt(lat0, lon0, height0);
      listePtsSol.push_back(*groundGCP);
      listePtsImage.push_back(*imageGCP);

      RDSmodel->optimizeModel(listePtsSol, listePtsImage);

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol0");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin0");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon0");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat0");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;

        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        ossimGpt * groundGCP = new ossimGpt(lat, lon, height);
        model->worldToLineSample(*groundGCP, imageret);
        std::cout << "Loc inverse des vraies coords geo : " << std::endl;
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;
        std::cout << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol1");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin1");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon1");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat1");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol2");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin2");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon2");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat2");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol3");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin3");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon3");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat3");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }

      /*
       * Localisation du point d'appui
       */
      if (model != NULL)
      {
        std::cout << "**** loc point d'appui ****" << std::endl;

        const char* i_str = geom.find("cornersCol4");
        int i = atoi(i_str);
        const char* j_str = geom.find("cornersLin4");
        int j = atoi(j_str);
        const char* lon_str = geom.find("cornersLon4");
        double lon = atof(lon_str);
        const char* lat_str = geom.find("cornersLat4");
        double lat = atof(lat_str);

        ossimDpt image(i, j);
        ossimDpt imageret;
        ossimGpt world;
        const char* height_str = geom.find("terrain_h");
        double height = atof(height_str);
        model->lineSampleHeightToWorld(image, height, world);
        std::cout << "Coordonnees de depart : x = " << i << " y = " << j << std::endl;
        std::cout << " latitude = " << world.lat << " longitude = " << world.lon << std::endl;

        std::cout << "altitude : " << world.height() << std::endl;
        model->worldToLineSample(world, imageret);
        std::cout << "x = " << imageret.x << " y = " << imageret.y << std::endl;

        std::cout << "Resultat attendu : " << std::endl;
        std::cout << "lat = " << lat << ", lon = " << lon << std::endl;
        std::cout << "    erreur lat =" << world.lat - lat << " , erreur lon =" << world.lon - lon << std::endl;
        std::cout << std::endl;
      }
    }
    {
      //    // ouvertures
      //    hid_t fileID, group_ID, attr_ID1, attr_ID2, attr_ID3, attr_ID4, attr_ID5, attr_ID6, attr_ID7, dataset_ID, mem_type_id ;
      //    herr_t status ;
      //    fileID = H5Fopen("D:\\locSAR\\exemple_CSKS\\hdf5_test.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
      //    group_ID = H5Gopen(fileID, "/links/hard links"  ) ;
      //    dataset_ID = H5Dopen(group_ID, "Eskimo"  ) ;
      //    attr_ID1 = H5Aopen_name(dataset_ID , "IMAGE_TRANSPARENCY"  ) ;
      //    attr_ID2 = H5Aopen_name(dataset_ID , "IMAGE_MINMAXRANGE"  ) ;
      //    attr_ID3 = H5Aopen_name(dataset_ID , "CLASS"  ) ;
      //    attr_ID4 = H5Aopen_name(dataset_ID , "IMAGE_VERSION"  ) ;
      //    attr_ID5 = H5Aopen_name(dataset_ID , "ajoutVMN"  ) ;
      //    attr_ID6 = H5Aopen_name(dataset_ID , "ajoutVMN_tabInt"  ) ;
      //    attr_ID7 = H5Aopen_name(dataset_ID , "ajoutVMN_double"  ) ;
      //
      //    // lectures
      //    mem_type_id = H5Aget_type(attr_ID1) ;
      //    unsigned int buffer1[1];
      //    status = H5Aread(attr_ID1, mem_type_id, buffer1 ) ;
      //    std::cout << buffer1[0] << std::endl ;
      //
      //    mem_type_id = H5Aget_type(attr_ID2) ;
      //    unsigned char buffer2[2] ;
      //    status = H5Aread(attr_ID2, mem_type_id, buffer2 ) ;
      //    std::cout << (int) buffer2[0] << std::endl ;
      //    std::cout << (int) buffer2[1] << std::endl ;
      //
      //    mem_type_id = H5Aget_type(attr_ID3) ;
      //    char buffer3[6] ;
      //    status = H5Aread(attr_ID3, mem_type_id, buffer3 ) ;
      //    char buffer4[10] ;
      //    status = H5Aread(attr_ID3, mem_type_id, buffer4 ) ;
      //    std::string classe(buffer4);
      //    std::cout << classe << std::endl ;
      //
      //    mem_type_id = H5Aget_type(attr_ID4) ;
      //    float buffer5[1] ;
      //    status = H5Aread(attr_ID4, mem_type_id, buffer5 ) ;
      //    std::cout << buffer5[0] << std::endl ;
      //
      //mem_type_id = H5Aget_type(attr_ID5) ;
      //    int buffer6[1] ;
      //    status = H5Aread(attr_ID5, mem_type_id, buffer6 ) ;
      //std::cout << buffer6[0] << std::endl ;
      //
      //mem_type_id = H5Aget_type(attr_ID6) ;
      //    int buffer7[2] ;
      //    status = H5Aread(attr_ID6, mem_type_id, buffer7 ) ;
      //std::cout << buffer7[0] << std::endl ;
      //std::cout << buffer7[1] << std::endl ;
      //
      //mem_type_id = H5Aget_type(attr_ID7) ;
      //    double buffer8[1] ;
      //    status = H5Aread(attr_ID7, mem_type_id, buffer8 ) ;
      //std::cout << buffer8[0] << std::endl ;
      //
      //    // fermeture
      //    status = H5Aclose(attr_ID1) ;
      //    status = H5Aclose(attr_ID2) ;
      //    status = H5Aclose(attr_ID3) ;
      //    status = H5Aclose(attr_ID4) ;
      //    status = H5Dclose(dataset_ID) ;
      //    status = H5Gclose(group_ID) ;
      //    status = H5Fclose(fileID) ;
    }

  }

  catch (std::bad_alloc & err)
  {
    std::cout << "Exception bad_alloc : " << (char*) err.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...)
  {
    std::cout << "Exception levee inconnue !" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}//Fin main()
