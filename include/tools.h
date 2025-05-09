#pragma once

#include <iostream>

#define VerifyOrReturnLogErrorMsg(expr, code, msg)                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(expr))                                                                                                   \
        {                                                                                                              \
            std::cerr << std::format("%s %s:%d", msg, __FILE__, __LINE__);                                             \
            return code;                                                                                               \
        }                                                                                                              \
    } while (false)

#define VerifyOrLogMsgError(expr, msg)                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(expr))                                                                                                   \
        {                                                                                                              \
            std::cerr << std::format("%s %s:%d", msg, __FILE__, __LINE__);                                             \
        }                                                                                                              \
    } while (false)
