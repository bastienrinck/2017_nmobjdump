/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <string.h>
#include <stdio.h>
#include "my_nm.h"

bool isElf(Elf64_Ehdr *elf)
{
	return (elf->e_ident[EI_MAG0] == ELFMAG0
		&& elf->e_ident[EI_MAG1] == ELFMAG1
		&& elf->e_ident[EI_MAG2] == ELFMAG2
		&& elf->e_ident[EI_MAG3] == ELFMAG3);
}

bool check_file_type(elf_t *elf)
{
	bool ret = true;

	if (!((Elf64_Ehdr *)elf->mmap)->e_ident[EI_CLASS])
		ret = print_err(elf->filename, "Invalid class", false);
	elf->type = (char)(((Elf64_Ehdr *)elf->mmap)->e_ident[EI_CLASS] - 1);
	return (ret);
}

bool fill_shstr(const char *filename, elf_t *elf)
{
	bool ret = true;

	if (elf->type && ((E64)elf->ehdr)->e_shstrndx != SHN_UNDEF)
		elf->shstr = &((S64)elf->shdr)[((E64)elf->ehdr)->e_shstrndx];
	else if (!elf->type && ((E32)elf->ehdr)->e_shstrndx != SHN_UNDEF)
		elf->shstr = &((S32)elf->shdr)[((E32)elf->ehdr)->e_shstrndx];
	else
		ret = print_err(filename,
			"Invalid section header string table index", false);
	return (ret);
}

bool get_strtab(const char *filename, elf_t *elf)
{
	int iter = elf->type ? ((E64)elf->ehdr)->e_shnum :
		((E32)elf->ehdr)->e_shnum;

	for (int i = 0; i < iter; ++i) {
		if (elf->type &&
			!strcmp(elf->mmap + ((S64)elf->shstr)->sh_offset +
				((S64)elf->shdr)[i].sh_name, ".strtab")) {
			elf->strtab = elf->mmap + ((S64)elf->shdr)[i].sh_offset;
			return (true);
		} else if (!elf->type &&
			!strcmp(elf->mmap + ((S32)elf->shstr)->sh_offset +
				((S32)elf->shdr)[i].sh_name, ".strtab")) {
			elf->strtab = elf->mmap + ((S32)elf->shdr)[i].sh_offset;
			return (true);
		}
	}
	return (print_err(filename, "Unable to locate .strtab section", false));
}

bool get_symtab(const char *filename, elf_t *elf, size_t *size)
{
	int iter = elf->type ? ((E64)elf->ehdr)->e_shnum :
		((E32)elf->ehdr)->e_shnum;

	for (int i = 0; i < iter; ++i)
		if (elf->type && ((S64)elf->shdr)[i].sh_type == SHT_SYMTAB) {
			elf->symstr = elf->mmap + ((S64)elf->shdr)[i].sh_offset;
			*size = ((S64)elf->shdr)[i].sh_size /
				((S64)elf->shdr)[i].sh_entsize;
			return (true);
		} else if (!elf->type &&
			((S32)elf->shdr)[i].sh_type == SHT_SYMTAB) {
			elf->symstr = elf->mmap + ((S32)elf->shdr)[i].sh_offset;
			*size = ((S32)elf->shdr)[i].sh_size /
				((S32)elf->shdr)[i].sh_entsize;
			return (true);
		}
	dprintf(2, "nm: %s: %s\n", filename, "no symbols");
	return (false);
}