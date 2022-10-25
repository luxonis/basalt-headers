/**
BSD 3-Clause License

This file is part of the Basalt project.
https://gitlab.com/VladyslavUsenko/basalt-headers.git

Copyright (c) 2019, Vladyslav Usenko and Nikolaus Demmel.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


@file
@brief Assertions used in the project
*/

#pragma once

#include <iostream>

namespace basalt {

#define UNUSED(x) (void)(x)

#ifdef _MSC_VER
#define BASALT_ATTRIBUTE_NORETURN __declspec(noreturn)
#define BASALT_ATTRIBUTE_NORETURN_POST_FUNCTION
#define BASALT_LIKELY(x) (x)

#else
#define BASALT_ATTRIBUTE_NORETURN __attribute__((noreturn))
#define BASALT_ATTRIBUTE_NORETURN_POST_FUNCTION
#define BASALT_LIKELY(x) __builtin_expect(x, 1)
#endif

inline BASALT_ATTRIBUTE_NORETURN void assertionFailed(char const* expr,
                                                      char const* function,
                                                      char const* file,
                                                      long line) {
  std::cerr << "***** Assertion (" << expr << ") failed in " << function
            << ":\n"
            << file << ':' << line << ":" << std::endl;
  std::abort();
}

inline BASALT_ATTRIBUTE_NORETURN void assertionFailedMsg(char const* expr,
                                                         char const* msg,
                                                         char const* function,
                                                         char const* file,
                                                         long line) {
  std::cerr << "***** Assertion (" << expr << ") failed in " << function
            << ":\n"
            << file << ':' << line << ": " << msg << std::endl;
  std::abort();
}

inline BASALT_ATTRIBUTE_NORETURN void logFatal(char const* function,
                                               char const* file, long line) {
  std::cerr << "***** Fatal error in " << function << ":\n"
            << file << ':' << line << ":" << std::endl;
  std::abort();
}

inline BASALT_ATTRIBUTE_NORETURN void logFatalMsg(char const* msg,
                                                  char const* function,
                                                  char const* file, long line) {
  std::cerr << "***** Fatal error in " << function << ":\n"
            << file << ':' << line << ": " << msg << std::endl;
  std::abort();
}

}  // namespace basalt

#if defined(BASALT_DISABLE_ASSERTS)

#define BASALT_ASSERT(expr) ((void)0)

#define BASALT_ASSERT_MSG(expr, msg) ((void)0)

#define BASALT_ASSERT_STREAM(expr, msg) ((void)0)

#else

#ifdef _MSC_VER

#define BASALT_ASSERT(expr)                                              \
  (BASALT_LIKELY(!!(expr))                                               \
       ? ((void)0)                                                       \
       : ::basalt::assertionFailed(#expr, __FUNCTION__, __FILE__, \
                                   __LINE__))

#define BASALT_ASSERT_MSG(expr, msg)                                      \
  (BASALT_LIKELY(!!(expr))                                                \
       ? ((void)0)                                                        \
       : ::basalt::assertionFailedMsg(#expr, msg, __FUNCTION__, __FILE__, \
                                      __LINE__))

#define BASALT_ASSERT_STREAM(expr, msg)  \
  (BASALT_LIKELY(!!(expr))               \
       ? ((void)0)                       \
       : (std::cerr << msg << std::endl, \
          ::basalt::assertionFailed(#expr, __FUNCTION__, __FILE__, __LINE__)))
#else
#define BASALT_ASSERT(expr)                                              \
  (BASALT_LIKELY(!!(expr))                                               \
       ? ((void)0)                                                       \
       : ::basalt::assertionFailed(#expr, __PRETTY_FUNCTION__, __FILE__, \
                                   __LINE__))

#define BASALT_ASSERT_MSG(expr, msg)                                   \
  (BASALT_LIKELY(!!(expr))                                             \
       ? ((void)0)                                                     \
       : ::basalt::assertionFailedMsg(#expr, msg, __PRETTY_FUNCTION__, \
                                      __FILE__, __LINE__))

#define BASALT_ASSERT_STREAM(expr, msg)                                   \
  (BASALT_LIKELY(!!(expr))                                                \
       ? ((void)0)                                                        \
       : (std::cerr << msg << std::endl,                                  \
          ::basalt::assertionFailed(#expr, __PRETTY_FUNCTION__, __FILE__, \
                                    __LINE__)))
#endif
#endif

#ifdef _MSC_VER
#define BASALT_LOG_FATAL(msg) \
  ::basalt::logFatalMsg(msg, __FUNCTION__, __FILE__, __LINE__)

#define BASALT_LOG_FATAL_STREAM(msg) \
  (std::cerr << msg << std::endl,    \
   ::basalt::logFatal(__FUNCTION__, __FILE__, __LINE__))

#else
#define BASALT_LOG_FATAL(msg) \
  ::basalt::logFatalMsg(msg, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define BASALT_LOG_FATAL_STREAM(msg) \
  (std::cerr << msg << std::endl,    \
   ::basalt::logFatal(__PRETTY_FUNCTION__, __FILE__, __LINE__))

#endif
