///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file sectionHeader.h
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

#include "customDataTypes.h"

// Local Prototypes
void readSectionHeaders(union fileHeader_union *fileHeader, struct sectionNode **startSecLL, FILE *file);
void readSectionNames(struct sectionNode *startSecLL, FILE *file);
void sectionHeader32to64(union sectionHeader_union *secHeader);
void sectionHeaderEndianSwap(union sectionHeader_union *secHeader);
void printSectionInfo(const union fileHeader_union *fileHeader);
void printSectionHeaders(struct sectionNode *startSecLL);

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

