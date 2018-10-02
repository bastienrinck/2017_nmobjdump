/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <stdio.h>
#include "my_objdump.h"
#include "flags.h"

static int search_flags32(Elf32_Shdr *shdr, Elf32_Ehdr *ehdr)
{
	int flags = 0;

	for (int i = 0; i < ehdr->e_shnum; ++i) {
		shdr = ((void *)ehdr) + ehdr->e_shoff;
		shdr = &(shdr[i]);
		if (shdr->sh_type == SHT_REL || shdr->sh_type == SHT_RELA)
			flags |= HAS_RELOC;
	}
	if (ehdr->e_type == ET_EXEC) {
		flags &= ~HAS_RELOC;
		flags |= EXEC_P | D_PAGED;
	}
	if (ehdr->e_type == ET_DYN) {
		flags &= ~HAS_RELOC;
		flags |= DYNAMIC | D_PAGED;
	}
	flags |= HAS_SYMS;
	return (flags);
}

void objdp32_format(void *data, char *filename)
{
	Elf32_Ehdr *elf;
	Elf32_Shdr *elf32_shdr;

	elf = (Elf32_Ehdr *)data;
	elf32_shdr = (Elf32_Shdr * )(data + elf->e_shoff);
	printf("\n%s:%5cfile format elf32-i386\n", filename, ' ');
	printf("architecture: i386, flags 0x%08x:\n",
		search_flags32(elf32_shdr, elf));
	print_bfd_flags((unsigned int)search_flags32(elf32_shdr, elf));
	printf("start address 0x%08x\n\n", (int)elf->e_entry);
}
