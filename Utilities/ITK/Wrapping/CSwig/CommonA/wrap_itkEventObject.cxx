/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: wrap_itkEventObject.cxx,v $
  Language:  C++
  Date:      $Date: 2005-03-25 13:17:57 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkEventObject.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkEventObjectGroup);
  namespace wrappers
  {
    typedef itk::EventObject itkEventObject;
    typedef itk::NoEvent itkNoEvent;
    typedef itk::AnyEvent itkAnyEvent;
    typedef itk::DeleteEvent itkDeleteEvent;
    typedef itk::StartEvent itkStartEvent;
    typedef itk::EndEvent itkEndEvent;
    typedef itk::ProgressEvent itkProgressEvent;
    typedef itk::ExitEvent itkExitEvent;
    typedef itk::ModifiedEvent itkModifiedEvent;
    typedef itk::IterationEvent itkIterationEvent;
    typedef itk::PickEvent itkPickEvent;
    typedef itk::StartPickEvent itkStartPickEvent;
    typedef itk::EndPickEvent itkEndPickEvent;
    typedef itk::AbortCheckEvent itkAbortCheckEvent;
    typedef itk::UserEvent itkUserEvent;
  }
}
#endif
