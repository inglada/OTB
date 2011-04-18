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

#include "otbRPCProjectionWrapper.h"
#include "otbImageKeywordlist.h"

#include "projection/ossimRpcProjection.h"
#include "projection/ossimProjection.h"
#include "projection/ossimRpcSolver.h"
#include "imaging/ossimImageGeometry.h"

namespace otb
{

RPCProjectionWrapper::RPCProjectionWrapper()
{
  // Create projection
  m_RpcProjection = new ossimRpcProjection();
}

RPCProjectionWrapper::~RPCProjectionWrapper()
{
  if (m_RpcProjection != NULL)
    {
    delete m_RpcProjection;
    }
}

void
RPCProjectionWrapper::TransformPoint(double x, double y, double z,
                                     double& lon, double& lat, double& h) const
{
  if (m_RpcProjection == NULL)
    {
    lon = 0.0;
    lat = 0.0;
    h = 0.0;
    return;
    }
  ossimDpt spoint(x, y);
  ossimGpt gpoint;
  gpoint.hgt = z;

  m_RpcProjection->lineSampleToWorld(spoint, gpoint);
  lon = gpoint.lon;
  lat = gpoint.lat;
  h = gpoint.hgt;
}

void
RPCProjectionWrapper::Solve(const GCPsContainerType& gcpContainer,
                            double& rmsError, ImageKeywordlist& otb_kwl)
{
  // TODO: for now, this is a simple transfer of what was done in the
  // GCPsToRPCSensorModelImageFilter. We might find some way to improve that.

  // The vector where geo and sensor points are stored
  std::vector<ossimDpt> sensorPoints;
  std::vector<ossimGpt> geoPoints;

  // Temporary points variable
  ossimDpt sensorPoint;
  ossimGpt geoPoint;

  // Retrieve the additional GCPs
  typename GCPsContainerType::const_iterator gcpIt;
  for (gcpIt = gcpContainer.begin(); gcpIt != gcpContainer.end(); ++gcpIt)
    {
    // Fill sensor point
    sensorPoint = ossimDpt(gcpIt->first[0], gcpIt->first[1]);

    // Fill geo point (lat, lon, elev)
    geoPoint =  ossimGpt(gcpIt->second[1], gcpIt->second[0], gcpIt->second[2]);

    // Add the sensor point to the list
    sensorPoints.push_back(sensorPoint);

    // Add the geo point to the list
    geoPoints.push_back(geoPoint);
    }

  // Build the ossim rpc solver
  ossimRefPtr<ossimRpcSolver> rpcSolver = new ossimRpcSolver(true, false);

  // Call the solve method
  rpcSolver->solveCoefficients(sensorPoints, geoPoints);

  rmsError = rpcSolver->getRmsError();

  // Retrieve the output RPC projection
  ossimRefPtr<ossimRpcModel> rpcModel = dynamic_cast<ossimRpcModel*>(rpcSolver->createRpcModel()->getProjection());

  // Export the sensor model in an ossimKeywordlist
  ossimKeywordlist geom_kwl;
  rpcModel->saveState(geom_kwl);

  // Compute projection
  m_RpcProjection = dynamic_cast<ossimRpcProjection*>(
    rpcSolver->createRpcProjection()->getProjection());
  ossimKeywordlist kwl;
  m_RpcProjection->saveState(kwl);

  // Build an otb::ImageKeywordList
  otb_kwl.SetKeywordlist(geom_kwl);
}

} // namespace otb
