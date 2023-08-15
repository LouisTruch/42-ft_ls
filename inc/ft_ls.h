#pragma once

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include "../libft/libft.h"

typedef enum flags {
    l_flag,
    r_flag,
    R_flag,
    a_flag,
    t_flag
} flags;

typedef enum file_type {
    directory,
} file_type;

typedef struct t_file {
    char* name;
    char* creator;
    char* owner;
    int type;
    time_t last_modif;
    struct t_file* next;
    struct t_file* prev;
} t_file;

t_file* lst_new(const char *file_name);
void lst_addback(t_file** lst, t_file* new);
void lst_print(t_file* lst);
void lst_clear(t_file** lst);
void lst_sort_ascii(t_file **lst);