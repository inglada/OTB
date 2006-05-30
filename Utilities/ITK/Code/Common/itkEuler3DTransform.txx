/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkEuler3DTransform.txx,v $
  Language:  C++
  Date:      $Date: 2006/02/09 15:37:37 $
  Version:   $Revision: 1.20 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkEuler3DTransform_txx
#define _itkEuler3DTransform_txx

#include "itkEuler3DTransform.h"


namespace itk
{

// Constructor with default arguments
template <class TScalarType>
Euler3DTransform<TScalarType>
::Euler3DTransform():
  Superclass(SpaceDimension, ParametersDimension)
{
  m_ComputeZYX = false;
  m_AngleX = m_AngleY = m_AngleZ = 0.0;
}

// Constructor with arguments
template <class TScalarType>
Euler3DTransform<TScalarType>
::Euler3DTransform(unsigned int spaceDimension,
                   unsigned int parametersDimension):
  Superclass(spaceDimension, parametersDimension)
{
  m_ComputeZYX = false;
  m_AngleX = m_AngleY = m_AngleZ = 0.0;
}

// Set Parameters
template <class TScalarType>
void
Euler3DTransform<TScalarType>
::SetParameters( const ParametersType & parameters )
{
  itkDebugMacro( << "Setting parameters " << parameters );

  // Set angles with parameters
  m_AngleX = parameters[0];
  m_AngleY = parameters[1];
  m_AngleZ = parameters[2];
  this->ComputeMatrix();

  // Transfer the translation part
  OutputVectorType newTranslation;
  newTranslation[0] = parameters[3];
  newTranslation[1] = parameters[4];
  newTranslation[2] = parameters[5];
  this->SetVarTranslation(newTranslation);
  this->ComputeOffset();

  // Modified is always called since we just have a pointer to the
  // parameters and cannot know if the parameters have changed.
  this->Modified();

  itkDebugMacro(<<"After setting parameters ");
}


// Get Parameters
template <class TScalarType>
const typename Euler3DTransform<TScalarType>::ParametersType &
Euler3DTransform<TScalarType>
::GetParameters( void ) const
{
  this->m_Parameters[0] = m_AngleX;
  this->m_Parameters[1] = m_AngleY;
  this->m_Parameters[2] = m_AngleZ;
  this->m_Parameters[3] = this->GetTranslation()[0];
  this->m_Parameters[4] = this->GetTranslation()[1];
  this->m_Parameters[5] = this->GetTranslation()[2];

  return this->m_Parameters;
}

// Set Rotational Part
template <class TScalarType>
void
Euler3DTransform<TScalarType>
::SetRotation(ScalarType angleX,ScalarType angleY,ScalarType angleZ)
{
  m_AngleX = angleX;
  m_AngleY = angleY;
  m_AngleZ = angleZ;
  this->ComputeMatrix();
  this->ComputeOffset();
}

// Compose
template <class TScalarType>
void
Euler3DTransform<TScalarType>
::SetIdentity(void)
{
  Superclass::SetIdentity();
  m_AngleX = 0;
  m_AngleY = 0;
  m_AngleZ = 0;
}


// Compute angles from the rotation matrix
template <class TScalarType>
void
Euler3DTransform<TScalarType>
::ComputeMatrixParameters(void)
{
  if(m_ComputeZYX)
    {
    m_AngleY = -asin(this->GetMatrix()[2][0]);
    double C = cos(m_AngleY);
    if(fabs(C)>0.00005)
      {
      double x = this->GetMatrix()[2][2] / C;
      double y = this->GetMatrix()[2][1] / C;
      m_AngleX = atan2(y,x);
      x = this->GetMatrix()[0][0] / C;
      y = this->GetMatrix()[1][0] / C;
      m_AngleZ = atan2(y,x);
      }
    else
      {
      m_AngleX = 0;
      double x = this->GetMatrix()[1][1];
      double y = -this->GetMatrix()[0][1];
      m_AngleZ = atan2(y,x);
      }
    }
  else
    {
    m_AngleX = asin(this->GetMatrix()[2][1]);
    double A = cos(m_AngleX);
    if(fabs(A)>0.00005)
      {
      double x = this->GetMatrix()[2][2] / A;
      double y = -this->GetMatrix()[2][0] / A;
      m_AngleY = atan2(y,x);

      x = this->GetMatrix()[1][1] / A;
      y = -this->GetMatrix()[0][1] / A;
      m_AngleZ = atan2(y,x);
      }
    else
      {
      m_AngleZ = 0;
      double x = this->GetMatrix()[0][0];
      double y = this->GetMatrix()[1][0];
      m_AngleY = atan2(y,x);
      }
    }
  this->ComputeMatrix();
}


// Compute the matrix
template <class TScalarType>
void
Euler3DTransform<TScalarType>
::ComputeMatrix( void )
{
  // need to check if angles are in the right order
  const double cx = cos(m_AngleX);
  const double sx = sin(m_AngleX);
  const double cy = cos(m_AngleY);
  const double sy = sin(m_AngleY); 
  const double cz = cos(m_AngleZ);
  const double sz = sin(m_AngleZ);

  Matrix<TScalarType,3,3> RotationX;
  RotationX[0][0]=1;RotationX[0][1]=0;RotationX[0][2]=0;
  RotationX[1][0]=0;RotationX[1][1]=cx;RotationX[1][2]=-sx;
  RotationX[2][0]=0;RotationX[2][1]=sx;RotationX[2][2]=cx;


  Matrix<TScalarType,3,3> RotationY;
  RotationY[0][0]=cy;RotationY[0][1]=0;RotationY[0][2]=sy;
  RotationY[1][0]=0;RotationY[1][1]=1;RotationY[1][2]=0;
  RotationY[2][0]=-sy;RotationY[2][1]=0;RotationY[2][2]=cy;

  
  Matrix<TScalarType,3,3> RotationZ;
  RotationZ[0][0]=cz;RotationZ[0][1]=-sz;RotationZ[0][2]=0;
  RotationZ[1][0]=sz;RotationZ[1][1]=cz;RotationZ[1][2]=0;
  RotationZ[2][0]=0;RotationZ[2][1]=0;RotationZ[2][2]=1;

  /** Aply the rotation first around Y then X then Z */
  if(m_ComputeZYX)
    {
    this->SetVarMatrix(RotationZ*RotationY*RotationX);
    }
  else
    {
    // Like VTK transformation order
    this->SetVarMatrix(RotationZ*RotationX*RotationY);
    }
}


// Set parameters
template<class TScalarType>
const typename Euler3DTransform<TScalarType>::JacobianType &
Euler3DTransform<TScalarType>::
GetJacobian( const InputPointType & p ) const
{
  // need to check if angles are in the right order
  const double cx = cos(m_AngleX);
  const double sx = sin(m_AngleX);
  const double cy = cos(m_AngleY);
  const double sy = sin(m_AngleY); 
  const double cz = cos(m_AngleZ);
  const double sz = sin(m_AngleZ);

  this->m_Jacobian.Fill(0.0);

  const double px = p[0] - this->GetCenter()[0];
  const double py = p[1] - this->GetCenter()[1];
  const double pz = p[2] - this->GetCenter()[2];


  if ( m_ComputeZYX )
    {
    this->m_Jacobian[0][0] = (cz*sy*cx+sz*sx)*py+(-cz*sy*sx+sz*cx)*pz;
    this->m_Jacobian[1][0] = (sz*sy*cx-cz*sx)*py+(-sz*sy*sx-cz*cx)*pz;
    this->m_Jacobian[2][0] = (cy*cx)*py+(-cy*sx)*pz;  
    
    this->m_Jacobian[0][1] = (-cz*sy)*px+(cz*cy*sx)*py+(cz*cy*cx)*pz;
    this->m_Jacobian[1][1] = (-sz*sy)*px+(sz*cy*sx)*py+(sz*cy*cx)*pz;
    this->m_Jacobian[2][1] = (-cy)*px+(-sy*sx)*py+(-sy*cx)*pz;
    
    this->m_Jacobian[0][2] = (-sz*cy)*px+(-sz*sy*sx-cz*cx)*py
                                        +(-sz*sy*cx+cz*sx)*pz;
    this->m_Jacobian[1][2] = (cz*cy)*px+(cz*sy*sx-sz*cx)*py+(cz*sy*cx+sz*sx)*pz;  
    this->m_Jacobian[2][2] = 0;
    }
  else
    {
    this->m_Jacobian[0][0] = (-sz*cx*sy)*px + (sz*sx)*py + (sz*cx*cy)*pz;
    this->m_Jacobian[1][0] = (cz*cx*sy)*px + (-cz*sx)*py + (-cz*cx*cy)*pz;
    this->m_Jacobian[2][0] = (sx*sy)*px + (cx)*py + (-sx*cy)*pz;  
    
    this->m_Jacobian[0][1] = (-cz*sy-sz*sx*cy)*px + (cz*cy-sz*sx*sy)*pz;
    this->m_Jacobian[1][1] = (-sz*sy+cz*sx*cy)*px + (sz*cy+cz*sx*sy)*pz;
    this->m_Jacobian[2][1] = (-cx*cy)*px + (-cx*sy)*pz;
    
    this->m_Jacobian[0][2] = (-sz*cy-cz*sx*sy)*px + (-cz*cx)*py 
                                                  + (-sz*sy+cz*sx*cy)*pz;
    this->m_Jacobian[1][2] = (cz*cy-sz*sx*sy)*px + (-sz*cx)*py 
                                                 + (cz*sy+sz*sx*cy)*pz;
    this->m_Jacobian[2][2] = 0;
    }
 
  // compute derivatives for the translation part
  unsigned int blockOffset = 3;  
  for(unsigned int dim=0; dim < SpaceDimension; dim++ ) 
    {
    this->m_Jacobian[ dim ][ blockOffset + dim ] = 1.0;
    }

  return this->m_Jacobian;

}
  
// Print self
template<class TScalarType>
void
Euler3DTransform<TScalarType>::
PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Euler's angles: AngleX=" << m_AngleX  
     << " AngleY=" << m_AngleY  
     << " AngleZ=" << m_AngleZ  
     << std::endl;
  os << indent << "m_ComputeZYX = " << m_ComputeZYX << std::endl;
}

} // namespace

#endif
