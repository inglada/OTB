#ifndef LIBLAS_CHIPPER_H
#define LIBLAS_CHIPPER_H

#include <liblas/liblas.hpp>
#include <vector>

namespace liblas
{

namespace chipper
{

enum Direction
{
    DIR_X,
    DIR_Y,
    DIR_NONE
};

class PtRef
{
public:
    double m_pos;
    boost::uint32_t m_ptindex;
    boost::uint32_t m_oindex;

    bool operator < (const PtRef& pt) const
        { return m_pos < pt.m_pos; }
};

struct RefList
{
public:
    std::vector<PtRef> m_vec;
    Direction m_dir;

    RefList(Direction dir = DIR_NONE) : m_dir(dir)
        {}
    std::vector<PtRef>::size_type size() const
        { return m_vec.size(); }
    void reserve(std::vector<PtRef>::size_type n)
        { m_vec.reserve(n); }
    void resize(std::vector<PtRef>::size_type n)
        { m_vec.resize(n); }
    void push_back(const PtRef& ref)
        { m_vec.push_back(ref); }
    std::vector<PtRef>::iterator begin()
        { return m_vec.begin(); }
    std::vector<PtRef>::iterator end()
        { return m_vec.end(); }
    PtRef& operator[](boost::uint32_t pos)
        { return m_vec[pos]; }
    std::string Dir()
    {
        if (m_dir == DIR_X)
            return "X";
        else if (m_dir == DIR_Y)
            return "Y";
        else
            return "NONE";
    }
};

class Chipper;

class Block
{
    friend class Chipper;

private:
    RefList *m_list_p;
    boost::uint32_t m_left;
    boost::uint32_t m_right;
    liblas::Bounds<double> m_bounds;
    // double m_xmin;
    // double m_ymin;
    // double m_xmax;
    // double m_ymax;

public:
    std::vector<boost::uint32_t> GetIDs() const; 
    liblas::Bounds<double> const& GetBounds() const {return m_bounds;} 
    void SetBounds(liblas::Bounds<double> const& bounds) {m_bounds = bounds;}
    // double GetXmin() const
    //     { return m_xmin; }
    // double GetYmin() const
    //     { return m_ymin; }
    // double GetXmax() const
    //     { return m_xmax; }
    // double GetYmax() const
    //     { return m_ymax; }
};

class Chipper
{
public:
    Chipper(Reader *reader, boost::uint32_t max_partition_size) :
        m_reader(reader), m_threshold(max_partition_size),
        m_xvec(DIR_X), m_yvec(DIR_Y), m_spare(DIR_NONE)
    {}

    void Chip();
    std::vector<Block>::size_type GetBlockCount()
        { return m_blocks.size(); }
    const Block& GetBlock(std::vector<Block>::size_type i)
        { return m_blocks[i]; }

private:
    void Load(RefList& xvec, RefList& yvec, RefList& spare);
    void Partition(boost::uint32_t size);
    void Split(RefList& xvec, RefList& yvec, RefList& spare);
    void DecideSplit(RefList& v1, RefList& v2, RefList& spare,
        boost::uint32_t left, boost::uint32_t right);
    void Split(RefList& wide, RefList& narrow, RefList& spare,
        boost::uint32_t left, boost::uint32_t right);
    void FinalSplit(RefList& wide, RefList& narrow,
        boost::uint32_t pleft, boost::uint32_t pcenter);
    void Emit(RefList& wide, boost::uint32_t widemin, boost::uint32_t widemax,
        RefList& narrow, boost::uint32_t narrowmin, boost::uint32_t narrowmax );

    Reader *m_reader;
    boost::uint32_t m_threshold;
    std::vector<Block> m_blocks;
    std::vector<boost::uint32_t> m_partitions;
    RefList m_xvec;
    RefList m_yvec;
    RefList m_spare;
};

} // namespace chipper

} // namespace liblas

#endif
