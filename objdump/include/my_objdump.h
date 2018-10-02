/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#ifndef MY_OBJDUMP_H
#define MY_OBJDUMP_H

#include <elf.h>
#include <sys/stat.h>
#include <stdbool.h>

#define E64 Elf64_Ehdr*
#define S64 Elf64_Shdr*

#define E32 Elf32_Ehdr*
#define S32 Elf32_Shdr*

typedef struct archive_s {
	char fid[16];
	char ftime[12];
	char owner[6];
	char group[6];
	char fmode[8];
	char fsize[10];
	char endc[2];
} archive_t;

typedef struct section_s {
	char *name;
	char *content;
	size_t addr;
} section_t;

typedef struct elf_s {
	char *filename;
	void *mmap;
	void *shdr;
	void *ehdr;
	void *shstr;
	bool type;
} elf_t;

void print_section(section_t *);
void print_bfd_flags(unsigned int);
void objdp32_format(void *data, char *);
void objdp64_format(void *, char *);
bool isElf(Elf64_Ehdr *);
bool check_archive(elf_t *, bool, struct stat *);
bool print_err(const char *, const char *, bool);
bool section_looper(elf_t *, struct stat *);

#endif
