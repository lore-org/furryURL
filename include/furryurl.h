#pragma once

#if LEONIS_SHARED_LIB
#if _WIN32
#if LEONIS_EXPORT

    #define LEONIS_API __declspec(dllexport)

#else /* LEONIS_EXPORT */

    #define LEONIS_API __declspec(dllimport)

#endif /* LEONIS_EXPORT */
#else /* _WIN32 */

    #define LEONIS_API __attribute__((visibility("default")))

#endif /* _WIN32 */
#else /* LEONIS_SHARED_LIB */

    #define LEONIS_API

#endif /* LEONIS_SHARED_LIB */

#include "furryurl/request.h"