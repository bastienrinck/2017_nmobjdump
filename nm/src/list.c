/*
** EPITECH PROJECT, 2021
** PSU_2017_nmobjdump
** File description:
** Created by rectoria
*/

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_nm.h"

sym_t *push_back(sym_t *list, sym_t *nelem)
{
	sym_t *temp = list;

	if (!list) {
		nelem->prev = NULL;
		nelem->next = NULL;
		return (nelem);
	}
	for (; temp->next; temp = temp->next);
	temp->next = nelem;
	nelem->prev = temp;
	nelem->next = NULL;
	return (list);
}

static void swap_elem(sym_t *first, sym_t *second)
{
	sym_t temp;

	temp.value = second->value;
	temp.sym = second->sym;
	temp.name = second->name;
	second->value = first->value;
	first->value = temp.value;
	second->sym = first->sym;
	first->sym = temp.sym;
	second->name = first->name;
	first->name = temp.name;
}

static int my_strcmp(char *str1, char *str2)
{

	for (; str1 && str1[0] && str1[0] == '_'; str1 += 1);
	for (; str2 && str2[0] && str2[0] == '_'; str2 += 1);
	return (strcasecmp(str1, str2));
}

sym_t *sort_list(sym_t *list)
{
	sym_t *temp = list;
	sym_t *iter;

	for (; temp; temp = iter ? list : temp->next) {
		iter = temp->next;
		for (; iter; iter = iter->next) {
			if (my_strcmp(temp->name, iter->name) > 0) {
				swap_elem(temp, iter);
				break;
			}
		}
	}
	return (list);
}

void display_list(sym_t *list, bool displayFN, const char *filename, bool is64)
{
	sym_t *temp = list;

	if (displayFN)
		printf("\n%s:\n", filename);
	for (; temp; temp = temp->next) {
		if (temp->value || (temp->sym != 'U' && temp->sym != 'w'))
			printf(is64 ? "%016lx" : "%08lx", temp->value);
		else
			printf(is64 ? "                " : "        ");
		printf(" %c %s\n", (temp->sym != '?') ? temp->sym : 't',
			temp->name);
	}
	for (temp = list, list = list->next;
		list && temp; temp = list, list = list->next)
		free(temp);
}