/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_ITKKernelDeformableTransforms.cxx,v $
  Language:  C++
  Date:      $Date: 2005-03-25 13:17:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkKernelTransform.h"
#include "itkElasticBodyReciprocalSplineKernelTransform.h"
#include "itkElasticBodySplineKernelTransform.h"
#include "itkThinPlateR2LogRSplineKernelTransform.h"
#include "itkThinPlateSplineKernelTransform.h"
#include "itkVolumeSplineKernelTransform.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"

#define ITK_WRAP_TRANSFORM_2(x, d1) \
  ITK_WRAP_OBJECT2(x, double, d1, itk##x##d1)
    
namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(ITKKernelDeformableTransforms);
  namespace wrappers
  {
    ITK_WRAP_TRANSFORM_2(ElasticBodyReciprocalSplineKernelTransform, 2);
    ITK_WRAP_TRANSFORM_2(ElasticBodyReciprocalSplineKernelTransform, 3);
    ITK_WRAP_TRANSFORM_2(ElasticBodySplineKernelTransform, 2);
    ITK_WRAP_TRANSFORM_2(ElasticBodySplineKernelTransform, 3);
    ITK_WRAP_TRANSFORM_2(KernelTransform, 2);
    ITK_WRAP_TRANSFORM_2(KernelTransform, 3);
    ITK_WRAP_TRANSFORM_2(ThinPlateR2LogRSplineKernelTransform, 2);
    ITK_WRAP_TRANSFORM_2(ThinPlateR2LogRSplineKernelTransform, 3);
    ITK_WRAP_TRANSFORM_2(ThinPlateSplineKernelTransform, 2);
    ITK_WRAP_TRANSFORM_2(ThinPlateSplineKernelTransform, 3);
    ITK_WRAP_TRANSFORM_2(VolumeSplineKernelTransform, 2);
    ITK_WRAP_TRANSFORM_2(VolumeSplineKernelTransform, 3);
  }
}
#endif
