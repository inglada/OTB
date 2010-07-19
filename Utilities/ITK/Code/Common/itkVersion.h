/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVersion.h,v $
  Language:  C++
  Date:      $Date: 2010-07-12 02:00:10 $
  Version:   $Revision: 1.3444 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVersion_h
#define __itkVersion_h

#include "itkObject.h"
#include "itkObjectFactory.h"

#define ITK_VERSION_TO_STRING(x) ITK_VERSION_TO_STRING0(x)
#define ITK_VERSION_TO_STRING0(x) #x
#define ITK_VERSION ITK_VERSION_TO_STRING(ITK_VERSION_MAJOR) "." \
                    ITK_VERSION_TO_STRING(ITK_VERSION_MINOR) "." \
                    ITK_VERSION_TO_STRING(ITK_VERSION_PATCH)
#define ITK_SOURCE_VERSION "itk version " ITK_VERSION ", itk source $Revision: 1.3444 $, $Date: 2010-07-12 02:00:10 $ (GMT)"

namespace itk
{
/** \class Version
 * \brief Track the current version of the software.
 *
 * Holds methods for defining/determining the current itk version
 * (major, minor, build).
 *
 * This file will change frequently to update the ITKSourceVersion which
 * timestamps a particular source release.
 *
 * \ingroup ITKSystemObjects
 */

class ITKCommon_EXPORT Version : public Object 
{
public:
  /** Standard class typedefs. */
  typedef Version                   Self;
  typedef Object                    Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Standard part of every itk Object. */
  itkTypeMacro(Version,Object);

  /** Return the version of itk this object is a part of.
   * A variety of methods are included. GetITKSourceVersion returns a string
   * with an identifier which timestamps a particular source tree.  */
  static const char *GetITKVersion();
  static int GetITKMajorVersion();
  static int GetITKMinorVersion();
  static int GetITKBuildVersion();
  static const char *GetITKSourceVersion();
    
protected:
  Version();
  ~Version();

private:
  Version(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif 
