#ifndef CMEMORY_H
#define CMEMORY_H

#ifdef SKIP_NULL
#define set_null(L)
#else
#define set_null(L) L = NULL
#endif // SKIP_NULL

#endif // CMEMORY_H
