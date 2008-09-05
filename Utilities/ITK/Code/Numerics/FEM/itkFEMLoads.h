/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMLoads.h,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:43 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/**
 * \file itkFEMLoads.h
 * \brief Include all finite element load classes defined in FEM toolkit.
 *
 * To make sure you have everything, just include this header file.
 */
#include "itkFEMLoadBase.h"
#include "itkFEMLoadNode.h"
#include "itkFEMLoadBC.h"
#include "itkFEMLoadBCMFC.h"
#include "itkFEMLoadElementBase.h"
#include "itkFEMLoadPoint.h"
#include "itkFEMLoadGrav.h"
#include "itkFEMLoadEdge.h"
#include "itkFEMImageMetricLoad.h"
#include "itkFEMImageMetricLoadImplementation.h"

#include "itkFEMLoadTest.h"
#include "itkFEMLoadImplementationTest.h"

#include "itkFEMLoadLandmark.h"
#include "itkFEMLoadImplementationGenericLandmarkLoad.h"
