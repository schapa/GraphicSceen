/*
 * memman.h
 *
 *  Created on: Apr 21, 2016
 *      Author: shapa
 */

#ifndef MEMMAN_H_
#define MEMMAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void *MEMMAN_malloc(size_t size);
void MEMMAN_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* MEMMAN_H_ */
