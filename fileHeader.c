///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file fileHeader.c
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Header file
#include "fileHeader.h"

/**
 * A function that reads a file's ELF header and stores the hex
 * values in a struct, whose pointer is passed to the function.
 * An endian-dependent utility function is used to help store the
 * header's content.
 *
 * INPUT:   pointer to struct to store header info
 *          pointer to file to read
 *
 * OUTPUT:  true if the ELF header was successfully read
 *          false if not
 */
bool readFileHeader(union fileHeader_union *fileHeader, FILE *file) {

   int i;
   char buffer[64];

   // Read file into buffer
   for (i = 0; i < 64; i++) {
      buffer[i] = fgetc(file);
   }

   // Verify file is an ELF
   if ( buffer[0] != 0x7f || buffer[1] != 'E' || buffer[2] != 'L' || buffer[3] != 'F' ) {
      return false; // tell parent function that file is incorrect
   }

   // Copy buffer to union
   memcpy(fileHeader->arr, buffer, 64);

   // Make sure union is in 64bit format
   if (fileHeader->arr[4] == BIT32) {
      fileHeader32to64(fileHeader);
   }

   // Make sure file endian matches local endian
   if (!checkSameEndian(fileHeader->arr[5])) {
      fileHeaderEndianSwap(fileHeader);
   }

   // Tell parent funtion that header successfully read
   return true;
}

/**
 * A utility function that converts a fileHeader's content from
 * 32bit to 64bit.  All other functions rely on the fileHeader info
 * to be in 64bit format.
 *
 * INPUT:   pointer to the fileHeader to update
 *
 * OUTPUT:  none
 */
void fileHeader32to64(union fileHeader_union *fileHeader) {

   // Copy regular fields from 32struct to 64struct in reverse order
   fileHeader->Struct.e_shstrndx  = fileHeader->Struct32.e_shstrndx;
   fileHeader->Struct.e_shnum     = fileHeader->Struct32.e_shnum;
   fileHeader->Struct.e_shentsize = fileHeader->Struct32.e_shentsize;
   fileHeader->Struct.e_phnum     = fileHeader->Struct32.e_phnum;
   fileHeader->Struct.e_phentsize = fileHeader->Struct32.e_phentsize;
   fileHeader->Struct.e_ehsize    = fileHeader->Struct32.e_ehsize;
   fileHeader->Struct.e_flags     = fileHeader->Struct32.e_flags;

   // Copy pointer fields from 32struct to 64struct in reverse order
   fileHeader->Struct.e_shoff     = (uint64_t)fileHeader->Struct32.e_shoff;
   fileHeader->Struct.e_phoff     = (uint64_t)fileHeader->Struct32.e_phoff;
   fileHeader->Struct.e_entry     = (uint64_t)fileHeader->Struct32.e_entry;
}

/**
 * A utility function that converts a fileHeader's content to
 * the same endianness as this program. All other functions rely
 * on the fileHeader info to be the same endianness as the 
 * machine this is running on.
 *
 * INPUT:   pointer to the fileHeader to update
 *
 * OUTPUT:  none
 */
void fileHeaderEndianSwap(union fileHeader_union *fileHeader) {

   // Swap endianness
   fileHeader->Struct.e_type      = endianSwap16Bit(fileHeader->Struct.e_type);
   fileHeader->Struct.e_machine   = endianSwap16Bit(fileHeader->Struct.e_machine);

   fileHeader->Struct.e_version   = endianSwap32Bit(fileHeader->Struct.e_version);

   fileHeader->Struct.e_entry     = endianSwap64Bit(fileHeader->Struct.e_entry);
   fileHeader->Struct.e_phoff     = endianSwap64Bit(fileHeader->Struct.e_phoff);
   fileHeader->Struct.e_shoff     = endianSwap64Bit(fileHeader->Struct.e_shoff);

   fileHeader->Struct.e_flags     = endianSwap32Bit(fileHeader->Struct.e_flags);

   fileHeader->Struct.e_ehsize    = endianSwap16Bit(fileHeader->Struct.e_ehsize);
   fileHeader->Struct.e_phentsize = endianSwap16Bit(fileHeader->Struct.e_phentsize);
   fileHeader->Struct.e_phnum     = endianSwap16Bit(fileHeader->Struct.e_phnum);
   fileHeader->Struct.e_shentsize = endianSwap16Bit(fileHeader->Struct.e_shentsize);
   fileHeader->Struct.e_shnum     = endianSwap16Bit(fileHeader->Struct.e_shnum);
   fileHeader->Struct.e_shstrndx  = endianSwap16Bit(fileHeader->Struct.e_shstrndx);
}

/**
 * A function that processes and prints the ELF header info, stored in
 * a struct whose pointer is passed in.  A utility function is used to
 * convert the multi-byte values into an int.
 *
 * INPUT:   pointer to struct to store header info
 *
 * OUTPUT:  none
 */
void printFileHeader(const union fileHeader_union *fileHeader) {

   int i;
   int bit;
   char buffer[256];
   char magicBytes[16];

   // Print magic bytes
   memcpy(magicBytes, fileHeader->arr, 16);
   bit = magicBytes[4];

   printf("ELF Header:\n");
   printf("  Magic:    ");
   for (i = 0; i < 15; i++) {
      printf("%02x ", magicBytes[i]);
   }
   printf("%02x\n", magicBytes[i]);
  
   // Print EI_CLASS
   switch(magicBytes[4]) {
      case BIT32:
         strcpy(buffer, "ELF32");
         break;
      case BIT64:
         strcpy(buffer, "ELF64");
         break;
      default:
         strcpy(buffer, "Unsupported EI_CLASS type!");
   }
   printf("  Class:\t\t\t     %s\n", buffer);

   // Print EI_DATA
   switch(bit) {
      case ENDIAN_LITTLE:
         strcpy(buffer, "2\'s complement, little endian");
         break;
      case ENDIAN_BIG:
         strcpy(buffer, "2\'s complement, big endian");
         break;
      default:
         strcpy(buffer, "Unsupported EI_DATA type!");
   }
   printf("  Data:\t\t\t\t     %s\n", buffer);

   // Print EI_VERSION
   switch(magicBytes[6]) {
      case 0x01:
         printf("  Version:\t\t\t     1 (current)\n");
         break;
      default:
         printf("  Version:\t\t\t     %d\n", (int)(magicBytes[6]));
   }

   // Print EI_OSABI
   switch(magicBytes[7]) {
      case 0x00:
         strcpy(buffer, "UNIX - System V");
         break;
      case 0x01:
         strcpy(buffer, "HP-UX");
         break;
      case 0x02:
         strcpy(buffer, "NetBSD");
         break;
      case 0x03:
         strcpy(buffer, "Linux");
         break;
      case 0x04:
         strcpy(buffer, "GNU Hurd");
         break;
      default:
         strcpy(buffer, "Unsupported EI_OSABI type!");
   }
   printf("  OS/ABI:\t\t\t     %s\n", buffer);

   // Print EI_ABIVERSION
   printf("  ABI Version:\t\t\t     %d\n", (int)magicBytes[8]);

   // Print e_type
   switch(fileHeader->Struct.e_type) {
      case 0x00:
         strcpy(buffer, "No file type");
         break;
      case 0x01:
         strcpy(buffer, "Relocatable file");
         break;
      case 0x02:
         strcpy(buffer, "Executable file");
         break;
      case 0x03:
         strcpy(buffer, "DYN (Shared object file)");
         break;
      case 0x04:
         strcpy(buffer, "Core file");
         break;
      default:
         strcpy(buffer, "Unsupported e_file type!");
   }
   printf("  Type:\t\t\t\t     %s\n", buffer);

   // Print e_machine
   switch(fileHeader->Struct.e_machine) {
      case 0x00:
         strcpy(buffer, "No machine");
         break;
      case 0x01:
         strcpy(buffer, "AT&T WE 32100");
         break;
      case 0x02:
         strcpy(buffer, "SPARC");
         break;
      case 0x03:
         strcpy(buffer, "Intel 80386");
         break;
      case 0x28:
         strcpy(buffer, "ARM");
         break;
      case 0x3e:
         strcpy(buffer, "Advanced Micro Devices X86-64");
         break;
      default:
         strcpy(buffer, "Unsupported e_file type!");
   }
   printf("  Machine:\t\t\t     %s\n", buffer);

   // Print e_version
   printf("  Version:\t\t\t     0x%x\n", fileHeader->Struct.e_version);

   // Print e_entry
   printf("  Entry point address:\t\t     %p\n", (void *)fileHeader->Struct.e_entry);

   // Print e_phoff
   printf("  Start of program headers:\t     %ld (bytes into file)\n",
         fileHeader->Struct.e_phoff);

   // Print e_shoff
   printf("  Start of section headers:\t     %ld (bytes into file)\n",
         fileHeader->Struct.e_shoff);

   // Print e_flags
   printf("  Flags:\t\t\t     0x%x\n", fileHeader->Struct.e_flags);

   // Print e_ehsize
   printf("  Size of this header:\t\t     %d (bytes)\n", fileHeader->Struct.e_ehsize);

   // Print e_phentsize
   printf("  Size of program headers:\t     %d (bytes)\n", fileHeader->Struct.e_phentsize);

   // Print e_phnum
   printf("  Number of program headers:\t     %d\n", fileHeader->Struct.e_phnum);

   // Print e_shentsize
   printf("  Size of section headers:\t     %d (bytes)\n", fileHeader->Struct.e_shentsize);

   // Print e_shnum
   printf("  Number of section headers:\t     %d\n", fileHeader->Struct.e_shnum);

   // Print e_shstrndx
   printf("  Section header string table index: %d\n", fileHeader->Struct.e_shstrndx);
}

