/*
crunchnet.h: CRNAPI master include file
Copyright (c) 2018 Kade Burnside
*/

#ifndef CRUNCHNET_H
#define CRUNCHNET_H

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

typedef ucnum8 cbool;
typedef void nullret;
typedef void* handle;

typedef ucnum64 NCRNFUNC;

/** Structure defenitions **/

// Date structure
typedef struct _crndate {
	ucnum8 day, month;
	ucnum32 year;
}crndate;

// Account structure
typedef struct _crnaccount {
	ucnum64 id;
	crnstr username, email;
	crndate dob, joindate;
}crnaccount, *pcrnacc;

/** Structure pointer types **/
typedef handle CTK;
typedef const crnstr* const FLOC;

/* Functions */

// Initialization functions
nullret CrunchInit(CTK token, FLOC fPath);
NCRNFUNC CrunchEnd(CTK token, FLOC fPath);

// App console functions
nullret AppLog(CTK token, crnstr msg, ucnum16 len);
nullret AppErr(CTK token, crnstr msg, ucnum16 len);

// Account functions
cbool GetAccountById(CTK token, ucnum64 id, pcrnacc crnacc);
cbool GetAccountByUsername(CTK token, crnstr username, pcrnacc crnacc);

#endif
