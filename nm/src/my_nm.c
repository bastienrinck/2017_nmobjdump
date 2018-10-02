/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <zconf.h>
#include <stdlib.h>
#include "my_nm.h"

bool print_err(const char *filename, const char *string, bool quote)
{
	dprintf(2, quote ? "nm: '%s': %s\n" : "nm: %s: %s\n", filename, string);
	return (false);
}

static bool load_mmap(int fd, void **buf, const char *filename, struct stat *s)
{
	bool ret = true;

	fstat(fd, s);
	if (((*buf) = mmap(NULL, (size_t)s->st_size, PROT_READ, MAP_PRIVATE, fd,
		0)) == MAP_FAILED)
		ret = print_err(filename, "mmap error", false);
	return (ret);
}

bool init_elf(elf_t *elf, bool displayFN, struct stat *s)
{
	bool ret;
	sym_t *list = NULL;
	size_t size = 0;

	if (isElf((Elf64_Ehdr *)elf->mmap)) {
		if (!(ret = check_file_type(elf)))
			return (ret);
		elf->ehdr = elf->mmap;
		if ((elf->type ? ((E64)elf->ehdr)->e_shoff :
			((E32)elf->ehdr)->e_shoff) > ((unsigned)s->st_size))
			return (print_err(elf->filename, "File truncated",
				false));
		elf->shdr = (elf->type) ?
			elf->mmap + ((Elf64_Ehdr *)elf->ehdr)->e_shoff :
			elf->mmap + ((Elf32_Ehdr *)elf->ehdr)->e_shoff;
		if (!fill_shstr(elf->filename, elf) ||
			!get_symtab(elf->filename, elf, &size) ||
			!get_strtab(elf->filename, elf))
			return (false);
		elf->type ? generate_sym_list64(elf, list, size, displayFN) :
			generate_sym_list32(elf, list, size, displayFN);
	} else
		ret = print_err(elf->filename, "File format not recognized",
			false);
	return (ret);
}

static bool my_nm(const char *filename, bool displayFN)
{
	int fd;
	elf_t elf;
	struct stat s;
	bool ret;

	elf.filename = (char *)filename;
	if ((fd = open(filename, O_RDONLY)) != -1) {
		if ((ret = load_mmap(fd, &elf.mmap, filename, &s)))
			ret = check_archive(&elf, displayFN, &s);
		close(fd);
	} else
		ret = print_err(filename, "No such file", true);
	return (ret);
}

int main(const int ac, const char *const *const av)
{
	int ret = EXIT_SUCCESS;

	if (ac == 1)
		ret = (my_nm("a.out", false)) ? ret : EXIT_FAILURE;
	for (int i = 1; i < ac; i++)
		ret += (my_nm(av[i], ac > 2)) ? EXIT_SUCCESS : EXIT_FAILURE;
	return (ret);
}