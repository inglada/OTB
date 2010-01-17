/*=========================================================================

  Program:   MetaIO
  Module:    $RCSfile: metaEllipse.h,v $
  Language:  C++
  Date:      $Date: 2008-04-09 01:44:28 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "metaTypes.h"

#ifndef ITKMetaIO_METAELLIPSE_H
#define ITKMetaIO_METAELLIPSE_H

#include "metaUtils.h"
#include "metaObject.h"

#include <list>


/*!    MetaEllipse (.h and .cpp)
 *
 * Description:
 *    Reads and Writes MetaEllipseFiles.
 *
 * \author Julien Jomier
 * 
 * \date May 22, 2002
 * 
 * Depends on:
 *    MetaUtils.h
 *    MetaObject.h
 */

#if (METAIO_USE_NAMESPACE)
namespace METAIO_NAMESPACE {
#endif


class METAIO_EXPORT MetaEllipse : public MetaObject
  {

  /////
  //
  // PUBLIC
  //
  ////
  public:

    ////
    //
    // Constructors & Destructor
    //
    ////
    MetaEllipse(void);

    MetaEllipse(const char *_headerName);   

    MetaEllipse(const MetaEllipse *_ellipse);    

    MetaEllipse(unsigned int dim);

    ~MetaEllipse(void);

    void PrintInfo(void) const;

    void CopyInfo(const MetaObject * _object);

    void  Clear(void);
    
    void  Radius(const float* radius);
    void  Radius(float radius);
    void  Radius(float r1,float r2);
    void  Radius(float r1,float r2, float r3);
    const float* Radius(void) const;


  ////
  //
  // PROTECTED
  //
  ////
  protected:

    void  M_Destroy(void);

    void  M_SetupReadFields(void);

    void  M_SetupWriteFields(void);

    bool  M_Read(void);

    float m_Radius[100];  // "Radius = "     0

  };

#if (METAIO_USE_NAMESPACE)
};
#endif


#endif
