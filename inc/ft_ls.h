#pragma once

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "../libft/libft.h"

typedef enum flags {
    l_flag,
    r_flag,
    R_flag,
    a_flag,
    t_flag
} flags;

typedef enum sort_by {
    alphabetical,
    chronological
} sort_by;

typedef enum file_type {
    directory,
} file_type;

typedef struct s_metadata {
    char* name;
    uid_t owner;
    gid_t group;
    mode_t mode;
    off_t size;
    blkcnt_t blocks;
    time_t last_modif;
} t_metadata;

typedef struct s_file {
    // char* name;
    // char* creator;
    // char* owner;
    // int type;
    // time_t last_modif;
    t_metadata* metadata;
    struct s_file* next;
    struct s_file* prev;
} t_file;

struct s_file* lst_new(const char *file_name);
void lst_addback(t_file** lst, t_file* new);
void lst_print(t_file* lst, bool ascending, bool l_flag);
void lst_clear(t_file** lst);
void lst_sort(t_file** head_ref, int sort_by);