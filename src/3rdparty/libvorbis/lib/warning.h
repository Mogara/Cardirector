#ifndef _V_WARNING_H_
#define _V_WARNING_H_

#ifdef _MSC_VER
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4305)
#pragma warning(disable:4996)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#endif // _V_WARNING_H_
