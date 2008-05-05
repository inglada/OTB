/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDeformableSimplexMesh3DGradientConstraintForceFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-02-03 04:05:28 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDeformableSimplexMesh3DGradientConstraintForceFilter_txx
#define __itkDeformableSimplexMesh3DGradientConstraintForceFilter_txx

#include "itkDeformableSimplexMesh3DGradientConstraintForceFilter.h"
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

#include <set>

namespace itk
{

/* Constructore  */
template <typename TInputMesh, typename TOutputMesh>
DeformableSimplexMesh3DGradientConstraintForceFilter<TInputMesh, TOutputMesh>
::DeformableSimplexMesh3DGradientConstraintForceFilter()
{
  m_Range = 1;
  m_StartVoxel = NULL;
}

template <typename TInputMesh, typename TOutputMesh>
DeformableSimplexMesh3DGradientConstraintForceFilter<TInputMesh, TOutputMesh>
::~DeformableSimplexMesh3DGradientConstraintForceFilter()
{
  this->Clear();
}


template <typename TInputMesh, typename TOutputMesh>
void
DeformableSimplexMesh3DGradientConstraintForceFilter<TInputMesh, TOutputMesh>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template <typename TInputMesh, typename TOutputMesh>
void
DeformableSimplexMesh3DGradientConstraintForceFilter<TInputMesh, TOutputMesh>
::Clear()
{
  delete m_StartVoxel;
  m_StartVoxel = 0;
  std::vector<ImageVoxel *>::iterator it;
  for(it = m_Positive.begin(); it != m_Positive.end(); it++)
    {
    delete *it;
    }
  m_Positive.erase(m_Positive.begin(), m_Positive.end());
  for(it = m_Negative.begin(); it != m_Negative.end(); it++)
    {
    delete *it;
    }
  m_Negative.erase(m_Negative.begin(), m_Negative.end());
}

template <typename TInputMesh, typename TOutputMesh>
int
DeformableSimplexMesh3DGradientConstraintForceFilter<TInputMesh, TOutputMesh>
::Signi(double a)
{
  if (a < 0)
    {
    return -1;
    }
  else if (a > 0 )
    {
    return 1;
    }
  else
    {
    return 0;
    }
}

template <typename TInputMesh, typename TOutputMesh>
double
DeformableSimplexMesh3DGradientConstraintForceFilter<TInputMesh, TOutputMesh>
::NextVoxel(const double* pp, int *ic, double *x, double *y, double *z)
{
  double d,dlx,dly,dlz;
  double dp[3];
  dp[0] = pp[0];
  dp[1] = pp[1];
  dp[2] = pp[2];

  if(dp[0] >= 0.0)
    {
    dlx = ((ic[0] + 1) * 1 - *x) / dp[0];
    if(dlx <= 0)
      {
      dlx = ((ic[0] + 2) * 1 - *x) / dp[0];
      }
    }
  else
    {
    dlx = (ic[0] * 1 - *x) / dp[0];
    if(dlx <= 0)
      {
      dlx = ((ic[0] - 1) * 1 - *x) / dp[0];
      }
    }

  if(dp[1] >= 0.0)
    {
    dly = ((ic[1] + 1) * 1 - *y) / dp[1];
    if(dly <= 0)
      {
      dly = ((ic[1] + 2) * 1 - *y) / dp[1];
      }
    }
  else
    {
    dly = (ic[1] * 1 - *y) / dp[1];
    if(dly <= 0)
      {
      dly = ((ic[1] - 1) * 1 - *y) / dp[1];
      }
    }

  if(dp[2] >= 0.0)
    {
    dlz = ((ic[2] + 1) * 1 - *z) / dp[2];
    if(dlz <= 0)
      {
      dlz = ((ic[2] + 2) * 1 - *z) / dp[2];
      }
    }
  else
    {
    dlz = (ic[2] * 1 - *z) / dp[2];
    if(dlz <= 0)
      {
      dlz = ((ic[2] - 1) * 1 - *z) / dp[2];
      }
    }

  if ((dlx<dly) && (dlx<dlz))
    {
    d = dlx;
    ic[0] += this->Signi(dp[0]);
    }
  else if ((dly<dlz) && (dly<dlx))
    {
    d = dly;
    ic[1] += this->Signi(dp[1]);
    }
  else
    {
    d = dlz;
    ic[2] += this->Signi(dp[2]);
    }

  *x += pp[0]*d;
  *y += pp[1]*d;
  *z += pp[2]*d;

  return d;
}

template <typename TInputMesh, typename TOutputMesh>
void
DeformableSimplexMesh3DGradientConstraintForceFilter<TInputMesh, TOutputMesh>
::ComputeExternalForce(SimplexMeshGeometry * data)
{

  PointType vec_for;

  // image coordinate
  int ic[3];
  GradientIndexType coord;
  double x,y,z;

  coord[0] = static_cast<ImageIndexValueType>(data->pos[0]);
  coord[1] = static_cast<ImageIndexValueType>(data->pos[1]);
  coord[2] = static_cast<ImageIndexValueType>(data->pos[2]);

  const OriginalImageType::PointType& orgn = m_Image->GetOrigin();
  const OriginalImageType::SpacingType& sp = m_Image->GetSpacing();

  if (m_StartVoxel)
    {
    this->Clear();
    }

  OriginalImageIndexType index;
  // ***keep in mind to add direction cosines in here once we have them
  x = data->pos[0] - orgn[0];
  y = data->pos[1] - orgn[1];
  z = data->pos[2] - orgn[2];

  //need to divide by image spacing if it was not one
  ic[0] = static_cast <ImageIndexValueType>(x / sp[0]);
  ic[1] = static_cast <ImageIndexValueType>(y / sp[1]);
  ic[2] = static_cast <ImageIndexValueType>(z / sp[2]);

  if ( ic[0] >= 0  &&
       ic[0] < this->m_ImageWidth &&
       ic[1] >= 0  &&
       ic[1] < this->m_ImageHeight &&
       ic[2] >= 0  &&
       ic[2] < this->m_ImageDepth)
    {
    bool stop;
    SIDE side = BOTH; //make sure you can set half segment as well but for noe we just set it to full segment
    int vpos[3],ii;
    double dist;
    double dp[3];
    double pos[3];

    ImageVoxel *current;
    vpos[0] = ic[0]; vpos[1] = ic[1]; vpos[2] = ic[2];

    if (data->normal[0] == 0)
      {
      dp[0] = 1e-6;
      }
    else
      {
      dp[0] = data->normal[0];
      }

    if (data->normal[1] == 0)
      {
      dp[1] = 1e-6;
      }
    else
      {
      dp[1] = data->normal[1];
      }

    if (data->normal[2] == 0)
      {
      dp[2] = 1e-6;
      }
    else
      {
      dp[2] = data->normal[2];
      }

    index[0] = ic[0]; index[1]=ic[1]; index[2]=ic[2];
    pos[0] = data->pos[0]; pos[1] = data->pos[1]; pos[2] = data->pos[2];

    m_StartVoxel = new ImageVoxel(vpos,  pos, (double)m_Image->GetPixel(index), 0.0, 0);
    current = new ImageVoxel(vpos,  pos, (double)m_Image->GetPixel(index), 0.0, 0);
    m_Positive.push_back(current);

    // scan normal side
    if (side== NORMAL || side == BOTH)
      {
      int i=0;
      stop = false;
      dist = 0.0;

      while (!stop)
        {

        double a = NextVoxel(dp, ic, &x,&y,&z);

        pos[0] += a * dp[0];
        pos[1] += a * dp[1];
        pos[2] += a * dp[2];
        dist += a;

        stop = ((dist > m_Range) ||
                !(ic[0] >= 0  &&
                  ic[0] < this->m_ImageWidth &&
                  ic[1] >= 0  &&
                  ic[1] < this->m_ImageHeight &&
                  ic[2] >= 0  &&
                  ic[2] < this->m_ImageDepth));

        if(!stop)
          {
          vpos[0] = ic[0]; vpos[1] = ic[1]; vpos[2] = ic[2];
          index[0] = ic[0]; index[1]=ic[1]; index[2]=ic[2];

          current = new ImageVoxel(vpos, pos, (double)m_Image->GetPixel(index), dist, ++i);
          m_Positive.push_back(current);
          if (current->GetDistance() > m_Range)
            {
            stop = true;
            }
          else
            {
            stop = false;
            }
          }
        }
      }

    // scan inverse normal side
    if(side == INVERSE || side == BOTH)
      {
      pos[0] = data->pos[0]; pos[1] = data->pos[1]; pos[2] = data->pos[2];
      // ***keep in mind to add direction cosines in here once we have them
      x = data->pos[0] - orgn[0];
      y = data->pos[1] - orgn[1];
      z = data->pos[2] - orgn[2];
      //need to divide by image spacing if it was not one
      ic[0] = static_cast <ImageIndexValueType>(x / sp[0]);
      ic[1] = static_cast <ImageIndexValueType>(y / sp[1]);
      ic[2] = static_cast <ImageIndexValueType>(z / sp[2]);

      dp[0] *= -1.0;  dp[1] *= -1.0; dp[2] *= -1.0;
      ii = 0;
      stop = false;
      dist = 0.0;

      while(!stop)
        {

        double a = NextVoxel(dp, ic, &x,&y,&z);

        pos[0] += a * dp[0];
        pos[1] += a * dp[1];
        pos[2] += a * dp[2];
        dist += a;

        stop = ((dist > m_Range) ||
                !(ic[0] >= 0  &&
                  ic[0] < this->m_ImageWidth &&
                  ic[1] >= 0  &&
                  ic[1] < this->m_ImageHeight &&
                  ic[2] >= 0  &&
                  ic[2] < this->m_ImageDepth));

        if(!stop)
          {
          vpos[0] = ic[0]; vpos[1] = ic[1]; vpos[2] = ic[2];
          index[0] = ic[0]; index[1]=ic[1]; index[2]=ic[2];

          current = new ImageVoxel(vpos, pos, (double)m_Image->GetPixel(index), dist, --ii);
          m_Negative.push_back(current);
          if (current->GetDistance() > m_Range)
            {
            stop = true;
            }
          else
            {
            stop = false;
            }
          }
        }
      }
    }
  else
    {
    m_StartVoxel = 0;
    }

  // now fun begins try to use all the above
  std::vector<ImageVoxel *>::iterator it;
  double mag, max=0;
  GradientIndexType coord3, coord2;
  coord2.Fill(0);
  for(it = m_Positive.begin(); it != m_Positive.end(); it++)
    {
    coord3[0] = static_cast<GradientIndexValueType>((*it)->GetX());
    coord3[1] = static_cast<GradientIndexValueType>((*it)->GetY());
    coord3[2] = static_cast<GradientIndexValueType>((*it)->GetZ());

    vec_for[0] = this->m_Gradient->GetPixel(coord3)[0];
    vec_for[1] = this->m_Gradient->GetPixel(coord3)[1];
    vec_for[2] = this->m_Gradient->GetPixel(coord3)[2];
    // check magnitude

    mag = vcl_sqrt(dot_product(vec_for.Get_vnl_vector(),vec_for.Get_vnl_vector()));
    if (mag > max)
      {
      max =  mag;
      coord2 = coord3;
      }

    }
  vec_for[0] = this->m_Gradient->GetPixel(coord2)[0];
  vec_for[1] = this->m_Gradient->GetPixel(coord2)[1];
  vec_for[2] = this->m_Gradient->GetPixel(coord2)[2];

  // now check highest gradient magnitude direction
  mag = dot_product(vec_for.Get_vnl_vector(), data->normal.Get_vnl_vector());

  if (mag > 0 )
    {
    vec_for[0] -= this->m_Gradient->GetPixel(coord)[0];
    vec_for[1] -= this->m_Gradient->GetPixel(coord)[1];
    vec_for[2] -= this->m_Gradient->GetPixel(coord)[2];
    }
  else
    {
    vec_for.Fill(0);
    }

  data->externalForce[0] = vec_for[0];
  data->externalForce[1] = vec_for[1];
  data->externalForce[2] = vec_for[2];
}

} // namespace itk

#endif
