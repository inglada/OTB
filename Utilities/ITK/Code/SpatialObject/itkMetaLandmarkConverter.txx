/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMetaLandmarkConverter.txx,v $
  Language:  C++
  Date:      $Date: 2007/08/17 18:08:09 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMetaLandmarkConverter_txx
#define __itkMetaLandmarkConverter_txx

#include "itkMetaLandmarkConverter.h"

namespace itk  
{

/** Constructor */ 
template <unsigned int NDimensions>
MetaLandmarkConverter<NDimensions>
::MetaLandmarkConverter()
{
  
}


/** Convert a metaLandmark into an Landmark SpatialObject  */
template <unsigned int NDimensions>
typename MetaLandmarkConverter<NDimensions>::SpatialObjectPointer
MetaLandmarkConverter<NDimensions>
::MetaLandmarkToLandmarkSpatialObject(MetaLandmark * Landmark)
{ 

  typedef itk::LandmarkSpatialObject<NDimensions> LandmarkSpatialObjectType;
  typename LandmarkSpatialObjectType::Pointer landmark = 
    LandmarkSpatialObjectType::New();
  
  //typedef LandmarkSpatialObjectType::VectorType VectorType;
  typedef vnl_vector<double> VectorType;

  unsigned int ndims = Landmark->NDims();
  double spacing[NDimensions];
  for(unsigned int i=0;i<ndims;i++)
    {
    spacing[i]=Landmark->ElementSpacing()[i];
    }
  landmark->GetIndexToObjectTransform()->SetScaleComponent(spacing);
  landmark->GetProperty()->SetName(Landmark->Name());
  landmark->SetId(Landmark->ID());
  landmark->SetParentId(Landmark->ParentID());
  landmark->GetProperty()->SetRed(Landmark->Color()[0]);
  landmark->GetProperty()->SetGreen(Landmark->Color()[1]);
  landmark->GetProperty()->SetBlue(Landmark->Color()[2]);
  landmark->GetProperty()->SetAlpha(Landmark->Color()[3]);

  typedef itk::SpatialObjectPoint<NDimensions> LandmarkPointType;
  
  typedef MetaLandmark::PointListType ListType;
  ListType::iterator it2 = Landmark->GetPoints().begin();
    
  vnl_vector<double> v(ndims);
  
  for(unsigned int identifier=0;identifier< Landmark->GetPoints().size();identifier++)
    {
    LandmarkPointType pnt;
    
    typedef typename LandmarkSpatialObjectType::PointType PointType;
    PointType point;

    for(unsigned int i=0;i<ndims;i++)
      {
      point[i]=(*it2)->m_X[i];
      }

    pnt.SetPosition(point);

    pnt.SetRed((*it2)->m_Color[0]);
    pnt.SetGreen((*it2)->m_Color[1]);
    pnt.SetBlue((*it2)->m_Color[2]);
    pnt.SetAlpha((*it2)->m_Color[3]);

    landmark->GetPoints().push_back(pnt);
    it2++;
    }
 
  return landmark;
}

/** Convert an Landmark SpatialObject into a metaLandmark */
template <unsigned int NDimensions>
MetaLandmark*
MetaLandmarkConverter<NDimensions>
::LandmarkSpatialObjectToMetaLandmark(SpatialObjectType * spatialObject)
{ 
  MetaLandmark* Landmark = new MetaLandmark(NDimensions);

  // fill in the Landmark information
   
  typename SpatialObjectType::PointListType::const_iterator i;
  for(i = dynamic_cast<SpatialObjectType*>(spatialObject)->GetPoints().begin(); 
      i != dynamic_cast<SpatialObjectType*>(spatialObject)->GetPoints().end(); 
      i++)
    {
    LandmarkPnt* pnt = new LandmarkPnt(NDimensions);

    for(unsigned int d=0;d<NDimensions;d++)
      {
      pnt->m_X[d]=(*i).GetPosition()[d];
      }
     
    pnt->m_Color[0] = (*i).GetRed();
    pnt->m_Color[1] = (*i).GetGreen();
    pnt->m_Color[2] = (*i).GetBlue();
    pnt->m_Color[3] = (*i).GetAlpha();
    Landmark->GetPoints().push_back(pnt); 
    }
    
  if(NDimensions == 2)
    {
    Landmark->PointDim("x y red green blue alpha");
    }
  else
    {
    Landmark->PointDim("x y z red green blue alpha");
    }

  float color[4];
  for(unsigned int i=0;i<4;i++)
    {
    color[i]=spatialObject->GetProperty()->GetColor()[i];
    }

  Landmark->Color(color);
  Landmark->ID( spatialObject->GetId());
  if(spatialObject->GetParent())
    {
    Landmark->ParentID(spatialObject->GetParent()->GetId());
    }
  Landmark->NPoints(Landmark->GetPoints().size());

  return Landmark;
}


/** Read a meta file give the type */
template <unsigned int NDimensions>
typename MetaLandmarkConverter<NDimensions>::SpatialObjectPointer
MetaLandmarkConverter<NDimensions>
::ReadMeta(const char* name)
{
  SpatialObjectPointer spatialObject;
  MetaLandmark* Landmark = new MetaLandmark();
  Landmark->Read(name);
  spatialObject = MetaLandmarkToLandmarkSpatialObject(Landmark);

  return spatialObject;
}


/** Write a meta Landmark file */
template <unsigned int NDimensions>
bool
MetaLandmarkConverter<NDimensions>
::WriteMeta(SpatialObjectType* spatialObject,const char* name)
{
  MetaLandmark* Landmark = LandmarkSpatialObjectToMetaLandmark(spatialObject);
  Landmark->BinaryData(true);
  Landmark->Write(name);
  return true;
}

} // end namespace itk 


#endif
