///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file utility.c
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Header file
#include "utility.h"

/**
 * A utility function that prints the usage of this program.
 * This function is called when given poor user input.
 *
 * INPUT:   string, the program name
 *
 * OUTPUT:  none
 */
void printUsage(char *programName) {
   fprintf(stderr, "%s: Warning: Nothing to do.\n", programName);
   fprintf(stderr, "Usage: %s <option(s)> elf-file(s)\n", programName);
   fprintf(stderr, " Display information about the contents of ELF format files\n");
   fprintf(stderr, " Options are:\n");
   fprintf(stderr, "  -h\t\t\tDisplay the ELF file header\n");
   fprintf(stderr, "  -t\t\t\tDisplay the section details\n");
   fprintf(stderr, "  -x <name>");
   fprintf(stderr, "\t\tDump the contents of the section <name> as bytes\n");
}

/**
 * A utility function that checks the endianness of the 
 * machine running this program, and compares it to the
 * endianness passed into the function.
 *
 * INPUT:   int endianness to compare to
 *
 * OUTPUT:  true if endiannesses are the same
 *          false if not
 */
bool checkSameEndian(int programEndian) {
   int n = 1;
   int localEndian;

   // Check local endianness
   if ( *(char *)&n == 1 ) {
      localEndian = ENDIAN_LITTLE;
   } else {
      localEndian = ENDIAN_BIG;
   }

   // Return sameEndian bool
   return (localEndian == programEndian);
}

/**
 * A utility function that is given a 2byte int
 * and swaps the endianness of the int.
 *
 * INPUT:   int to swap endianness
 *
 * OUTPUT:  int with swapped endianness
 */
uint16_t endianSwap16Bit(uint16_t input) {

   union {
      uint16_t       machineWord;
      unsigned char  bytes[2];
   } overlay;

   overlay.machineWord = input;

   // swap bytes
   unsigned char temp;
   temp             = overlay.bytes[0];
   overlay.bytes[0] = overlay.bytes[1];
   overlay.bytes[1] = temp;

   // Return swapped word
   return (uint16_t)overlay.machineWord;
}

/**
 * A utility function that is given a 4byte int
 * and swaps the endianness of the int.
 *
 * INPUT:   int to swap endianness
 *
 * OUTPUT:  int with swapped endianness
 */
uint32_t endianSwap32Bit(uint32_t input) {

   union {
      uint32_t       machineWord;
      unsigned char  bytes[4];
   } overlay;

   overlay.machineWord = input;

   // swap bytes
   unsigned char temp;
   temp             = overlay.bytes[0];
   overlay.bytes[0] = overlay.bytes[3];
   overlay.bytes[3] = temp;

   temp             = overlay.bytes[1];
   overlay.bytes[1] = overlay.bytes[2];
   overlay.bytes[2] = temp;

   // Return swapped word
   return (uint32_t)overlay.machineWord;
}

/**
 * A utility function that is given a 8byte int
 * and swaps the endianness of the int.
 *
 * INPUT:   int to swap endianness
 *
 * OUTPUT:  int with swapped endianness
 */
uint64_t endianSwap64Bit(uint64_t input) {

   union {
      uint64_t       machineWord;
      unsigned char  bytes[8];
   } overlay;

   overlay.machineWord = input;

   // swap bytes
   unsigned char temp;
   temp             = overlay.bytes[0];
   overlay.bytes[0] = overlay.bytes[7];
   overlay.bytes[7] = temp;

   temp             = overlay.bytes[1];
   overlay.bytes[1] = overlay.bytes[6];
   overlay.bytes[6] = temp;

   temp             = overlay.bytes[2];
   overlay.bytes[2] = overlay.bytes[5];
   overlay.bytes[5] = temp;

   temp             = overlay.bytes[3];
   overlay.bytes[3] = overlay.bytes[4];
   overlay.bytes[4] = temp;

   // Return swapped word
   return (uint64_t)overlay.machineWord;
}

/**
 * A utility function to iterate over the section header
 * linked list and unallocate the memory used to store the
 * nodes in the list.  The list's head is then set to NULL.
 *
 * INPUT:   pointer to a pointer to the LL head
 *
 * OUTPUT:  none
 */
void freeSecLL(struct sectionNode **startSecLL) {

   struct sectionNode *currNode = *startSecLL;
   struct sectionNode *nextNode = currNode->next;

   // Check for empty LL
   if (currNode == NULL) {
      return;
   }

   // Check for one-long LL
   if (nextNode == NULL) {
      free(currNode);            // free node
      *startSecLL = NULL;        // set start ptr to NULL
   }

   // Iterate over LL, unallocating each node
   while (nextNode != NULL) {
      free(currNode);            // free node
      currNode = nextNode;       // increment currNode
      nextNode = currNode->next; // increment nextNode
   }

   // Unallocate tail node
   free(currNode);

   // Set start ptr to NULL
   *startSecLL = NULL;
};

