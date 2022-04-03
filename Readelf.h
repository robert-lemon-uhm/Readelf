///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file lab5.h
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
#include <unistd.h>

#include "customDataTypes.h"

// Local Prototypes
void processFile(const char *fileName, bool *hexDumpArgs, int argc, char *argv[]);

// Other Prototypes
void printUsage(char *programName);

bool readFileHeader(union fileHeader_union *fileHeader, FILE *file);
void printFileHeader(const union fileHeader_union *fileHeader);

void readSectionHeaders(union fileHeader_union *fileHeader, struct sectionNode **startSecLL, FILE *file);
void printSectionInfo(const union fileHeader_union *fileHeader);
void printSectionHeaders(struct sectionNode *startSecLL);

bool findNode(struct sectionNode *startSecLL, const char *secName, bool *hexDumpNodes);
void printHexDump(struct sectionNode *startSecLL, FILE *file, bool *hexDumpNodes);

void freeSecLL(struct sectionNode **startSecLL);

