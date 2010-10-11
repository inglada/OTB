/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS point class 
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

#ifndef LIBLAS_LASPOINT_HPP_INCLUDED
#define LIBLAS_LASPOINT_HPP_INCLUDED

#include <liblas/cstdint.hpp>
#include <liblas/detail/fwd.hpp>
#include <liblas/detail/utility.hpp>
#include <liblas/lascolor.hpp>
// std
#include <stdexcept> // std::out_of_range
#include <cstdlib> // std::size_t

namespace liblas {

/// Point data record composed with X, Y, Z coordinates and attributes.
class LASPoint
{
public:

    enum DataMemberFlag
    {
        eReturnNumber = 1,
        eNumberOfReturns = 2,
        eScanDirection = 4,
        eFlightLineEdge = 8,
        eClassification = 16,
        eScanAngleRank = 32,
        eTime = 64
    };


    enum ClassificationType
    {
        eCreated = 0,
        eUnclassified,
        eGround,
        eLowVegetation,
        eMediumVegetation,
        eHighVegetation,
        eBuilding,
        eLowPoint,
        eModelKeyPoint,
        eWater = 9,
        // = 10 // reserved for ASPRS Definition
        // = 11 // reserved for ASPRS Definition
        eOverlapPoints = 12
        // = 13-31 // reserved for ASPRS Definition
    };

    enum ScanAngleRankRange
    {
        eScanAngleRankMin = -90,
        eScanAngleRankMax = 90
    };

    LASPoint();
    LASPoint(LASPoint const& other);
    LASPoint& operator=(LASPoint const& rhs);

    double GetX() const;
    double GetY() const;
    double GetZ() const;
    void SetCoordinates(double const& x, double const& y, double const& z);
    void SetCoordinates(LASHeader const& header, double x, double y, double z);
    
    void SetX(double const& value);
    void SetY(double const& value);
    void SetZ(double const& value);

    uint16_t GetIntensity() const;
    void SetIntensity(uint16_t const& intensity);

    /// Gets all scanning flags encoded as single byte.
    /// The flags are (mandatory):
    /// - Return Number (bits 0, 1, 2);
    /// - Number of Returns - given pulse (bits 3, 4, 5);
    /// - Scan Direction Flag (bit 6);
    /// - Edge of Flight Line (bit 7).
    uint8_t GetScanFlags() const;

    /// Sets all scanning flags passed as a single byte.
    /// \sa Documentation of GetScanFlags method for flags details.
    void SetScanFlags(uint8_t const& flags);
    
    uint16_t GetReturnNumber() const;
    void SetReturnNumber(uint16_t const& num);

    uint16_t GetNumberOfReturns() const;
    void SetNumberOfReturns(uint16_t const& num);

    uint16_t GetScanDirection() const;
    void SetScanDirection(uint16_t const& dir);
    
    uint16_t GetFlightLineEdge() const;
    void SetFlightLineEdge(uint16_t const& edge);

    uint8_t GetClassification() const;
    void SetClassification(uint8_t const& classify);

    int8_t GetScanAngleRank() const;
    void SetScanAngleRank(int8_t const& rank);

    /// Fetch value of File Marker (LAS 1.0) or User Data (LAS 1.1).
    uint8_t GetUserData() const;

    /// Set value of File Marker (LAS 1.0) or User Data (LAS 1.1).
    void SetUserData(uint8_t const& data);

    /// Fetch value of User Bit Field (LAS 1.0) or Point Source ID (LAS 1.1).
    uint16_t GetPointSourceID() const;

    /// Set value of User Bit Field (LAS 1.0) or Point Source ID (LAS 1.1).
    void SetPointSourceID(uint16_t const& id);

    /// Fetch color value associated with this point (LAS 1.2)
    LASColor const& GetColor() const;

    /// Set color value associated with this point (LAS 1.2)
    void SetColor(LASColor const& value);

                
    double GetTime() const;
    void SetTime(double const& time);

    /// Index operator providing access to XYZ coordinates of point record.
    /// Valid index values are 0, 1 or 2.
    /// \exception std::out_of_range if requested index is out of range (> 2).
    double& operator[](std::size_t const& n);

    /// Const version of index operator providing access to XYZ coordinates of point record.
    /// Valid index values are 0, 1 or 2.
    /// \exception std::out_of_range if requested index is out of range (> 2).
    double const& operator[](std::size_t const& n) const;

    /// \todo TODO: Should we compare other data members, but not only coordinates?
    bool equal(LASPoint const& other) const;

    bool Validate() const;
    bool IsValid() const;
    

private:

    static std::size_t const coords_size = 3;
    double m_coords[coords_size];
    uint16_t m_intensity;
    uint8_t m_flags;
    uint8_t m_class;
    int8_t m_angleRank;
    uint8_t m_userData;
    uint16_t m_pointSourceId;
    double m_gpsTime;
    
    LASColor m_color;
    detail::PointRecord m_rec;
    
    void throw_out_of_range() const
    {
        throw std::out_of_range("coordinate subscript out of range");
    }
};

/// Equal-to operator implemented in terms of LASPoint::equal method.
inline bool operator==(LASPoint const& lhs, LASPoint const& rhs)
{
    return lhs.equal(rhs);
}

/// Not-equal-to operator implemented in terms of LASPoint::equal method.
inline bool operator!=(LASPoint const& lhs, LASPoint const& rhs)
{
    return (!(lhs == rhs));
}

inline void LASPoint::SetCoordinates(double const& x, double const& y, double const& z)
{
    m_coords[0] = x;
    m_coords[1] = y;
    m_coords[2] = z;
}

inline double LASPoint::GetX() const
{
    return m_coords[0];
}

inline void LASPoint::SetX( double const& value ) 
{
    m_coords[0] = value;
}

inline double LASPoint::GetY() const
{
    return m_coords[1];
}

inline void LASPoint::SetY( double const& value ) 
{
    m_coords[1] = value;
}

inline double LASPoint::GetZ() const
{
    return m_coords[2];
}

inline void LASPoint::SetZ( double const& value ) 
{
    m_coords[2] = value;
}

inline uint16_t LASPoint::GetIntensity() const
{
    return m_intensity;
}

inline void LASPoint::SetIntensity(uint16_t const& intensity)
{
    m_intensity = intensity;
}

inline uint16_t LASPoint::GetReturnNumber() const
{
    // Read bits 1,2,3 (first 3 bits)
    return (m_flags & 0x07);
}

inline uint16_t LASPoint::GetNumberOfReturns() const
{
    // Read bits 4,5,6
    return ((m_flags >> 3) & 0x07);
}

inline uint16_t LASPoint::GetScanDirection() const
{
    // Read 7th bit
    return ((m_flags >> 6) & 0x01);
}

inline uint16_t LASPoint::GetFlightLineEdge() const
{
    // Read 8th bit
    return ((m_flags >> 7) & 0x01);
}

inline uint8_t LASPoint::GetScanFlags() const
{
    return m_flags;
}

inline void LASPoint::SetScanFlags(uint8_t const& flags)
{
    m_flags = flags;
}

inline uint8_t LASPoint::GetClassification() const
{
    return m_class;
}

inline void LASPoint::SetClassification(uint8_t const& classify)
{
    m_class = classify;
}

inline int8_t LASPoint::GetScanAngleRank() const
{
    return m_angleRank;
}

inline uint8_t LASPoint::GetUserData() const
{
    return m_userData;
}

inline uint16_t LASPoint::GetPointSourceID() const
{
    return m_pointSourceId;
}

inline void LASPoint::SetPointSourceID(uint16_t const& id)
{
    m_pointSourceId = id;
}

inline double LASPoint::GetTime() const
{
    return m_gpsTime;
}

inline void LASPoint::SetTime(double const& time)
{
    m_gpsTime = time;
}

inline LASColor const& LASPoint::GetColor() const
{
    return m_color;
}

inline void LASPoint::SetColor(LASColor const& value)
{
    m_color = value;
}


inline double& LASPoint::operator[](std::size_t const& n)
{
    if (coords_size <= n)
        throw_out_of_range();

    return m_coords[n];
}

inline double const& LASPoint::operator[](std::size_t const& n) const
{
    if (coords_size <= n)
        throw_out_of_range();

    return m_coords[n];
}

} // namespace liblas

#endif // LIBLAS_LASPOINT_HPP_INCLUDED
