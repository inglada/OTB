/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMatlabTransformIO.cxx,v $
  Language:  C++
  Date:      $Date: 2007/08/09 15:15:59 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkMatlabTransformIO.h"
#include <itksys/SystemTools.hxx>
#include <vnl/vnl_matlab_read.h>
#include <vnl/vnl_matlab_write.h>

namespace itk
{
MatlabTransformIO::
MatlabTransformIO()
{
}

MatlabTransformIO::
~MatlabTransformIO()
{
}

bool
MatlabTransformIO::
CanReadFile(const char *fileName)
{
  return itksys::SystemTools::GetFilenameLastExtension(fileName) == ".mat";
  
}

bool
MatlabTransformIO::
CanWriteFile(const char *fileName)
{
  return itksys::SystemTools::GetFilenameLastExtension(fileName) == ".mat";
}

//
// ReadMat -- we always want double precision,
// but handle single precision as well.
static void 
ReadMat(vnl_matlab_readhdr &mathdr,
        itk::Array<double> &array)
{
  if(mathdr.is_single())
    {
    vnl_vector<float> fv(mathdr.rows());
    mathdr.read_data(fv.begin());
    for(int i = 0; i < mathdr.rows(); i++)
      {
      array[i] = fv[i];
      }
    }
  else
    {
    mathdr.read_data(array.begin());
    }
}

void 
MatlabTransformIO::
Read()
{
  std::ifstream matfile(this->m_FileName.c_str(),
                        std::ios::in|std::ios::binary);
  if(matfile.fail())
    {
    matfile.close();
    itkExceptionMacro("The file could not be opened for read access "
                      << std::endl << "Filename: \"" << m_FileName << "\"");
    }
  while(!matfile.eof())
    {
    vnl_matlab_readhdr mathdr(matfile);
    if(! mathdr )
      {
      break;
      }
    if(mathdr.cols() != 1)
      {
      matfile.close();
      itkExceptionMacro 
        ("Only vector parameters supported");
      }
    itk::Array<double> TmpParameterArray(mathdr.rows());
    ReadMat(mathdr,TmpParameterArray);
    std::string classname(mathdr.name());
    // create transform based on name of first vector
    TransformPointer transform;
    this->CreateTransform(transform,classname);
    this->m_ReadTransformList.push_back(transform);
    vnl_matlab_readhdr mathdr2(matfile);
    if(mathdr2.cols() != 1)
      {
      matfile.close();
      itkExceptionMacro 
        ("Only vector parameters supported");
      }
    itk::Array<double> TmpFixedParameterArray(mathdr2.rows());
    ReadMat(mathdr2,TmpFixedParameterArray);
    transform->SetFixedParameters(TmpFixedParameterArray);
    transform->SetParametersByValue(TmpParameterArray);
    }
  matfile.close();
}

void 
MatlabTransformIO::
Write()
{
  ConstTransformListType::iterator it = m_WriteTransformList.begin();
  vnl_vector<double> TempArray;
  std::ofstream out;
  this->OpenStream(out,true);
  while(it != m_WriteTransformList.end())
    {
    std::string xfrmType((*it)->GetTransformTypeAsString());
    TempArray = (*it)->GetParameters();
    vnl_matlab_write(out,TempArray.begin(),TempArray.size(),xfrmType.c_str());
    TempArray = (*it)->GetFixedParameters();
    vnl_matlab_write(out,TempArray.begin(),TempArray.size(),"fixed");
    it++;
    }
  out.close();
}

}
