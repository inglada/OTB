/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTestFDStream.cxx,v $
  Language:  C++
  Date:      $Date: 2009-11-24 15:16:01 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/*
   This file tests whether fdstream compiles
*/

#include "fdstream.hpp"

int main() 
{
  // stream with buffer reading from file descriptor 0 (standard input)
  boost::fdistream in(0);

  // stream with buffer writing to file descriptor 1 (standard output)
  boost::fdostream out(1);

  return 0;
}

