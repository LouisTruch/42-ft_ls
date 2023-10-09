#pragma once

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include "../libft/libft.h"

// Delete before push
#include <stdio.h>

// Mandatory flags
#define NO_OPTION 0x0
#define l_OPTION 0x0001
#define R_OPTION 0x0002
#define a_OPTION 0x0004
#define r_OPTION 0x0008
#define t_OPTION 0x0010

#define is_l(option) (option & l_OPTION)
#define is_R(option) (option & R_OPTION)
#define is_a(option) (option & a_OPTION)
#define is_r(option) (option & r_OPTION)
#define is_t(option) (option & t_OPTION)

// Bonus flags
#define u_OPTION 0x0020
#define f_OPTION 0x0040
#define g_OPTION 0x0080
#define d_OPTION 0x0100

#define INVALID_OPTION 1

typedef u_int32_t option;

typedef enum options
{
    l_option,
    r_option,
    R_option,
    a_option,
    t_option
} options;

typedef enum sort_by
{
    alphabetical,
    chronological
} sort_by;

typedef enum file_type
{
    directory,
} file_type;

typedef struct s_metadata
{
    char *name;
    uid_t owner;
    gid_t group;
    mode_t mode;
    off_t size;
    blkcnt_t blocks;
    time_t last_modif;
} t_metadata;

typedef struct s_file
{
    t_metadata *metadata;
    struct s_file *next;
    struct s_file *prev;
} t_file;

struct s_file *lst_new(const char *file_name);
void lst_addback(t_file **lst, t_file *new);
void lst_print(t_file *lst, bool ascending, bool l_option);
void lst_clear(t_file **lst);
void lst_sort(t_file **head_ref, int sort_by);