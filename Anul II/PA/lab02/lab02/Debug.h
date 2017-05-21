#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>

#define LOG_LEVEL 3

#ifdef DEBUG
#define LOG(level, content)                           \
    do {                                              \
      if (level <= LOG_LEVEL) {                       \
        std::cerr << __FUNCTION__ << "("              \
                  << __FILE__ << ":"                  \
                  << __LINE__ << ") "                 \
                  << content << std::endl;            \
      }                                               \
    } while(0)
#else
#define LOG(level, content)                           \
    do {} while(0)
#endif

#endif

