// This is core/vnl/vnl_math.h
#ifndef vnl_math_h_
#define vnl_math_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief Namespace with standard math functions
//
//  The vnl_math namespace provides a standard set of the simple mathematical
//  functions (min, max, sqr, sgn, rnd, abs), and some predefined constants
//  such as pi and e, which are not defined by the ANSI C++ standard.
//
//  There are complex versions defined in vnl_complex.h
//
//  That's right, M_PI is nonstandard!
//
//  Aside from e, pi and their associates the class also defines eps,
//  the IEEE double machine precision.  This is the smallest number
//  eps such that 1+eps != 1.
//
//  The operations are overloaded for int, float and double arguments,
//  which in combination with inlining can make them  more efficient than
//  their counterparts in the standard C library.
//
// \author Andrew W. Fitzgibbon, Oxford RRG
// \date   July 13, 1996
//
// \verbatim
//  Modifications
//   21 May 1998 AWF Removed conditional VCL_IMPLEMENT_STATIC_CONSTS, sometimes gcc needs them.
//   LSB (Modifications) 23 Jan 2001 Documentation tidied
//   Peter Vanroose - 7 Sep 2002 - maxdouble etc. replaced by vnl_numeric_traits<T>::maxval
//   Amitha Perera - 13 Sep 2002 - make constant initialization standards compliant.
// \endverbatim

#include <vcl_cmath.h>
#include "dll.h"
#include <vxl_config.h>
#include <vnl/vnl_config.h> // for VNL_CONFIG_ENABLE_SSE2_ROUNDING
#ifdef VNL_CHECK_FPU_ROUNDING_MODE
# include <vcl_cassert.h>
#endif

// Figure out when the fast implementation can be used
#if VNL_CONFIG_ENABLE_SSE2_ROUNDING && (!defined(__GCCXML__)) && defined(__SSE2__)
# if !VXL_HAS_EMMINTRIN_H
#   error "Required file emmintrin.h for SSE2 not found"
# else
#   include <emmintrin.h> // sse 2 intrinsics
#   define USE_SSE2_IMPL 1
# endif
#else
# define USE_SSE2_IMPL 0
#endif
// Turn on fast impl when using GCC on x86 platform with the following exception:
//   GCCXML
#if defined(__GNUC__) && (!defined(__GCCXML__)) &&  (defined(__i386__) || defined(__i386) || defined(__x86_64__) || defined(__x86_64))
# define GCC_USE_FAST_IMPL 1
#else
# define GCC_USE_FAST_IMPL 0
#endif
// Turn on fast impl when using msvc on 32 bits windows
#if defined(VCL_VC) && (!defined(__GCCXML__)) && !defined(_WIN64)
# define VC_USE_FAST_IMPL 1
#else
# define VC_USE_FAST_IMPL 0
#endif


//: Type-accessible infinities for use in templates.
template <class T> T vnl_huge_val(T);
double   vnl_huge_val(double);
float    vnl_huge_val(float);
long int vnl_huge_val(long int);
int      vnl_huge_val(int);
short    vnl_huge_val(short);
char     vnl_huge_val(char);

//: real numerical constants
class vnl_math
{
 public:
  //: pi, e and all that
  static VNL_DLL_DATA const double e                VCL_STATIC_CONST_INIT_FLOAT_DECL(2.7182818284590452354);
  static VNL_DLL_DATA const double log2e            VCL_STATIC_CONST_INIT_FLOAT_DECL(1.4426950408889634074);
  static VNL_DLL_DATA const double log10e           VCL_STATIC_CONST_INIT_FLOAT_DECL(0.43429448190325182765);
  static VNL_DLL_DATA const double ln2              VCL_STATIC_CONST_INIT_FLOAT_DECL(0.69314718055994530942);
  static VNL_DLL_DATA const double ln10             VCL_STATIC_CONST_INIT_FLOAT_DECL(2.30258509299404568402);
  static VNL_DLL_DATA const double pi               VCL_STATIC_CONST_INIT_FLOAT_DECL(3.14159265358979323846);
  static VNL_DLL_DATA const double pi_over_2        VCL_STATIC_CONST_INIT_FLOAT_DECL(1.57079632679489661923);
  static VNL_DLL_DATA const double pi_over_4        VCL_STATIC_CONST_INIT_FLOAT_DECL(0.78539816339744830962);
  static VNL_DLL_DATA const double one_over_pi      VCL_STATIC_CONST_INIT_FLOAT_DECL(0.31830988618379067154);
  static VNL_DLL_DATA const double two_over_pi      VCL_STATIC_CONST_INIT_FLOAT_DECL(0.63661977236758134308);
  static VNL_DLL_DATA const double two_over_sqrtpi  VCL_STATIC_CONST_INIT_FLOAT_DECL(1.12837916709551257390);
  static VNL_DLL_DATA const double one_over_sqrt2pi VCL_STATIC_CONST_INIT_FLOAT_DECL(0.39894228040143267794);
  static VNL_DLL_DATA const double sqrt2            VCL_STATIC_CONST_INIT_FLOAT_DECL(1.41421356237309504880);
  static VNL_DLL_DATA const double sqrt1_2          VCL_STATIC_CONST_INIT_FLOAT_DECL(0.70710678118654752440);

  //: IEEE double machine precision
  static VNL_DLL_DATA const double eps             VCL_STATIC_CONST_INIT_FLOAT_DECL(2.2204460492503131e-16);
  static VNL_DLL_DATA const double sqrteps         VCL_STATIC_CONST_INIT_FLOAT_DECL(1.490116119384766e-08);
  //: IEEE single machine precision
  static VNL_DLL_DATA const float float_eps        VCL_STATIC_CONST_INIT_FLOAT_DECL(1.192092896e-07f);
  static VNL_DLL_DATA const float float_sqrteps    VCL_STATIC_CONST_INIT_FLOAT_DECL(3.4526698307e-4f);
};

// We do not want to make assumptions about unknown types that happen
// to have conversions to one of the fundamental types.  The templated
// versions of isnan, isinf, and isfinite below serve as catch-alls to
// cause linker errors if these functions are invoked with an unknown
// type.  However, due to compiler bugs, the templates sometimes match
// too often (see documentation of VCL_TEMPLATE_MATCHES_TOO_OFTEN) and
// are selected over reference-binding overloads like those in
// vnl_rational.h.  We add the catch-all templates only if the
// compiler does not have this bug. -- Brad King

// Note that the three template functions below should not be declared "inline"
// since that would override the non-inline specialisations. - PVr.
//

// isnan
inline bool vnl_math_isnan(char)          { return false; }
inline bool vnl_math_isnan(short)         { return false; }
inline bool vnl_math_isnan(int)           { return false; }
inline bool vnl_math_isnan(long)          { return false; }
inline bool vnl_math_isnan(signed char)   { return false; }
inline bool vnl_math_isnan(unsigned char) { return false; }
inline bool vnl_math_isnan(unsigned short){ return false; }
inline bool vnl_math_isnan(unsigned int)  { return false; }
inline bool vnl_math_isnan(unsigned long) { return false; }
bool vnl_math_isnan(float);
bool vnl_math_isnan(double);
bool vnl_math_isnan(long double);
#if !VCL_TEMPLATE_MATCHES_TOO_OFTEN
template <class T> bool vnl_math_isnan(T);
#endif

// isinf
inline bool vnl_math_isinf(char)          { return false; }
inline bool vnl_math_isinf(short)         { return false; }
inline bool vnl_math_isinf(int)           { return false; }
inline bool vnl_math_isinf(long)          { return false; }
inline bool vnl_math_isinf(signed char)   { return false; }
inline bool vnl_math_isinf(unsigned char) { return false; }
inline bool vnl_math_isinf(unsigned short){ return false; }
inline bool vnl_math_isinf(unsigned int)  { return false; }
inline bool vnl_math_isinf(unsigned long) { return false; }
bool vnl_math_isinf(float);
bool vnl_math_isinf(double);
bool vnl_math_isinf(long double);
#if !VCL_TEMPLATE_MATCHES_TOO_OFTEN
template <class T> bool vnl_math_isinf(T);
#endif

// isfinite
inline bool vnl_math_isfinite(char)          { return true; }
inline bool vnl_math_isfinite(short)         { return true; }
inline bool vnl_math_isfinite(int)           { return true; }
inline bool vnl_math_isfinite(long)          { return true; }
inline bool vnl_math_isfinite(signed char)   { return true; }
inline bool vnl_math_isfinite(unsigned char) { return true; }
inline bool vnl_math_isfinite(unsigned short){ return true; }
inline bool vnl_math_isfinite(unsigned int)  { return true; }
inline bool vnl_math_isfinite(unsigned long) { return true; }
bool vnl_math_isfinite(float);
bool vnl_math_isfinite(double);
bool vnl_math_isfinite(long double);
#if !VCL_TEMPLATE_MATCHES_TOO_OFTEN
template <class T> bool vnl_math_isfinite(T);
#endif



// vnl_math_rnd_halfinttoeven  -- round towards nearest integer
//         halfway cases are rounded towards the nearest even integer, e.g.
//         vnl_math_rnd_halfinttoeven( 1.5) ==  2
//         vnl_math_rnd_halfinttoeven(-1.5) == -2
//         vnl_math_rnd_halfinttoeven( 2.5) ==  2
//         vnl_math_rnd_halfinttoeven( 3.5) ==  4
//
// We assume that the rounding mode is not changed from the default
// one (or at least that it is always restored to the default one).

#if USE_SSE2_IMPL // Fast sse2 implementation

inline int vnl_math_rnd_halfinttoeven(float  x)
{
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
  assert(fegetround()==FE_TONEAREST);
# endif
  return _mm_cvtss_si32(_mm_set_ss(x));
}
inline int vnl_math_rnd_halfinttoeven(double  x)
{
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
  assert(fegetround()==FE_TONEAREST);
# endif
  return _mm_cvtsd_si32(_mm_set_sd(x));
}

#elif GCC_USE_FAST_IMPL // Fast gcc asm implementation

inline int vnl_math_rnd_halfinttoeven(float  x)
{
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
  assert(fegetround()==FE_TONEAREST);
# endif
  int r;
  __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
  return r;
}
inline int vnl_math_rnd_halfinttoeven(double  x)
{
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
  assert(fegetround()==FE_TONEAREST);
# endif
  int r;
  __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
  return r;
}

#elif VC_USE_FAST_IMPL // Fast msvc asm implementation

inline int vnl_math_rnd_halfinttoeven(float  x)
{
  int r;
  __asm {
    fld x
    fistp r
  }
  return r;
}
inline int vnl_math_rnd_halfinttoeven(double  x)
{
  int r;
  __asm {
    fld x
    fistp r
  }
  return r;
}

#else // Vanilla implementation

inline int vnl_math_rnd_halfinttoeven(float  x)
{
  if (x>=0.f)
  {
     x+=0.5f;
     const int r = static_cast<int>(x);
     if ( x != static_cast<float>(r) ) return r;
     return 2*(r/2);
  }
  else
  {
     x-=0.5f;
     const int r = static_cast<int>(x);
     if ( x != static_cast<float>(r) ) return r;
     return 2*(r/2);
  }
}
inline int vnl_math_rnd_halfinttoeven(double x)
{
  if (x>=0.)
  {
     x+=0.5;
     const int r = static_cast<int>(x);
     if ( x != static_cast<double>(r) ) return r;
     return 2*(r/2);
  }
  else
  {
     x-=0.5;
     const int r = static_cast<int>(x);
     if ( x != static_cast<double>(r) ) return r;
     return 2*(r/2);
  }
}

#endif



// vnl_math_rnd_halfintup  -- round towards nearest integer
//         halfway cases are rounded upward, e.g.
//         vnl_math_rnd_halfintup( 1.5) ==  2
//         vnl_math_rnd_halfintup(-1.5) == -1
//         vnl_math_rnd_halfintup( 2.5) ==  3
//
// Be careful: argument absolute value must be less than INT_MAX/2
// for vnl_math_rnd_halfintup to be guaranteed to work.
// We also assume that the rounding mode is not changed from the default
// one (or at least that it is always restored to the default one).

#if USE_SSE2_IMPL || GCC_USE_FAST_IMPL || VC_USE_FAST_IMPL

inline int vnl_math_rnd_halfintup(float  x) { return vnl_math_rnd_halfinttoeven(2*x+0.5f)>>1; }
inline int vnl_math_rnd_halfintup(double  x) { return vnl_math_rnd_halfinttoeven(2*x+0.5)>>1; }

#else // Vanilla implementation

inline int vnl_math_rnd_halfintup(float  x)
{
  x+=0.5f;
  return static_cast<int>(x>=0.f?x:(x==static_cast<int>(x)?x:x-1.f));
}
inline int vnl_math_rnd_halfintup(double x)
{
  x+=0.5;
  return static_cast<int>(x>=0.?x:(x==static_cast<int>(x)?x:x-1.));
}

#endif



// vnl_math_rnd  -- round towards nearest integer
//         halfway cases such as 0.5 may be rounded either up or down
//         so as to maximize the efficiency, e.g.
//         vnl_math_rnd_halfinttoeven( 1.5) ==  1 or  2
//         vnl_math_rnd_halfinttoeven(-1.5) == -2 or -1
//         vnl_math_rnd_halfinttoeven( 2.5) ==  2 or  3
//         vnl_math_rnd_halfinttoeven( 3.5) ==  3 or  4
//
// We assume that the rounding mode is not changed from the default
// one (or at least that it is always restored to the default one).

#if  USE_SSE2_IMPL || GCC_USE_FAST_IMPL || VC_USE_FAST_IMPL

inline int vnl_math_rnd(float  x) { return vnl_math_rnd_halfinttoeven(x); }
inline int vnl_math_rnd(double  x) { return vnl_math_rnd_halfinttoeven(x); }

#else // Vanilla implementation

inline int vnl_math_rnd(float  x) { return x>=0.f?static_cast<int>(x+.5f):static_cast<int>(x-.5f); }
inline int vnl_math_rnd(double x) { return x>=0.0?static_cast<int>(x+0.5):static_cast<int>(x-0.5); }


#endif



// vnl_math_floor -- round towards minus infinity
//
// Be careful: argument absolute value must be less than INT_MAX/2
// for vnl_math_floor to be guaranteed to work.
// We also assume that the rounding mode is not changed from the default
// one (or at least that it is always restored to the default one).

#if  USE_SSE2_IMPL // Fast sse2 implementation

inline int vnl_math_floor(float  x)
{
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
  assert(fegetround()==FE_TONEAREST);
# endif
   return _mm_cvtss_si32(_mm_set_ss(2*x-.5f))>>1;
}
inline int vnl_math_floor(double  x)
{
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
  assert(fegetround()==FE_TONEAREST);
# endif
   return _mm_cvtsd_si32(_mm_set_sd(2*x-.5))>>1;
}

#elif GCC_USE_FAST_IMPL // Fast gcc asm implementation

inline int vnl_math_floor(float  x)
{
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
  assert(fegetround()==FE_TONEAREST);
# endif
  int r;
  x = 2*x-.5f;
  __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
  return r>>1;
}
inline int vnl_math_floor(double  x)
{
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
  assert(fegetround()==FE_TONEAREST);
# endif
  int r;
  x = 2*x-.5;
  __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
  return r>>1;
}

#elif VC_USE_FAST_IMPL // Fast msvc asm implementation

inline int vnl_math_floor(float  x)
{
  int r;
  x = 2*x-.5f;
  __asm {
    fld x
    fistp r
  }
  return r>>1;
}
inline int vnl_math_floor(double  x)
{
  int r;
  x = 2*x-.5;
  __asm {
    fld x
    fistp r
  }
  return r>>1;
}

#else // Vanilla implementation

inline int vnl_math_floor(float  x)
{
  return static_cast<int>(x>=0.f?x:(x==static_cast<int>(x)?x:x-1.f));
}
inline int vnl_math_floor(double x)
{
  return static_cast<int>(x>=0.0?x:(x==static_cast<int>(x)?x:x-1.0));
}

#endif



// vnl_math_ceil -- round towards plus infinity
//
// Be careful: argument absolute value must be less than INT_MAX/2
// for vnl_math_ceil to be guaranteed to work.
// We also assume that the rounding mode is not changed from the default
// one (or at least that it is always restored to the default one).

#if  USE_SSE2_IMPL // Fast sse2 implementation

inline int vnl_math_ceil(float  x)
{
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
  assert(fegetround()==FE_TONEAREST);
# endif
   return -(_mm_cvtss_si32(_mm_set_ss(-.5f-2*x))>>1);
}
inline int vnl_math_ceil(double  x)
{
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
  assert(fegetround()==FE_TONEAREST);
# endif
   return -(_mm_cvtsd_si32(_mm_set_sd(-.5-2*x))>>1);
}

#elif GCC_USE_FAST_IMPL // Fast gcc asm implementation

inline int vnl_math_ceil(float  x)
{
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
  assert(fegetround()==FE_TONEAREST);
# endif
  int r;
  x = -.5f-2*x;
  __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
  return -(r>>1);
}
inline int vnl_math_ceil(double  x)
{
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
  assert(fegetround()==FE_TONEAREST);
# endif
  int r;
  x = -.5-2*x;
  __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
  return -(r>>1);
}

#elif VC_USE_FAST_IMPL // Fast msvc asm implementation

inline int vnl_math_ceil(float  x)
{
  int r;
  x = -.5f-2*x;
  __asm {
    fld x
    fistp r
  }
  return -(r>>1);
}
inline int vnl_math_ceil(double  x)
{
  int r;
  x = -.5-2*x;
  __asm {
    fld x
    fistp r
  }
  return -(r>>1);
}

#else // Vanilla implementation

inline int vnl_math_ceil(float  x)
{
  return static_cast<int>(x<0.f?x:(x==static_cast<int>(x)?x:x+1.f));
}
inline int vnl_math_ceil(double x)
{
  return static_cast<int>(x<0.0?x:(x==static_cast<int>(x)?x:x+1.0));
}

#endif



// abs
inline bool           vnl_math_abs(bool x)          { return x; }
inline unsigned char  vnl_math_abs(unsigned char x) { return x; }
inline unsigned char  vnl_math_abs(signed char x)   { return x < 0 ? static_cast<unsigned char>(-x) : x; }
inline unsigned char  vnl_math_abs(char x)          { return static_cast<unsigned char>(x); }
inline unsigned short vnl_math_abs(short x)         { return x < 0 ? static_cast<unsigned short>(-x) : x; }
inline unsigned short vnl_math_abs(unsigned short x){ return x; }
inline unsigned int   vnl_math_abs(int x)           { return x < 0 ? -x : x; }
inline unsigned int   vnl_math_abs(unsigned int x)  { return x; }
inline unsigned long  vnl_math_abs(long x)          { return x < 0L ? -x : x; }
inline unsigned long  vnl_math_abs(unsigned long x) { return x; }
inline float          vnl_math_abs(float x)         { return x < 0.0f ? -x : x; }
inline double         vnl_math_abs(double x)        { return x < 0.0 ? -x : x; }
inline long double    vnl_math_abs(long double x)   { return x < 0.0 ? -x : x; }

// max
inline int           vnl_math_max(int x, int y)                     { return (x > y) ? x : y; }
inline unsigned int  vnl_math_max(unsigned int x, unsigned int y)   { return (x > y) ? x : y; }
inline long          vnl_math_max(long x, long y)                   { return (x > y) ? x : y; }
inline unsigned long vnl_math_max(unsigned long x, unsigned long y) { return (x > y) ? x : y;}
inline float         vnl_math_max(float x, float y)                 { return (x < y) ? y : x; }
inline double        vnl_math_max(double x, double y)               { return (x < y) ? y : x; }

// min
inline int           vnl_math_min(int x, int y)                     { return (x < y) ? x : y; }
inline unsigned int  vnl_math_min(unsigned int x, unsigned int y)   { return (x < y) ? x : y; }
inline long          vnl_math_min(long x, long y)                   { return (x < y) ? x : y; }
inline unsigned long vnl_math_min(unsigned long x, unsigned long y) { return (x < y) ? x : y;}
inline float         vnl_math_min(float x, float y)                 { return (x > y) ? y : x; }
inline double        vnl_math_min(double x, double y)               { return (x > y) ? y : x; }

// sqr (square)
inline bool          vnl_math_sqr(bool x)          { return x; }
inline int           vnl_math_sqr(int x)           { return x*x; }
inline unsigned int  vnl_math_sqr(unsigned int x)  { return x*x; }
inline long          vnl_math_sqr(long x)          { return x*x; }
inline unsigned long vnl_math_sqr(unsigned long x) { return x*x; }
inline float         vnl_math_sqr(float x)         { return x*x; }
inline double        vnl_math_sqr(double x)        { return x*x; }

// cube
inline bool          vnl_math_cube(bool x)          { return x; }
inline int           vnl_math_cube(int x)           { return x*x*x; }
inline unsigned int  vnl_math_cube(unsigned int x)  { return x*x*x; }
inline long          vnl_math_cube(long x)          { return x*x*x; }
inline unsigned long vnl_math_cube(unsigned long x) { return x*x*x; }
inline float         vnl_math_cube(float x)         { return x*x*x; }
inline double        vnl_math_cube(double x)        { return x*x*x; }

// sgn (sign in -1, 0, +1)
inline int vnl_math_sgn(int x)    { return x?((x>0)?1:-1):0; }
inline int vnl_math_sgn(long x)   { return x?((x>0)?1:-1):0; }
inline int vnl_math_sgn(float x)  { return (x != 0)?((x>0)?1:-1):0; }
inline int vnl_math_sgn(double x) { return (x != 0)?((x>0)?1:-1):0; }

// sgn0 (sign in -1, +1 only, useful for reals)
inline int vnl_math_sgn0(int x)    { return (x>=0)?1:-1; }
inline int vnl_math_sgn0(long x)   { return (x>=0)?1:-1; }
inline int vnl_math_sgn0(float x)  { return (x>=0)?1:-1; }
inline int vnl_math_sgn0(double x) { return (x>=0)?1:-1; }

// squared_magnitude
inline unsigned int  vnl_math_squared_magnitude(char          x) { return int(x)*int(x); }
inline unsigned int  vnl_math_squared_magnitude(unsigned char x) { return int(x)*int(x); }
inline unsigned int  vnl_math_squared_magnitude(int           x) { return x*x; }
inline unsigned int  vnl_math_squared_magnitude(unsigned int  x) { return x*x; }
inline unsigned long vnl_math_squared_magnitude(long          x) { return x*x; }
inline unsigned long vnl_math_squared_magnitude(unsigned long x) { return x*x; }
inline float         vnl_math_squared_magnitude(float         x) { return x*x; }
inline double        vnl_math_squared_magnitude(double        x) { return x*x; }
inline long double   vnl_math_squared_magnitude(long double   x) { return x*x; }

// cuberoot
inline float  vnl_math_cuberoot(float  a) { return float((a<0) ? -vcl_exp(vcl_log(-a)/3) : vcl_exp(vcl_log(a)/3)); }
inline double vnl_math_cuberoot(double a) { return       (a<0) ? -vcl_exp(vcl_log(-a)/3) : vcl_exp(vcl_log(a)/3); }

// hypotenuse
inline double      vnl_math_hypot(int         x, int         y) { return vcl_sqrt(double(x*x + y*y)); }
inline float       vnl_math_hypot(float       x, float       y) { return float( vcl_sqrt(double(x*x + y*y)) ); }
inline double      vnl_math_hypot(double      x, double      y) { return vcl_sqrt(x*x + y*y); }
inline long double vnl_math_hypot(long double x, long double y) { return vcl_sqrt(x*x + y*y); }

#endif // vnl_math_h_
