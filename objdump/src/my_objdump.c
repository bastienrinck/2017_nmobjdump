/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <zconf.h>
#include <stdlib.h>
#include "my_objdump.h"

bool isElf(Elf64_Ehdr *elf)
{
	return (elf->e_ident[EI_MAG0] == ELFMAG0 &&
		elf->e_ident[EI_MAG1] == ELFMAG1 &&
		elf->e_ident[EI_MAG2] == ELFMAG2 &&
		elf->e_ident[EI_MAG3] == ELFMAG3);
}

bool print_err(const char *filename, const char *string, bool quote)
{
	dprintf(2, quote ? "objdump: '%s': %s\n" : "objdump: %s: %s\n",
		filename, string);
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

static bool my_objdump(const char *filename)
{
	int fd;
	elf_t elf;
	struct stat s;
	bool ret;

	elf.filename = (char *)filename;
	if ((fd = open(filename, O_RDONLY)) != -1) {
		if ((ret = load_mmap(fd, &elf.mmap, filename, &s)))
			ret = check_archive(&elf, true, &s);
		close(fd);
	} else
		ret = print_err(filename, "No such file", true);
	return (ret);
}

int main(int ac, char **av)
{
	int ret = EXIT_SUCCESS;

	setbuf(stdout, NULL);
	if (ac == 1)
		ret = (my_objdump("a.out")) ? ret : EXIT_FAILURE;
	for (int i = 1; i < ac; i++)
		ret += my_objdump(av[i]);
	return (ret);
}
