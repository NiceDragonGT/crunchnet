/*
crunchnet.h: CRNAPI master include file
Copyright (c) 2018 Kade Burnside
*/

#ifndef CRUNCHNET_H
#define CRUNCHNET_H

#define NCRNFUNC ucnum64
#define EXP __declspec(dllexport)
#define CRNFUNC EXP ucnum64

/* Types */

// Basic types
typedef char cnum8;
typedef short cnum16;
typedef int cnum32;
typedef long long cnum64;
typedef unsigned char ucnum8;
typedef unsigned short ucnum16;
typedef unsigned int ucnum32;
typedef unsigned long long ucnum64;
typedef cnum8* crnstr;
typedef crnstr* crnstrarr;

typedef void nullret;
typedef void* handle;

// Structure pointer types
typedef handle CTK;
typedef crnstrarr FLOC;

/* Functions */

// Initialization functions
CTK CrunchInit(CTK, FLOC);
NCRNFUNC CrunchEnd(CTK, FLOC);

// App console functions
nullret AppLog(CTK, crnstr, ucnum16);
nullret AppErr(CTK, crnstr, ucnum16);

#endif
