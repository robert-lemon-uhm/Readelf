///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file lab5.c
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Header file
#include "Readelf.h"

// Global Variable
char *programName;

bool fileHeaderOpt = false;
bool sectionHeaderOpt = false;
bool hexDumpOpt = false;

int hexDumpSection;

/**
 * The main driver for the lab05 ReadELF program. It takes the '-h'
 * option to specify header reading for the second argument - a file name.
 * The driver checks for valid arguments, then reads and process the 
 * file's ELF header.
 */
int main(int argc, char *argv[]) {

   int i;
   char ch;
   bool *hexDumpArgs;

   // Set global program name
   programName = argv[0] + 2;

   // Allocate memory for arg bool array
   hexDumpArgs = malloc(argc);

   // Intialize hexDumpArgs bools
   for (i = 0; i < argc; i++) {
      hexDumpArgs[i] = false;
   }

   // Process arg opts
   while ((ch = getopt(argc, argv, "hxt")) != -1) {
      switch(ch) {
         case 'h':
            fileHeaderOpt = true;
            break;
         case 't':
            sectionHeaderOpt = true;
            break;
         case 'x':
            hexDumpOpt = true;
            hexDumpArgs[optind++] = true;
            break;
         default:
            printUsage(programName);
      }
   }

   // If no opt was entered, exit
   if (optind == 1) {
      printUsage(programName);
   }

   // If no file was entered, exit
   else if (optind >= argc) {
      printUsage(programName);
   }

   // If only one file, process it
   else if (optind == argc-1) {
      processFile(argv[optind], hexDumpArgs, argc, argv);
   }

   // If there are multiple files, process each one
   else {
      for (i=optind; i < argc; i++) {
         printf("\nFile: %s\n", argv[i]);
         processFile(argv[i], hexDumpArgs, argc, argv);
      }
   }

   // Free hexDumpArgs memory
   free(hexDumpArgs);

   return EXIT_SUCCESS;
}

/**
 * A function that processes a file given by the main driver. It utilizes the
 * global variables to determine which options the user entered. 
 *
 * INPUT:   name of file to process (string)
 *          boolean array for hex dump args one-hot-encoding
 *          number of args
 *          args (pointer to string arr)
 *    
 * OUTPUT:  none
 */
void processFile(const char *fileName, bool *hexDumpArgs, int argc, char *argv[]) {

   int i;
   bool *hexDumpNodes;
   FILE * file;
   union fileHeader_union fileHeader;
   struct sectionNode *startSecLL = NULL;

   // Attempt to open the file, otherwise exit
   if ( (file = fopen(fileName, "r")) == NULL ) {
      fprintf(stderr, "%s: %s: Error: No such file\n", programName, fileName);
      exit(EXIT_FAILURE);
   }

   // Grab data from file header
   if (!readFileHeader(&fileHeader, file)) { // not an ELF if false
      fprintf(stderr, "%s: %s: Error: Not an ELF file - ", programName, fileName);
      fprintf(stderr, "it has the wrong magic bytes at the start\n");
      exit(EXIT_FAILURE);
   }

   // Grab Data from section headers
   if (sectionHeaderOpt || hexDumpOpt) {
      readSectionHeaders(&fileHeader, &startSecLL, file);
   }

   // === [-h] OPT ===================================================================
   if (fileHeaderOpt) {
      // Print file headers
      printFileHeader(&fileHeader);
   }

   // === [-t] OPT ===================================================================
   if (sectionHeaderOpt) {
      // Print header info if ELF header not printed
      if (!fileHeaderOpt) {
         printSectionInfo(&fileHeader);
      }
      // Print section headers
      printSectionHeaders(startSecLL);
   }

   // === [-x] OPT ===================================================================
   if (hexDumpOpt) {

      // Allocate memory for hexDumpNodes one-hot-encoding
      hexDumpNodes = malloc(fileHeader.Struct.e_shnum);

      // Initialize hexDumpNodes to false
      for (i=0; i < fileHeader.Struct.e_shnum; i++) {
         hexDumpNodes[i] = false;
      }

      // Iterate over each arg
      for (i=0; i < argc; i++) {

         // Check for args associated with [-x] opt
         if (hexDumpArgs[i]) {

            // Update one-hot-encoding, or print error if invalid section
            if (!findNode(startSecLL, argv[i], hexDumpNodes)) {
               fprintf(stderr, "%s: %s: Warning: Section '%s' was not dumped because it does not exist!\n",
                     programName, fileName, argv[i]);
            }
         }
      }

      // Print hex dump for all args associated with [-x] opt
      printHexDump(startSecLL, file, hexDumpNodes); 

      // Unallocate memory for hexDumpNodes one-hot-encoding
      free(hexDumpNodes);
   }
   // ================================================================================

   // Free LL memory
   if (sectionHeaderOpt || hexDumpOpt) {
      freeSecLL(&startSecLL);
   }

   // Attempt to close file, otherwise exit
   if (fclose(file) == EOF) {
      fprintf(stderr, "%s: Can't close [%s]\n", programName, fileName);
      exit(EXIT_FAILURE);
   }
}

