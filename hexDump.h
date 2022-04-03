///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file hexDump.h
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
#include <string.h>

#include "customDataTypes.h"

// Local Prototypes
bool findNode(struct sectionNode *startSecLL, const char *secName, bool *hexDumpNodes);
void printHexDump(struct sectionNode *startSecLL, FILE *file, bool *hexDumpNodes);
void printHexBody(const int secSize, FILE *file);

