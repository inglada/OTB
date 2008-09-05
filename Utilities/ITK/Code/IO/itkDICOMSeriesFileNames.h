/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDICOMSeriesFileNames.h,v $
  Language:  C++
  Date:      $Date: 2007-03-29 18:39:27 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDICOMSeriesFileNames_h
#define __itkDICOMSeriesFileNames_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkExceptionObject.h"
#include <vector>

#include "DICOMParser.h"
#include "DICOMAppHelper.h"

namespace itk
{

/** \class DICOMSeriesFileNames
 * \brief Generate an ordered sequence of filenames.
 *
 * This class generates an ordered sequence of filenames based on the
 * DICOM tags in the files. Files can be sorted based on image number,
 * slice location, or patient position. The files in the specified
 * directory are grouped by SeriesUID.  The list of SeriesUIDs can be
 * queried and the filenames for a specific series extracted.
 *
 * \ingroup IOFilters
 *
 */

class ITK_EXPORT DICOMSeriesFileNames : public Object
{
public:
  /** Standard class typedefs. */
  typedef DICOMSeriesFileNames    Self;
  typedef Object                  Superclass;
  typedef SmartPointer<Self>      Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DICOMSeriesFileNames, Object);

  /* -------- Define the API for DICOMSeriesFileNames ----------- */

  /** Set the directory containing the DICOM
   * files. DICOMSeriesFileNames caches information about filenames
   * and series UIDs. Calling SetDirectory() causes this information
   * to be cleared. This is the behavior even if SetDirectory() is
   * called with the same directory as the previous call.  In this
   * case, a second call to SetDirectory() with the same directory
   * name forces the directory to be rescanned.  This is useful if
   * DICOM files have been added to a directory since the last time
   * the directory was scanned. */
  void SetDirectory(const std::string& dir)
    {
    if (m_Directory != dir)
      {
      m_Directory = dir;
      this->Modified();

      // Clear the SeriesUIDs and FileNames
      m_SeriesUIDs.clear();
      m_FileNames.clear();
      m_AppHelper.Clear();
      }

    // Keep track of when the directory name was set so we can
    // compare it when the directory was last scanned. We set this
    // modified time each time SetDirectory() is called. This allows
    // a call to SetDirectory() to force a directory to be rescanned
    // the next time GetFileNames() or GetSeriesUIDs() is called.
    m_DirectorySetTime.Modified();
    }

  /** Get the directory containing the DICOM files. */
  itkGetStringMacro(Directory);

  /** The sorting order can be ascending or descending. The default
   * sort order is ascending. */
  itkSetMacro(Ascending, bool);
  itkGetMacro(Ascending, bool);
  itkBooleanMacro(Ascending);

  /** Returns a vector containing the UIDs for each series in the
   * directory. If parameter "recursive" is true, subdirectories will
   * be scanned. */
  const std::vector<std::string> &GetSeriesUIDs(bool recursive = false);
  
  /** Returns a vector containing the Descriptions for each series in the
   * directory. GetSeriesUIDs() should be called before */
  const std::vector<std::string> &GetSeriesDescriptions(){return m_SeriesDescriptions;}
  const std::vector<std::string> &GetSeriesBodyParts(){return m_BodyParts;}
  const std::vector<std::string> &GetSeriesScanOptions(){return m_ScanOptions;}

  /** Returns a vector containing the series file names. The file
   * names are sorted based on the current sorting mode. If parameter
   * "recursive" is true, subdirectories will be scanned. */
  const std::vector<std::string> &GetFileNames (bool recursive = false);

  /** Returns a vector containing the file names for a specified
   * series UID. The file names are sorted based on the current
   * sorting mode. If parameter "recursive" is true, subdirectories
   * will be scanned. */
  const std::vector<std::string> &GetFileNames (const std::string& seriesUID,
                                                bool recursive = false);
  
  /** Set the filename sorting order to sorting images based on the
   * DICOM field of slice number, the DICOM field of slice location,
   * or the position of the image computed using the
   * ImagePositionPatient and ImageOrientationPatient DICOM fields. */
  typedef enum {SortByImageNumber, SortBySliceLocation, SortByImagePositionPatient} FileNameSortingOrderType;
  itkSetEnumMacro(FileNameSortingOrder, FileNameSortingOrderType);
  itkGetEnumMacro(FileNameSortingOrder, FileNameSortingOrderType);
  void SetFileNameSortingOrderToSortByImageNumber()
    { this->SetFileNameSortingOrder(SortByImageNumber); }
  void SetFileNameSortingOrderToSortBySliceLocation()
    { this->SetFileNameSortingOrder(SortBySliceLocation); }
  void SetFileNameSortingOrderToSortByImagePositionPatient()
    { this->SetFileNameSortingOrder(SortByImagePositionPatient); }

  /** Get the filename associated with a specific instance UID. This
   * requires the internal database has already been built via a call
   * to GetFileNames() */
  std::string GetFileName(const std::string& instanceUID);
  

protected:
  DICOMSeriesFileNames();
  ~DICOMSeriesFileNames() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
private:
  DICOMSeriesFileNames(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  int CanReadFile(const char* fname);
  void RecurseDirectory( std::string directory, std::vector<std::string> &filenames);

  itkdicomparser::DICOMParser    m_Parser;
  itkdicomparser::DICOMAppHelper m_AppHelper;

  bool                     m_Ascending;
  std::string              m_Directory;
  std::vector<std::string> m_FileNames;
  std::vector<std::string> m_SeriesUIDs;
  std::vector<std::string> m_SeriesDescriptions;
  std::vector<std::string> m_BodyParts;
  std::vector<std::string> m_ScanOptions;

  FileNameSortingOrderType m_FileNameSortingOrder;

  TimeStamp  m_DirectorySetTime;
  TimeStamp  m_DirectoryScanTime;
};

} //namespace ITK

#endif // __itkDICOMSeriesFileNames_h
