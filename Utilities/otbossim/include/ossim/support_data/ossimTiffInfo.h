//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: TIFF Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimTiffInfo_HEADER
#define ossimTiffInfo_HEADER

#include <iosfwd>
#include <string>
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

class ossimDpt;
class ossimEndian;
class ossimKeywordlist;
class ossimTieGptSet;

/**
 * @brief TIFF info class.
 *
 * Encapsulates the listgeo functionality.
 */
class OSSIM_DLL ossimTiffInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimTiffInfo();

   /** virtual destructor */
   virtual ~ossimTiffInfo();

   /**
    * Anonymous enumerations.  Note prefixed with 'O' to avoid conflict with
    * libtiff.
    */
   enum
   {
      UNDEFINED                           = 0,
      OPIXEL_IS_AREA                      = 1,
      OPIXEL_IS_POINT                     = 2,
      OTIFFTAG_SUBFILETYPE                = 254,
      OTIFFTAG_IMAGEWIDTH                 = 256,
      OTIFFTAG_IMAGELENGTH                = 257,
      OTIFFTAG_BITSPERSAMPLE              = 258,
      OTIFFTAG_COMPRESSION                = 259,
      OTIFFTAG_PHOTOMETRIC                = 262,
      OTIFFTAG_IMAGEDESCRIPTION           = 270,
      OTIFFTAG_MODEL                      = 272,
      OTIFFTAG_STRIPOFFSETS               = 273,
      OTIFFTAG_ORIENTATION                = 274,
      OTIFFTAG_SAMPLESPERPIXEL            = 277,
      OTIFFTAG_ROWSPERSTRIP               = 278,
      OTIFFTAG_STRIPBYTECOUNTS            = 279,
      OTIFFTAG_MINSAMPLEVALUE             = 280,
      OTIFFTAG_MAXSAMPLEVALUE             = 281,
      OTIFFTAG_XRESOLUTION                = 282,
      OTIFFTAG_YRESOLUTION                = 283,
      OTIFFTAG_PLANARCONFIG               = 284,
      OTIFFTAG_RESOLUTIONUNIT             = 296,
      OTIFFTAG_SOFTWARE                   = 305,
      OTIFFTAG_DATETIME                   = 306,
      OTIFFTAG_TILEWIDTH                  = 322,
      OTIFFTAG_TILELENGTH                 = 323,
      OTIFFTAG_TILEOFFSETS                = 324,
      OTIFFTAG_TILEBYTECOUNTS             = 325,
      OTIFFTAG_EXTRASAMPLES               = 338,
      OTIFFTAG_SAMPLEFORMAT               = 339,
      OTIFFTAG_SMINSAMPLEVALUE            = 340,
      OTIFFTAG_SMAXSAMPLEVALUE            = 341,
      OSAMPLEFORMAT_UINT                  = 1,
      OSAMPLEFORMAT_INT                   = 2,
      OSAMPLEFORMAT_IEEEFP                = 3,
      OSAMPLEFORMAT_VOID                  = 4,
      OSAMPLEFORMAT_COMPLEXINT            = 5,
      OSAMPLEFORMAT_COMPLEXIEEEFP         = 6,
      OTIFFTAG_SUBIFD                     = 330,
      OGT_MODEL_TYPE_GEO_KEY              = 1024,
      OGT_RASTER_TYPE_GEO_KEY             = 1025,
      OGT_CITATION_GEO_KEY                = 1026,
      OGEOGRAPHIC_TYPE_GEO_KEY            = 2048,
      OGEOG_CITATION_GEO_KEY              = 2049,
      OGEOG_GEODETIC_DATUM_GEO_KEY        = 2050,
      OGEOG_LINEAR_UNITS_GEO_KEY          = 2052,
      OGEOG_ANGULAR_UNITS_GEO_KEY         = 2054,
      OGEOG_ELLIPSOID_GEO_KEY             = 2056,
      OGEOG_SEMI_MAJOR_AXIS               = 2057,
      OGEOG_SEMI_MINOR_AXIS               = 2058,
      OPROJECTED_CS_TYPE_GEO_KEY          = 3072,
      OPCS_CITATION_GEO_KEY               = 3073,
      OPROJECTION_GEO_KEY                 = 3074,
      OPROJ_COORD_TRANS_GEO_KEY           = 3075,
      OLINEAR_UNITS_GEO_KEY               = 3076,
      OPROJ_STD_PARALLEL1_GEO_KEY         = 3078,
      OPROJ_STD_PARALLEL2_GEO_KEY         = 3079,
      OPROJ_NAT_ORIGIN_LONG_GEO_KEY       = 3080,
      OPROJ_NAT_ORIGIN_LAT_GEO_KEY        = 3081,
      OPROJ_FALSE_EASTING_GEO_KEY         = 3082,
      OPROJ_FALSE_NORTHING_GEO_KEY        = 3083,
      OPROJ_FALSE_ORIGIN_LONG_GEO_KEY     = 3084,
      OPROJ_FALSE_ORIGIN_LAT_GEO_KEY      = 3085,
      OPROJ_FALSE_ORIGIN_EASTING_GEO_KEY  = 3086,
      OPROJ_FALSE_ORIGIN_NORTHING_GEO_KEY = 3087,
      OPROJ_CENTER_LONG_GEO_KEY           = 3088,
      OPROJ_CENTER_LAT_GEO_KEY            = 3089,
      OPROJ_SCALE_AT_NAT_ORIGIN_GEO_KEY   = 3092,
      OLINEAR_METER                       = 9001,
      OLINEAR_FOOT                        = 9002,
      OLINEAR_FOOT_US_SURVEY              = 9003,
      OANGULAR_DEGREE                     = 9102,
      OANGULAR_ARC_MINUTE                 = 9103,
      OANGULAR_ARC_SECOND                 = 9104,
      OANGULAR_GRAD                       = 9105,
      OANGULAR_GON                        = 9106,
      OANGULAR_DMS                        = 9107,
      OANGULAR_DMS_HEMISPHERE             = 9108,
      OPCS_BRITISH_NATIONAL_GRID          = 27700,
      OUSER_DEFINED                       = 32767,
      OTIFFTAG_COPYRIGHT                  = 33432,
      OMODEL_PIXEL_SCALE_TAG              = 33550,
      OMODEL_TIE_POINT_TAG                = 33922,
      OMODEL_TRANSFORM_TAG                = 34264,
      OTIFFTAG_PHOTOSHOP                  = 34377,
      OGEO_KEY_DIRECTORY_TAG              = 34735,
      OGEO_DOUBLE_PARAMS_TAG              = 34736,
      OGEO_ASCII_PARAMS_TAG               = 34737
   };

   enum CompressType
   {
      NOT_COMPRESSED = 0,
      COMPRESSED     = 1
   };
 
   enum PhotoInterpretation
   {
      OPHOTO_MINISWHITE  = 0,   // min value is white 
      OPHOTO_MINISBLACK  = 1,   // min value is black 
      OPHOTO_RGB         = 2,   // RGB color model 
      OPHOTO_PALETTE     = 3,   // color map indexed 
      OPHOTO_MASK        = 4,   // $holdout mask 
      OPHOTO_SEPARATED   = 5,   // !color separations 
      OPHOTO_YCBCR       = 6,   // !CCIR 601 
      OPHOTO_CIELAB      = 8    // !1976 CIE L*a*b*
   };

   enum ModelType
   {
      UNKNOWN                = 0,
      OMODEL_TYPE_PROJECTED  = 1,  // Projection Coordinate System
      OMODEL_TYPE_GEOGRAPHIC = 2,  // Geographic latitude-longitude System 
      OMODEL_TYPE_GEOCENTRIC = 3
   };

   enum WordType
   {
      TWO_OR_EIGHT  = 0,
      FOUR_OR_EIGHT = 1
   };

   enum
   {
      OTIFF_NOTYPE = 0,      /* placeholder */
      OTIFF_BYTE = 1,        /* 8-bit unsigned integer */
      OTIFF_ASCII = 2,       /* 8-bit bytes w/ last byte null */
      OTIFF_SHORT = 3,       /* 16-bit unsigned integer */
      OTIFF_LONG = 4,        /* 32-bit unsigned integer */
      OTIFF_RATIONAL = 5,    /* 64-bit unsigned fraction */
      OTIFF_SBYTE = 6,       /* !8-bit signed integer */
      OTIFF_UNDEFINED = 7,   /* !8-bit untyped data */
      OTIFF_SSHORT = 8,      /* !16-bit signed integer */
      OTIFF_SLONG = 9,       /* !32-bit signed integer */
      OTIFF_SRATIONAL = 10,  /* !64-bit signed fraction */
      OTIFF_FLOAT = 11,      /* !32-bit IEEE floating point */
      OTIFF_DOUBLE = 12,     /* !64-bit IEEE floating point */
      OTIFF_IFD = 13,        /* %32-bit unsigned integer (offset) */
      OTIFF_LONG8 = 16,      /* BigTIFF 64-bit unsigned integer */
      OTIFF_SLONG8 = 17,     /* BigTIFF 64-bit signed integer */
      OTIFF_IFD8 = 18        /* BigTIFF 64-bit unsigned integer (offset) */
   };

   /**
    * @brief open method.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   virtual bool open(const ossimFilename& file);

   /**
    * Print method.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

   /**
    * @brief Print method.
    * 
    * Print method that takes a stream that should be positions at the start
    * of tiff stream. This was added to see embedded information on embedded
    * geotiff's in a geojp2 file.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ifstream& inStr,
                               std::ostream& outStr) const;

   /**
    *  @brief extracts geometry info to keyword list.  This method assumes
    *  that open has been called and theFile is set.
    *  
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *
    *  @param geomKwl Keyword list that will be initialized with geometry info.
    *  Returns true if geometry info is present, false if not.
    *
    *  @param entryIndex Entry to get geometry from. 
    */
   bool getImageGeometry(ossimKeywordlist& geomKwl,
                         ossim_uint32 entryIndex) const;
   /**
    *  @brief extracts geometry info from stream to keyword list.
    *  
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *
    *  @note that the str should be position at start of tiff portion. This
    *  was written to allow passing a stream positioned at an embedded tiff
    *  image in a jp2 file.
    *
    *  @param str Stream position at start of tiff image.
    *
    *  @param geomKwl Keyword list that will be initialized with geometry info.
    *  Returns true if geometry info is present, false if not.
    *
    *  @param entryIndex Entry to get geometry from. 
    */
   bool getImageGeometry(std::ifstream& str,
                         ossimKeywordlist& geomKwl,
                         ossim_uint32 entryIndex) const;

private:
   
   /** Initializes s reference.  Does byte swapping as needed. */
   void readShort(ossim_uint16& s, std::ifstream& str) const;
   
   /** Initializes l reference.  Does byte swapping as needed. */
   void readLong(ossim_uint32& l, std::ifstream& str) const;
   
   /** Initializes l reference.  Does byte swapping as needed. */
   void readLongLong(ossim_uint64& l, std::ifstream& str) const;
   
   /** Attempts to parse the pcs code (3072). */
   void parsePcsCode(int code);
   
   /** Attempts to parse the projection geo key (3074). */
   void parseProjGeoCode(int code);
   
   /**
    *  Converts double passed in to meters if needed.  The conversion is
    *  base on "theLiniarUnitsCode".
    */
   double convert2meters(double d) const;
   
   /**
    * This will read either 4 or 8 bytes depending on the version and
    * initialize offset.
    *
    * @return true if stream is good, false if not.
    */
   bool getOffset(std::streamoff& offset, std::ifstream& str,
                  ossim_uint16 version) const;
   
   /**
    * This will read either 2 , 4or 8 bytes depending on the version and
    * WORD_TYPE.
    *
    * @return true if stream is good, false if not.
    */
   bool getValue(ossim_uint64& val, std::ifstream& str, WordType type,
                 ossim_uint16 version) const;
   
   ossim_uint64 getArraySizeInBytes(ossim_uint64 length,
                                    ossim_uint16 type) const;
   
   /**
    * @return The byte size of each tiff type or 0 if unhandled.
    */
   ossim_uint16 getTypeByteSize(ossim_uint16 type) const;
   
   /**
    * Eats the value field.  Either 4 or 8 bytes depending on the version.
    */
   void eatValue(std::ifstream& str, ossim_uint16 version) const;
   
   void swapBytes(ossim_uint8* v, ossim_uint16 type, ossim_uint64 count) const;
   
   /** Initializes v. */
   template <class T> void getArrayValue(T& v,
                                         ossim_uint8* array,
                                         ossim_uint64 position) const;
   
   std::ostream& print(std::ostream& out,
                       const std::string&  prefix,
                       ossim_uint64   tagIdx,
                       ossim_uint16   tag,
                       ossim_uint16   type,
                       ossim_uint64   count,
                       ossim_uint64   arraySizeInBytes,
                       ossim_uint8*   valueArray) const;
   
   std::ostream& printValue(std::ostream& out,
                            ossim_uint16 type,
                            ossim_uint8* valueArray) const;
   
   std::ostream& printArray(std::ostream& out,
                            ossim_uint16 type,
                            ossim_uint64 count,
                            ossim_uint8* valueArray) const;
   
   /** @brief Prints geo keys. */
   std::ostream& printGeoKeys(std::ostream& out,
                              const std::string& prefix,
                              ossim_uint64   geoKeyLength,
                              ossim_uint16*  geoKeyBlock,
                              ossim_uint64   geoDoubleLength,
                              ossim_float64* geoDoubleBlock,
                              ossim_uint64   geoAsciiLength,
                              ossim_int8*    geoAsciiBlock) const;
   

   /** @brief Prints key 1024 */
   std::ostream& printModelType(std::ostream& out,
                                const std::string& prefix,
                                ossim_uint16 code) const;

   /** @brief Prints key 1025 */
   std::ostream& printRasterType(std::ostream& out,
                                 const std::string& prefix,
                                 ossim_uint16 code) const;

   /** @brief Prints key 2054 */
   std::ostream& printAngularUnits(std::ostream& out,
                                   const std::string& prefix,
                                   ossim_uint16 code) const;

   /** @brief Prints key 3075 */
   std::ostream& printCoordTrans(std::ostream& out,
                                  const std::string& prefix,
                                  ossim_uint16 code) const;

   /** @brief Prints key 3076 */
   std::ostream& printLinearUnits(std::ostream& out,
                                  const std::string& prefix,
                                  ossim_uint16 code) const;

   /** @brief Prints tag 274 */
   std::ostream& printOrientation(std::ostream& out,
                                  const std::string& prefix,
                                  ossim_uint16 code) const;
  
   /** @brief adds imageN. to prefix where N is zero base directory index. */
   void getDirPrefix(ossim_int32 dirIndex, std::string& prefix) const;

   /**
    * @brief Gets the required pixel scale from keyword list looking for the
    * key model_pixel_scale.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param scale The point to initialize.
    * @return true on success, false on error.
    */
   bool getPixelScale(const ossimString& gtiffPrefix,
                      const ossimKeywordlist& gtiffKwl,
                      ossimDpt& scale) const;
   
   /**
    * @brief Gets the model_tie_point array.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param ties The array to initialize.
    * @return true on success, false on error.
    */
   bool getTiePoint(const ossimString& gtiffPrefix,
                    const ossimKeywordlist& gtiffKwl,
                    std::vector<ossim_float64>& ties) const;

   /**
    * @brief Gets the model_transform array.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param ties The array to initialize.
    * @return true on success, false on error.
    */
   bool getModelTransform(const ossimString& gtiffPrefix,
                          const ossimKeywordlist& gtiffKwl,
                          std::vector<ossim_float64>& xfrm) const;

   /**
    * @brief Extracts float values from keyword list that match key.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param key to look for.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param floats Array to stuff.  Will have size of zero on failure.
    * @return true on success, false on error.  If size of array is zero it
    * will return false.
    */
   bool  getFloats(const ossimString& gtiffPrefix,
                   const ossimString& key,
                   const ossimKeywordlist& gtiffKwl,
                   std::vector<ossim_float64>& floats) const;

   /**
    * @brief Extracts float values from lookup line and puts in floats array.
    * @param lookup Line to parse like:
    * "0.00138888888889 0.00138888888889, 0.0"
    * @param floats Array to stuff.  Will have size of zero on failure.
    */
   bool getFloats(const ossimString& line,
                  std::vector<ossim_float64>& floats) const;

   /**
    * @brief Gets the pcs code from the keyword list as a string.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param pcsCode The code to initialize.
    * @return true on success, false on error.
    */
   bool getPcsCode(const ossimString& gtiffPrefix,
                   const ossimKeywordlist& gtiffKwl,
                   ossimString& pcsCode) const;

   /**
    * @brief Gets units from keyword list as a string. This will be angular
    * or linear base on the model type.  The default is meters if not found.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param linearUnits The string to initialize with units.
    * @return true on success, false on error.
    */
   bool getUnits(const ossimString& gtiffPrefix,
                 const ossimKeywordlist& gtiffKwl,
                 ossimString& units) const;
   /**
    * @brief Gets the linear units from keyword list as a string.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param linearUnits The string to initialize with units.
    * @return true on success, false on error.
    */
   bool getLinearUnits(const ossimString& gtiffPrefix,
                       const ossimKeywordlist& gtiffKwl,
                       ossimString& linearUnits) const;

   /**
    * @brief Gets the units from keyword list as a string.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param angularUnit The string to initialize with units.
    * @return true on success, false on error.
    */
   bool getAngularUnits(const ossimString& gtiffPrefix,
                        const ossimKeywordlist& gtiffKwl,
                        ossimString& units) const;


   /**
    * @brief Gets the pixel type (point or area) from keyword list as a string.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param unit The string to initialize with pixel type.
    * @return true on success, false on error.
    */   
   bool getPixelType(const ossimString& gtiffPrefix,
                     const ossimKeywordlist& gtiffKwl,
                     ossimString& pixelType) const;

   /**
    * @brief Gets the model type from keyword list as a string.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param modelType The string to initialize with pixel type.
    * @return true on success, false on error.
    */   
   bool getModelType(const ossimString& gtiffPrefix,
                     const ossimKeywordlist& gtiffKwl,
                     ossimString& modeType) const;

   /**
    * @brief Gets the ossimProjection from keyword list as a string.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @param ossimProj The string to initialize with pixel type.
    * @return true on success, false on error.
    */   
   bool getOssimProjectionName(const ossimString& gtiffPrefix,
                               const ossimKeywordlist& gtiffKwl,
                               ossimString& ossimProj) const;
   
   /**
    * @brief Gets the number of lines from keyword list.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @return Number of lines or 0 if lines not found.
    */   
   ossim_uint32 getLines(const ossimString& gtiffPrefix,
                         const ossimKeywordlist& gtiffKwl) const;

   /**
    * @brief Gets the number of samples from keyword list.
    * @param gtiffPrefix Prefix for gtiffKwl.
    * @param gtiffKwl The keyword list with all tiff keywords.
    * @return Number of lines or 0 if lines not found.
    */   
   ossim_uint32 getSamples(const ossimString& gtiffPrefix,
                           const ossimKeywordlist& gtiffKwl) const;

   bool getStdParallelOne(const ossimString& gtiffPrefix,
                          const ossimKeywordlist& gtiffKwl,
                          ossimString& value) const;

   bool getStdParallelTwo(const ossimString& gtiffPrefix,
                          const ossimKeywordlist& gtiffKwl,
                          ossimString& value) const;
   
   bool getFalseEasting(const ossimString& gtiffPrefix,
                        const ossimKeywordlist& gtiffKwl,
                        ossimString& value) const;

   bool getFalseNorthing(const ossimString& gtiffPrefix,
                         const ossimKeywordlist& gtiffKwl,
                         ossimString& value) const;

   bool getFalseEastingNorthing(const ossimString& gtiffPrefix,
                                const ossimKeywordlist& gtiffKwl,
                                ossimDpt& eastingNorthing) const;

   bool getScaleFactor(const ossimString& gtiffPrefix,
                       const ossimKeywordlist& gtiffKwl,
                       ossim_float64& value) const;

   bool getOriginLat(const ossimString& gtiffPrefix,
                     const ossimKeywordlist& gtiffKwl,
                     ossim_float64& value) const;

   bool getCentralMeridian(const ossimString& gtiffPrefix,
                           const ossimKeywordlist& gtiffKwl,
                           ossim_float64& value) const;

   void getTieSets(const std::vector<ossim_float64>& ties,
                   ossim_uint32 width,
                   ossim_uint32 height,
                   ossimTieGptSet& tieSet) const;

   bool hasOneBasedTiePoints(const std::vector<ossim_float64>& ties,
                             ossim_uint32 width,
                             ossim_uint32 height) const;
   
   ossimFilename  theFile;
   ossimEndian*   theEndian;
};

#endif /* End of "#ifndef ossimTiffInfo_HEADER" */
