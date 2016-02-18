#ifndef _DSA_H_
#define _DSA_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DSA_MALLOC
#define DSA_MALLOC malloc
#endif

#ifndef DSA_FREE
#define DSA_FREE free
#endif

#ifndef DSA_CALLOC
#define DSA_CALLOC calloc
#endif

#include "dsa_list.h"
#include "dsa_hashmap.h"

#ifdef __cplusplus
}
#endif

#endif /* _DSA_H_ */
