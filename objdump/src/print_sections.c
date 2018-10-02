/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include "my_objdump.h"
#include "flags.h"

void print_bfd_flags(unsigned int flags)
{
	unsigned printed = 0;
	const unsigned bin[] = {flags & HAS_RELOC, flags & EXEC_P,
		flags & HAS_LINENO, flags & HAS_DEBUG, flags & HAS_SYMS,
		flags & HAS_LOCALS, flags & DYNAMIC, flags & WP_TEXT,
		flags & D_PAGED};
	static const char *const strs[] = {"HAS_RELOC", "EXEC_P", "HAS_LINENO",
		"HAS_DEBUG", "HAS_SYMS", "HAS_LOCALS", "DYNAMIC", "WP_TEXT",
		"D_PAGED",};

	for (unsigned i = 0; i < 9; i++)
		if (bin[i])
			printf("%s%s", (printed++ ? ", " : ""), strs[i]);
	printf("\n");
}

static void print_section_indent(section_t *data, uint32_t i, uint32_t j)
{
	if ((i + 1) % 32 == 0) {
		for (uint32_t a = j % 36; a < 36; ++a)
			write(1, " ", 1);
		write(1, " ", 1);
		for (uint32_t o = 0; o < 32; o += 2) {
			char c[3] = {data->content[i - 31 + o],
				data->content[i - 30 + o], 0};
			char r = (char)strtoul(c, NULL, 16);
			if (i - 30 + o < strlen(data->content))
				write(1, (r >= 32 && r <= 126) ? &r : ".", 1);
			else
				write(1, " ", 1);
		}
		write(1, "\n", 1);
	}
}

void print_section(section_t *data)
{
	uint32_t j = 0;
	uint32_t size;
	char *sizec = NULL;

	printf("Contents of section %s:\n", data->name);
	asprintf(&sizec, "%x0", (unsigned int)(strlen(data->content) / 32));
	size = (uint32_t)((strlen(sizec) > 4) ? strlen(sizec) : 4);
	free(sizec);
	for (uint32_t i = 0; i < strlen(data->content); ++i, ++j) {
		if (i % 32 == 0)
			printf(" %0*lx", size, data->addr + i / 2);
		if (i % 8 == 0) {
			write(1, " ", 1);
			j++;
		}
		write(1, &data->content[i], 1);
		i = (i + 1 == strlen(data->content)) ? i + (32 - i % 32) - 1 :
			i;
		print_section_indent(data, i, j);
	}
	free(data->content);
}
