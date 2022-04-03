///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file hexDump.c
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Header file
#include "hexDump.h"

/**
 * A function that iterates over the sectionHeader LL, looking for a node 
 * based on the user-inputted section name.  If the node is found, it
 * updates the one-hot encoding array that corresponds to which nodes are
 * meant to be hex dumped.
 *
 * INPUT:   pointer to the section LL
 *          string of user-inputted name
 *          bool array of one-hot-encoding
 *
 * OUTPUT:  true if node found
 *          false otherwise
 *
 * @todo: allow user to enter section index instead of name
 */
bool findNode(struct sectionNode *startSecLL, const char *secName, bool *hexDumpNodes) {

   bool foundNode = false;
   struct sectionNode *node = startSecLL;

   // Iterate over the section LL
   while (node != NULL && !foundNode) {

      // Check if node's name matches input
      if (strcmp(secName, node->name) == 0) {
         foundNode = true;
      }

      // Increment node if wrong node
      else {
         node = node->next;
      }
   }

   // Update one-hot encoding of hex dump nodes
   if (foundNode) {
      hexDumpNodes[node->index] = true;
   }

   // Return whether or not node was found
   return foundNode;
}

/**
 * A function that prints the hex dump of a give node.  This function 
 * iterates of the LL and finds the node to print, then prints the 
 * name.  A second function is called to print the actual hex values.
 *
 * INPUT:   pointer to the section LL head
 *          pointer to the file to dump
 *          boolean array of the one-hot-encoding
 *
 * OUTPUT:  none
 */
void printHexDump(struct sectionNode *startSecLL, FILE *file, bool *hexDumpNodes) {

   int size;
   uint64_t addr;
   struct sectionNode *node = startSecLL;

   // Iterate over the section LL
   while (node != NULL) {

      // Dump nodes associated with [-x] opt
      if (hexDumpNodes[node->index]) {

         // Get size of section
         size = node->data.Struct.sh_size;

         // Check for empty section
         if (size == 0) {
            printf("Section '%s' has no data to dump.\n", node->name); 
            node = node->next;
            continue;
         }

         // Print section name
         printf("\n");
         printf("Hex dump of section '%s':\n", node->name);

         // Jump to section in file
         addr = node->data.Struct.sh_offset;
         fseek(file, addr, SEEK_SET);

         // Print hex body
         printHexBody(size, file);
      }

      // Increment node
      node = node->next;
   }
}

/**
 * A function called by printHexDump to iterate over a section
 * and print its content in hex format.
 *
 * INPUT:   int size of the section
 *          pointer to location in file to print from
 *
 * OUTPUT:  none
 */
void printHexBody(const int secSize, FILE *file) {

   int i;

   int addr = 0;
   int currByte = 0;
   int bufferSize = 0;

   unsigned char buffer[16];

   // Iterate over the the lines to print
   while (currByte < secSize) {
      
      // Determine amount of bytes in next line
      if ( (secSize - currByte) >= 16 ) {
         bufferSize = 16;                    // full 16byte line to print
      } else {
         bufferSize = secSize - currByte;    // < 16bytes to print
      }

      // Scan bytes to buffer
      for (i=0; i < bufferSize; i++) {
         buffer[i] = fgetc(file);
      }

      // Print and increment the address
      printf("  0x%08x ", addr);
      addr += 0x10;

      // Print the bytes in hex
      for (i=0; i < bufferSize; i++) {
         printf("%02x", buffer[i]);
         // Print space between every 4bytes
         if ( ((i+1) % 4) == 0 ) {
            printf(" ");
         }
      }

      // Ensure ASCII Printout is in right place
      for (i=bufferSize; i < 16; i++) {
         printf("  ");
         if ( ((i+1) % 4) == 0 ) {
            printf(" ");
         }
      }

      // Print the bytes in ASCII
      for (i=0; i < bufferSize; i++) {
         if (buffer[i] >= 0x20 && buffer[i] <= 0x7e) {   // Prints ASCII chars
            printf("%c", buffer[i]);
         } else {                                        // Prints . for non-ASCII
            printf(".");
         }
      }
      printf("\n");

      // Update loop param
      currByte += bufferSize;
   }

   printf("\n");
}

