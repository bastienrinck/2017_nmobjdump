/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <zconf.h>
#include <stdlib.h>
#include <string.h>
#include "my_nm.h"

static char get_type64_2(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char c = '?';

	if (shdr[sym.st_shndx].sh_type == SHT_NOBITS &&
		shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
		shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
		shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
		shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	return (c);
}

static char get_type64(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
		c = (char)((sym.st_shndx == SHN_UNDEF) ? 'w' : 'W');
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK &&
		ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
		c = (char)((sym.st_shndx == SHN_UNDEF) ? 'v' : 'V');
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';
	else
		c = get_type64_2(sym, shdr);
	return ((ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?') ?
		(char)(c + 32) : c);
}

void generate_sym_list64(elf_t *elf, sym_t *list, size_t size, bool displayFN)
{
	sym_t *temp = NULL;
	char sym;

	for (size_t i = 1; i < size; i++) {
		sym = get_type64(((Elf64_Sym *)elf->symstr)[i], elf->shdr);
		if (((char *)(elf->strtab +
			((Elf64_Sym *)elf->symstr)[i].st_name))[0] &&
			sym != 'a') {
			temp = malloc(sizeof(sym_t));
			memset(temp, 0, sizeof(sym_t));
			temp->value = ((Elf64_Sym *)elf->symstr)[i].st_value;
			temp->name = (char *)(elf->strtab +
				((Elf64_Sym *)elf->symstr)[i].st_name);
			temp->sym = sym;
			list = push_back(list, temp);
		}
	}
	list = sort_list(list);
	display_list(list, displayFN, elf->filename, true);
	free(temp);
}