/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#ifndef MY_NM_H
#define MY_NM_H

#include <elf.h>
#include <stdbool.h>
#include <sys/stat.h>

#define E64 Elf64_Ehdr*
#define S64 Elf64_Shdr*

#define E32 Elf32_Ehdr*
#define S32 Elf32_Shdr*


/**
 * mmap:	mmap() ptr
 * ehdr		Elf header ptr
 * shdr:	Section header table
 * shstr:	Section header string table
 * symstr	Symbols string table
 */

typedef struct archive_s {
	char fid[16];
	char ftime[12];
	char owner[6];
	char group[6];
	char fmode[8];
	char fsize[10];
	char endc[2];
} archive_t;

typedef struct sym_s {
	long int value;
	char sym;
	char *name;
	struct sym_s *prev;
	struct sym_s *next;
} sym_t;

typedef struct elf64_s {
	bool type;
	char *filename;
	void *mmap;
	void *symstr;
	void *strtab;
	void *ehdr;
	void *shdr;
	void *shstr;
} elf_t;

sym_t *sort_list(sym_t *);
sym_t *push_back(sym_t *, sym_t *);
bool init_elf(elf_t *, bool, struct stat *);
bool check_archive(elf_t *, bool, struct stat *);
bool print_err(const char *, const char *, bool);
bool get_symtab(const char *, elf_t *, size_t *);
bool get_strtab(const char *, elf_t *);
bool fill_shstr(const char *, elf_t *);
bool check_file_type(elf_t *);
bool isElf(Elf64_Ehdr *);
void display_list(sym_t *list, bool displayFN, const char *filename, bool is64);
void generate_sym_list64(elf_t *, sym_t *, size_t, bool);
void generate_sym_list32(elf_t *, sym_t *, size_t, bool);

#endif
