/*
crunchnet.h: CRNAPI master include file (CrunchNet 0.1) (C/C++)
Copyright (c) 2018 Kade Burnside
*/
#ifndef CRUNCHNET_H
#define CRUNCHNET_H
/* CrunchNet types */
// Basic types
typedef void NULLRET; // Function without return type
typedef char CBOOL; // Boolean
typedef char CNUM8; // 8-bit integer
typedef short CNUM16; // 16-bit integer
typedef int CNUM32; // 32-bit integer
typedef long long CNUM64; // 64-bit integer
// Pointer types
typedef NULLRET* CHAND; // Handle
typedef char* CRNSTR; // String
typedef CRNSTR* CRNSTRARR; // String array
// Simplification types
typedef CNUM64 NCRNFUNC; // CrunchMain function return type
typedef CHAND CTK; // CrunchNet back-end application token
typedef CHAND ATK; // CrunchNet account access token
typedef CNUM64 CRNID; // 64-bit CrunchNet ID
typedef struct _CRNIDL { CRNID id[2]; }CRNIDL; // 128-bit CrunchNet ID
// Structures
typedef struct _CRNDATE { CNUM8 day, month; CNUM32 year; }CRNDATE, *PCRNDATE; // Date
typedef struct _CRNFILE { const CRNIDL id, owner; const CNUM64 size; const CRNDATE creationDate, modificationDate; const CNUM8** const data; }CRNFILE, *PCRNFILE; // File
typedef struct _CRNWALLET { const CRNID id, owner; const CNUM64 balance; }CRNWALLET, *PCRNWALLET; // Wallet
typedef struct _CRNMINER { const CRNID id, owner; const CRNSTR name; const CNUM64 sessionLength, sessionEarnings, cryptXHour, cryptXDay; }CRNMINER, *PCRNMINER; // Miner
typedef struct _CRNACCOUNT { const CRNID id; const CRNSTR username, email; const CRNDATE birthday, joinDate; const CRNIDL* const files; const CRNID* const wallets; const CRNID* const miners; const ATK stdAccessToken; }CRNACCOUNT, *PCRNACCOUNT; // Account
/* CrunchNet macros */
#define CRNFUNC __declspec(dllexport) NCRNFUNC // CrunchMain function macro
// Error modes
#define NO_ERRORS 0 // Default value. CRNAPI functions do not log errors
#define STANDARD_LOG 1 // Functions use CRNAppLog to log errors
#define ERROR_LOG 2 // Functions use CRNAppErr to log errors
// Miner statistics
#define CRYPTX_HOUR 0 // CryptX/hour
#define CRYPTX_DAY 1 // CryptX/day
// Access types
#define STD_ACCESS 0 // Default value. Allows editing of username, email and password, sending miner data and execution of CrunchNet applications
#define MINER_ACCESS 1 // Allows access to miner data of account
#define FILE_ACCESS 2 // Allows access to all files owned by account
#define BASIC_WALLET_ACCESS 2 // Allows access to balance and transaction list of wallet
#define FULL_WALLET_ACCESS 3 // Allows application to send CryptX from wallet
/* CrunchNet functions */
// Standard functions
CHAND MemAlloc(CTK token, CNUM64 size); // Allocates memory (implementation of "malloc" function)
// Initialization functions
NULLRET CrunchInit(CTK token); // Initializes CrunchNet
NCRNFUNC CrunchEnd(CTK token); // Unloads CrunchNet
// Developer log API
NULLRET CRNAppLog(CTK token, CRNSTR message, CNUM16 length); // Prints message to developer's app console
NULLRET CRNAppErr(CTK token, CRNSTR message, CNUM16 length); // Notifies developer of application error
NULLRET CRNSetErrorMode(CTK token, CNUM8 mode); // Sets function error mode
// File API
CRNIDL MakeIDL(CRNID firstPart, CRNID secondPart); // Converts 2 64-bit IDs into 1 128-bit ID
CBOOL CRNGetFileAttr(CTK token, CRNIDL id, PCRNFILE file, ATK accessToken); // Gets attributes of specified file ID
CBOOL CRNGetFileData(CTK token, CRNIDL id, PCRNFILE file, CNUM32 section, ATK accessToken); // Gets data from specified section of file
CBOOL CRNGetFileData_All(CTK token, CRNIDL id, PCRNFILE file, ATK accessToken); // Gets all data from file
CBOOL CRNGetFile_Full(CTK token, CRNIDL id, PCRNFILE file, ATK accessToken); // Gets file attributes and all data from file
CBOOL CRNCreateFile(CTK token, PCRNFILE file, ATK accessToken); // Creates new file
CBOOL CRNAppendFile(CTK token, CNUM8* data, CNUM64 size, PCRNFILE file, ATK accessToken); // Appends data to end of file
CBOOL CRNInsertFile(CTK token, CNUM8* data, CNUM64 size, CNUM64 position, PCRNFILE file, ATK accessToken); // Inserts data at specified section of file
CBOOL CRNOverwriteFile(CTK token, CNUM8* data, CNUM64 size, CNUM64 position, PCRNFILE file, ATK accessToken); // Overwrites data at specified section of file
CBOOL CRNEraseFile(CTK token, PCRNFILE file, ATK accessToken); // Erases all data from file
CBOOL CRNDeleteFile(CTK token, PCRNFILE file, ATK accessToken); // Deletes file
// Wallet API
CBOOL CRNGetWallet(CTK token, PCRNWALLET wallet, PCRNACCOUNT account, ATK accessToken); // Gets wallet information
CBOOL CRNSendCryptX(CTK token, PCRNWALLET sender, CRNID recipient, ATK accessToken); // Sends CryptX to specified wallet
// Miner API
CBOOL CRNGetMiners(CTK token, PCRNMINER* miners, PCRNACCOUNT account, ATK accessToken); // Gets miner info
CBOOL CRNUpdateMiners(CTK token, PCRNMINER* miners, PCRNACCOUNT account, ATK accessToken); // Updates miner info
CRNSTR CRNGetTimeReadout(CTK token, PCRNMINER miner, ATK accessToken); // Converts 64-bit integer time into human-readable string
CRNSTR CRNGetCryptXReadout(CTK token, PCRNMINER miner, CNUM8 stat, ATK accessToken); // Converts 64-bit integer CryptX value into human-readable string
// Account API
CRNID CRNGetID(CRNSTR username); // Gets account ID from username
CBOOL CRNGetAccountFromID(CTK token, CRNID id, PCRNACCOUNT account); // Gets account using ID
CBOOL CRNGetAccountFromUsername(CTK token, CRNSTR username, PCRNACCOUNT account); // Gets account using username
CBOOL CRNGetBasicAccessToken(CTK token, CRNSTR password, PCRNACCOUNT account); // Gets basic access token for account
ATK CRNGetAccessToken(CTK token, PCRNACCOUNT account, CNUM16 accessType); // Gets more access rights for account
CBOOL CRNCreateAccount(CTK token, CRNSTR username, CRNSTR password, PCRNACCOUNT account); // Creates new account
CBOOL CRNChangeUsername(CTK token, CRNSTR username, PCRNACCOUNT account); // Changes account username
CBOOL CRNChangePassword(CTK token, CRNSTR password, PCRNACCOUNT account); // Changes account password
CBOOL CRNDeleteAccount(CTK token, PCRNACCOUNT account); // Deletes account
// Application interactivity API
CNUM8* CRNSendApplicationRequest(CTK token, CRNID appId, CNUM32 input); // Sends request to external CrunchNet application
#endif
