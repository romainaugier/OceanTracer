#pragma once

#ifdef _MSC_VER 

#define FORCEINLINE static __forceinline

#else __GNUC__

#define FORCEINLINE static __attribute__((always_inline))

#endif