///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file fileHeader.h
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "customDataTypes.h"

// Local Prototypes
bool readFileHeader(union fileHeader_union *fileHeader, FILE *file);
void fileHeader32to64(union fileHeader_union *fileHeader);
void fileHeaderEndianSwap(union fileHeader_union *fileHeader);
void printFileHeader(const union fileHeader_union *fileHeader);

// Other Prototyptes
bool checkSameEndian(const int programEndian);
uint16_t endianSwap16Bit(const uint16_t input);
uint32_t endianSwap32Bit(const uint32_t input);
uint64_t endianSwap64Bit(const uint64_t input);

// Defines
#define BIT32 1
#define BIT64 2
#define ENDIAN_LITTLE 1
#define ENDIAN_BIG 2

