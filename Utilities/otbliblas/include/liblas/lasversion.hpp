/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Definition of LAS version constants
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#ifndef LIBLAS_LASVERSION_HPP_INCLUDED
#define LIBLAS_LASVERSION_HPP_INCLUDED

#include <liblas/capi/las_version.h>
#include <liblas/lasspatialreference.hpp>

namespace liblas {

/// Version numbers of the ASPRS LAS Specification.
/// Numerical representation of versions is calculated according to 
/// following formula: <em>major * 100000 + minor</em>
enum LASVersion
{
    eLASVersion10 = 1 * 100000 + 0, ///< LAS Format 1.0
    eLASVersion11 = 1 * 100000 + 1, ///< LAS Format 1.1
    eLASVersion12 = 1 * 100000 + 2, ///< LAS Format 1.2
    eLASVersion20 = 2 * 100000 + 0  ///< LAS Format 2.0
};

/// Range of allowed ASPRS LAS file format versions.
enum FormatVersion
{
    eVersionMajorMin = 1, ///< Minimum of major component
    eVersionMajorMax = 1, ///< Maximum of major component
    eVersionMinorMin = 0, ///< Minimum of minor component
    eVersionMinorMax = 3  ///< Maximum of minor component
};

/// Versions of point record format.
enum PointFormatName
{
    ePointFormat0 = 0,  ///< Point Data Format \e 0
    ePointFormat1 = 1,  ///< Point Data Format \e 1
    ePointFormat2 = 2,  ///< Point Data Format \e 2
    ePointFormat3 = 3,  ///< Point Data Format \e 3
    ePointFormat4 = 4,  ///< Point Data Format \e 3
    ePointFormat5 = 5,  ///< Point Data Format \e 3
    ePointFormatUnknown = -99 ///< Point Data Format is unknown
};

/// Number of bytes of point record storage in particular format.
enum PointSize
{
    ePointSize0 = 20, ///< Size of point record in data format \e 0
    ePointSize1 = 28, ///< Size of point record in data format \e 1
    ePointSize2 = 26, ///< Size of point record in data format \e 2
    ePointSize3 = 34  ///< Size of point record in data format \e 3
};

/// Check if GDAL support has been built in to libLAS.
inline bool IsGDALEnabled()
{
#ifdef HAVE_GDAL
    return true;
#else
    return false;
#endif
}

/// Check if GeoTIFF support has been built in to libLAS.
inline bool IsLibGeoTIFFEnabled()
{
#ifdef HAVE_LIBGEOTIFF
    return true;
#else
    return false;
#endif
}

/// Tell the user a bit about libLAS' compilation
inline std::string  GetFullVersion(void) {

    std::ostringstream os;
#ifdef HAVE_LIBGEOTIFF
    os << " GeoTIFF "
       << (LIBGEOTIFF_VERSION / 1000) << '.'
       << (LIBGEOTIFF_VERSION / 100 % 10) << '.'
       << (LIBGEOTIFF_VERSION % 100 / 10);
#endif
#ifdef HAVE_GDAL
    os << " GDAL " << GDALVersionInfo("RELEASE_NAME");
#endif

    std::string info(os.str());
    os.str("");
    os << "libLAS " << LIBLAS_RELEASE_NAME;
    if (!info.empty())
    {
        os << " with" << info;
    }


    return os.str();
}

/// Tell the user our dotted release name.
inline std::string GetVersion() {
    return std::string(LIBLAS_RELEASE_NAME);
}

} // namespace liblas

#endif // LIBLAS_LASVERSION_HPP_INCLUDED
