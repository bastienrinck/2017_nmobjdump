/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_nm.h"

static bool loop_archive(elf_t *elf, struct stat *s, archive_t *archive,
	char *name
)
{
	bool ret = true;
	bool avail = true;
	void *save = elf->mmap;

	for (unsigned i = 0; s->st_size >
		((void *)archive + atoi(archive->fsize)) - save; archive =
		(void *)archive + atoi(archive->fsize) +
			sizeof(archive_t), i = 0) {
		elf->mmap = (void *)archive + sizeof(archive_t);
		for (; i < 16 && archive->fid[i] != '/'; i++);
		elf->filename = i ? strndup(archive->fid, i) :
			strtok((avail ? name : NULL), "/");
		ret = init_elf(elf, true, s);
		free(i ? elf->filename : NULL);
		avail = !i && avail ? false : avail;
	}
	free(avail ? NULL : name);
	return (ret);
}

bool check_archive(elf_t *elf, bool displayFN, struct stat *s)
{
	archive_t *archive = elf->mmap + atoi((char *)(elf->mmap + 56)) + 68;
	char str[] = {'!', '<', 'a', 'r', 'c', 'h', '>', 0x0A};
	char *name = NULL;

	if (!strncmp((char *)elf->mmap, (const char *)&str, 8)) {
		if (displayFN)
			printf("\n%s:\n", elf->filename);
		if (!isElf((void *)archive + sizeof(archive_t))) {
			name = strndup((void *)archive + sizeof(archive_t),
				(size_t)atoi(archive->fsize));
			archive = (void *)archive + atoi(archive->fsize) +
				sizeof(archive_t);
		}
		loop_archive(elf, s, archive, name);
	} else
		return (init_elf(elf, displayFN, s));
	return (displayFN);
}