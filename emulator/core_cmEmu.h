/*
 * core_cmEmu.h
 *
 *  Created on: Dec 7, 2016
 *      Author: shapa
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

#define __DSB()
#define __NOP()

#define __CLZ             __builtin_clz

__attribute__((always_inline)) static inline uint32_t __RBIT(uint32_t value)
{
  uint32_t result;
  int32_t s = 4 /*sizeof(v)*/ * 8 - 1; /* extra shift needed at end */

  result = value;                      /* r will be reversed bits of v; first get LSB of v */
  for (value >>= 1U; value; value >>= 1U)
  {
    result <<= 1U;
    result |= value & 1U;
    s--;
  }
  result <<= s;                        /* shift when v's highest bits are zero */
  return(result);
}
