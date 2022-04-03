///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file sectionHeader.c
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Header file
#include "sectionHeaders.h"

/**
 * A function that scans each section header in a given file and allocates
 * memory for a linked list node to store the section data.  It then
 * updates the head of the linked list.
 *
 * INPUT:   pointer to the fileHeader for this file
 *          pointer to a pointer to the LL head
 *          pointer to the file
 *
 * OUTPUT:  none
 */
void readSectionHeaders(union fileHeader_union *fileHeader, struct sectionNode **startSecLL, FILE *file) {

   int i;
   int iter;

   int headerSize;
   int headerCount;
   long int start;

   char buffer[64] = {0};

   struct sectionNode *currNode = *startSecLL;
   struct sectionNode *newNode;

   // Update section variables
   headerSize  = fileHeader->Struct.e_shentsize;
   headerCount = fileHeader->Struct.e_shnum;
   start       = fileHeader->Struct.e_shoff;

   // Set file pointer to section header start
   fseek(file, start, SEEK_SET);

   // Iterate over each header
   for (iter=0; iter < headerCount; iter++) {

      // Re-initialize buffer
      for (i=0; i < headerSize; i++) {
        buffer[i] = 0;
      }

      // Populate buffer with section header
      for (i=0; i < headerSize; i++) {
        buffer[i] = fgetc(file);
      }

      // Allocate memory for section header
      newNode = malloc(sizeof(struct sectionNode));

      // Add node to LL
      if (currNode == NULL) {
         currNode = newNode;        // create head node
         *startSecLL = currNode;    // update head pointer
      } else {
         currNode->next = newNode;  // update previous node's .next
         currNode = newNode;        // update current node
      }

      // Update currNode values
      currNode->index = iter;
      currNode->next = NULL;
      memcpy(currNode->data.arr, buffer, 64);

      // Make sure union is in 64bit format
      if (fileHeader->arr[4] == BIT32) {
         sectionHeader32to64(&(currNode->data));
      }

      // Make sure file endian matches local endian
      if (!checkSameEndian(fileHeader->arr[5])) {
         sectionHeaderEndianSwap(&(currNode->data));
      }
   }

   // Update names of each section
   readSectionNames(*startSecLL, file);
}

/**
 * A function that iterates over the LL and copies the section name
 * to the node name.
 *
 * INPUT:   pointer to the linked list head
 *          pointer to the file
 *
 * OUTPUT:  none
 */
void readSectionNames(struct sectionNode *startSecLL, FILE *file) {

   uint64_t shstrndx;

   struct sectionNode *node = startSecLL;

   // Find the shstrndx header (end of LL)
   while (node->next != NULL) {
      node = node->next;
   }

   // Set shstrndx offset
   shstrndx = node->data.Struct.sh_offset;

   // Reset node variable
   node = startSecLL;

   // Iterate over each node
   while (node != NULL) {

      // Move file pointer to start of section name
      fseek(file, shstrndx + node->data.Struct.sh_name, SEEK_SET);

      // Copy name to node
      fgets(node->name, 128, file);
      
      // Increment node
      node = node->next;
   }
}

/**
 * A utility function that converts a sectionHeader's content from
 * 32bit to 64bit.  All other functions rely on the sectionHeader info
 * to be in 64bit format.
 *
 * INPUT:   pointer to the section header to update
 *
 * OUTPUT:  none
 */
void sectionHeader32to64(union sectionHeader_union *secHeader) {

   // Copy fields from 32struct to 64struct in reverse order
   secHeader->Struct.sh_entsize   = (uint64_t)secHeader->Struct32.sh_entsize;
   secHeader->Struct.sh_addralign = (uint64_t)secHeader->Struct32.sh_addralign;

   secHeader->Struct.sh_info      = secHeader->Struct32.sh_info;
   secHeader->Struct.sh_link      = secHeader->Struct32.sh_link;

   secHeader->Struct.sh_size      = (uint64_t)secHeader->Struct32.sh_size;
   secHeader->Struct.sh_offset    = (uint64_t)secHeader->Struct32.sh_offset;
   secHeader->Struct.sh_addr      = (uint64_t)secHeader->Struct32.sh_addr;
   secHeader->Struct.sh_flags     = (uint64_t)secHeader->Struct32.sh_flags;
}

/**
 * A utility function that converts a sectionHeader's content to
 * the same endianness as this program. All other functions rely
 * on the sectionHeader info to be the same endianness as the 
 * machine this is running on.
 *
 * INPUT:   pointer to the sectionHeader to update
 *
 * OUTPUT:  none
 */
void sectionHeaderEndianSwap(union sectionHeader_union *secHeader) {

   // Swap endianness
   secHeader->Struct.sh_name      = endianSwap32Bit(secHeader->Struct.sh_name);
   secHeader->Struct.sh_type      = endianSwap32Bit(secHeader->Struct.sh_type);

   secHeader->Struct.sh_flags     = endianSwap64Bit(secHeader->Struct.sh_flags);
   secHeader->Struct.sh_addr      = endianSwap64Bit(secHeader->Struct.sh_addr);
   secHeader->Struct.sh_offset    = endianSwap64Bit(secHeader->Struct.sh_offset);
   secHeader->Struct.sh_size      = endianSwap64Bit(secHeader->Struct.sh_size);

   secHeader->Struct.sh_link      = endianSwap32Bit(secHeader->Struct.sh_link);
   secHeader->Struct.sh_info      = endianSwap32Bit(secHeader->Struct.sh_info);

   secHeader->Struct.sh_addralign = endianSwap64Bit(secHeader->Struct.sh_addralign);
   secHeader->Struct.sh_entsize   = endianSwap64Bit(secHeader->Struct.sh_entsize);
}

/**
 * A function that prints general section header data before printing
 * the actual sectionHeader.  This is only done if the [-h] opt is not
 * called, since that prints the same data.
 *
 * INPUT:   pointer to the fileHeader
 *
 * OUTPUT:  none
 */
void printSectionInfo(const union fileHeader_union *fileHeader) {
   // Print general information for headers
   printf("There are %d section headers, starting at offset 0x%lx:",
         fileHeader->Struct.e_shnum, fileHeader->Struct.e_shoff);
   printf("\n");
}

/**
 * A function that iterates over the section header linked list,
 * printing each node's data as it goes.
 *
 * INPUT:   pointer to the section LL head
 *
 * OUTPUT:  none
 */
void printSectionHeaders(struct sectionNode *startSecLL) {

   struct sectionNode *node = startSecLL;
   union sectionHeader_union *secHeader;

   // Find the shstrndx header (end of LL)
   while (node->next != NULL) {
      node = node->next;
   }

   // Reset node variable
   node = startSecLL;

   // Print formatting info
   printf("\n");
   printf("Section Headers:\n");
   printf("  [Nr] Name\n");

   printf("       Type\t\tAddress\t\t  Offset\t    Link\n");
   printf("       Size\t\tEntSize\t\t  Info\t\t    Align\n");
   printf("       Flags\n");

   // Iterate over each node
   while (node != NULL) {

      // Set union pointer to current node's union
      secHeader = &(node->data);

      // === LINE ONE ===

      // Print header number
      printf("  [%2d] ", node->index);

      // Print sh_name 
      printf("%s\n", node->name);

      // === LINE TWO ===
      printf("       ");

      // Print sh_type
      switch(secHeader->Struct.sh_type) {
         case 0:
            printf("NULL\t\t");
            break;
         case 1:
            printf("PROGBITS\t\t");
            break;
         case 2:
            printf("SYMTAB\t\t");
            break;
         case 3:
            printf("STRTAB\t\t");
            break;
         case 4:
            printf("RELA\t\t");
            break;
         case 5:
            printf("HASH\t\t");
            break;
         case 6:
            printf("DYNAMIC\t\t");
            break;
         case 7:
            printf("NOTE\t\t");
            break;
         default:
            printf("???\t\t");
      }

      // Print sh_addr
      printf("%016lx  ", secHeader->Struct.sh_addr);

      // Print sh_offset
      printf("%016lx  ", secHeader->Struct.sh_offset);

      // Print sh_link
      printf("%d\n", secHeader->Struct.sh_link);

      // === LINE THREE ===
      printf("       ");

      // Print sh_size
      printf("%016lx ", secHeader->Struct.sh_size);

      // Print sh_entsize
      printf("%016lx  ", secHeader->Struct.sh_entsize);

      // Print sh_info
      printf("%-16d  ", secHeader->Struct.sh_info);

      // Print sh_addralign
      printf("%-16ld\n", secHeader->Struct.sh_addralign);

      // === LINE FOUR ===
      printf("       ");

      // Print sh_flags
      printf("[%016lx]", secHeader->Struct.sh_flags);
      printf("\n");

      // Update node
      node = node->next;
   }
}

