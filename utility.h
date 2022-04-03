///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file utility.h
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "customDataTypes.h"

// Global variables
extern const char *programName;

// Local Prototypes
void printUsage(char *programName);
bool checkSameEndian(const int programEndian);
uint16_t endianSwap16Bit(const uint16_t input);
uint32_t endianSwap32Bit(const uint32_t input);
uint64_t endianSwap64Bit(const uint64_t input);
void freeSecLL(struct sectionNode **startSecLL);

// Defines
#define BIT32 1
#define BIT64 2
#define ENDIAN_LITTLE 1
#define ENDIAN_BIG 2

