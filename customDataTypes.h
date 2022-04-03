///////////////////////////////////////////////////////////////////////////////
/// Software Reverse Engineering
/// ReadELF 
///
/// @file customDataTypes.h
/// @version 1.0
///
/// @author Robert Lemon <rlemon@hawaii.edu>
/// @date 05_mar_2021
///////////////////////////////////////////////////////////////////////////////

// Includes
#include <stdint.h>

/**
 * Holds the content of an ELF file header.
 *  - Can take in data from 32bit or 64bit, but
 *    should get translated to 64bit for use.
 *  - Can take little endian or big endian, but
 *    should get translated to local endian
 *    for use.
 */
union fileHeader_union
{
   struct fileHeader_struct32
   {
      uint8_t EI_MAG0;
      uint8_t EI_MAG1;
      uint8_t EI_MAG2;
      uint8_t EI_MAG3;
      uint8_t EI_CLASS;
      uint8_t EI_DATA;
      uint8_t EI_VERSION;
      uint8_t EI_OSABI;
      uint8_t EI_ABIVERSION; 

      unsigned char EI_PAD[7];
   
      uint16_t e_type;
      uint16_t e_machine;
      uint32_t e_version;
      uint32_t e_entry;
      uint32_t e_phoff;
      uint32_t e_shoff;
      uint32_t e_flags;
      uint16_t e_ehsize;
      uint16_t e_phentsize;
      uint16_t e_phnum;
      uint16_t e_shentsize;
      uint16_t e_shnum;
      uint16_t e_shstrndx;
   }Struct32;

   // Struct for 64bit machines
   struct fileHeader_struct64
   {
      uint8_t EI_MAG0;
      uint8_t EI_MAG1;
      uint8_t EI_MAG2;
      uint8_t EI_MAG3;
      uint8_t EI_CLASS;
      uint8_t EI_DATA;
      uint8_t EI_VERSION;
      uint8_t EI_OSABI;
      uint8_t EI_ABIVERSION; 

      unsigned char EI_PAD[7];
   
      uint16_t e_type;
      uint16_t e_machine;
      uint32_t e_version;
      uint64_t e_entry;
      uint64_t e_phoff;
      uint64_t e_shoff;
      uint32_t e_flags;
      uint16_t e_ehsize;
      uint16_t e_phentsize;
      uint16_t e_phnum;
      uint16_t e_shentsize;
      uint16_t e_shnum;
      uint16_t e_shstrndx;
   }Struct;

   // Array version
   unsigned char arr[64];

};

/**
 * Holds the content of a section header.
 *  - Can take in data from 32bit or 64bit, but
 *    should get translated to 64bit for use.
 *  - Can take little endian or big endian, but
 *    should get translated to local endian
 *    for use.
 */
union sectionHeader_union
{
   // Struct for 32bit machines
   struct sectionHeader_struct32
   {
      uint32_t sh_name;
      uint32_t sh_type;
      uint32_t sh_flags;
      uint32_t sh_addr;
      uint32_t sh_offset;
      uint32_t sh_size;
      uint32_t sh_link;
      uint32_t sh_info;
      uint32_t sh_addralign;
      uint32_t sh_entsize;
   }Struct32;

   // Struct for 64bit machines
   struct sectionHeader_struct64
   {
      uint32_t sh_name;
      uint32_t sh_type;
      uint64_t sh_flags;
      uint64_t sh_addr;
      uint64_t sh_offset;
      uint64_t sh_size;
      uint32_t sh_link;
      uint32_t sh_info;
      uint64_t sh_addralign;
      uint64_t sh_entsize;
   }Struct;
  
   // Array version of structs
   char arr[64];
};

/** A node for the section header LL.
 *  Used to dynamically store section info.
 */
struct sectionNode
{
   int index;
   char name[128];
   union sectionHeader_union data;
   struct sectionNode* next;
};


