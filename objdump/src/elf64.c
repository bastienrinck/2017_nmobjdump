/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <stdio.h>
#include "my_objdump.h"
#include "flags.h"

static int search_flags64(Elf64_Shdr *shdr, Elf64_Ehdr *ehdr)
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

void objdp64_format(void *data, char *filename)
{
	Elf64_Ehdr *elf;
	Elf64_Shdr *elf64_shdr;

	elf = (Elf64_Ehdr *)data;
	elf64_shdr = (Elf64_Shdr * )(data + elf->e_shoff);
	printf("\n%s:%5cfile format elf64-x86-64\n", filename, ' ');
	printf("architecture: i386:x86-64, flags 0x%08x:\n",
		search_flags64(elf64_shdr, elf));
	print_bfd_flags((unsigned int)search_flags64(elf64_shdr, elf));
	printf("start address 0x%016x\n\n", (int)elf->e_entry);
}