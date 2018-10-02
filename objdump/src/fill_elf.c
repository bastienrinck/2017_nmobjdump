/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <string.h>
#include <malloc.h>
#include "my_objdump.h"

static bool check_file_type(elf_t *elf)
{
	bool ret = true;

	if (!((Elf64_Ehdr *)elf->mmap)->e_ident[EI_CLASS])
		ret = print_err(elf->filename, "Invalid class", false);
	elf->type = (char)(((Elf64_Ehdr *)elf->mmap)->e_ident[EI_CLASS] - 1);
	return (ret);
}

static bool fill_shstr(const char *filename, elf_t *elf)
{
	bool ret = true;

	if (elf->type && ((E64)elf->ehdr)->e_shstrndx != SHN_UNDEF)
		elf->shstr = elf->mmap +
			((S64)elf->shdr)[((E64)elf->ehdr)->e_shstrndx].sh_offset;
	else if (!elf->type && ((E32)elf->ehdr)->e_shstrndx != SHN_UNDEF)
		elf->shstr = elf->mmap +
			((S32)elf->shdr)[((E32)elf->ehdr)->e_shstrndx].sh_offset;
	else
		ret = print_err(filename,
			"Invalid section header string table index", false);
	return (ret);
}

static bool init_looper(elf_t *elf, int *iter, struct stat *s)
{
	elf->ehdr = elf->mmap;
	if (!isElf((Elf64_Ehdr *)elf->mmap))
		return (print_err(elf->filename, "File format not recognized",
			false));
	if (!check_file_type(elf))
		return (false);
	if ((elf->type ? ((E64)elf->ehdr)->e_shoff :
		((E32)elf->ehdr)->e_shoff) > ((unsigned)s->st_size))
		return (print_err(elf->filename, "File truncated", false));
	*iter = elf->type ? ((E64)elf->ehdr)->e_shnum :
		((E32)elf->ehdr)->e_shnum;
	elf->shdr = (elf->type) ? elf->mmap + ((E64)elf->ehdr)->e_shoff :
		elf->mmap + ((E32)elf->ehdr)->e_shoff;
	if (!fill_shstr(elf->filename, elf))
		return (false);
	return (true);
}

static void set_section(elf_t *elf, void *section, section_t *storage)
{
	uint32_t size;

	storage->name = elf->shstr +
		(elf->type ? ((S64)section)->sh_name : ((S32)section)->sh_name);
	unsigned char *data = elf->mmap +
		(elf->type ? ((S64)section)->sh_offset :
			((S32)section)->sh_offset);
	size = (uint32_t)(
		elf->type ? ((S64)section)->sh_size : ((S32)section)->sh_size);
	storage->content = malloc(sizeof(*storage->content) * size * 2 + 64);
	storage->addr =
		elf->type ? ((S64)section)->sh_addr : ((S32)section)->sh_addr;
	memset(storage->content, 0, size * 2 + 64);
	char *c = storage->content;
	char a[] = "0123456789abcdef";
	for (uint32_t i = 0; i < size; ++i) {
		c[i * 2] = a[data[i] / 16];
		c[i * 2 + 1] = a[data[i] % 16];
	}
}

bool section_looper(elf_t *elf, struct stat *s)
{
	int iter;
	section_t section;
	static const char *exSec[] = {".symtab", ".strtab", ".shstrtab",
		".bss"};

	if (!init_looper(elf, &iter, s))
		return (false);
	if (elf->type)
		objdp64_format(elf->mmap, elf->filename);
	else
		objdp32_format(elf->mmap, elf->filename);
	for (int i = 0; i < iter; i++) {
		set_section(elf, elf->type ? (void *)(((S64)elf->shdr) + i) :
			(void *)(((S32)elf->shdr) + i), &section);
		for (int j = 0; j < 4; j++)
			if (!strcmp(section.name, exSec[j])) {
				section.name = NULL;
				break;
			}
		if (section.name && section.name[0])
			print_section(&section);
	}
	return (true);
}