#pragma once

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "../libft/libft.h"

typedef enum options {
    l_option,
    r_option,
    R_option,
    a_option,
    t_option
} options;

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
    t_metadata* metadata;
    struct s_file* next;
    struct s_file* prev;
} t_file;

struct s_file* lst_new(const char *file_name);
void lst_addback(t_file** lst, t_file* new);
void lst_print(t_file* lst, bool ascending, bool l_option);
void lst_clear(t_file** lst);
void lst_sort(t_file** head_ref, int sort_by);