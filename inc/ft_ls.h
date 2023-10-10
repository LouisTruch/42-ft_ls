#pragma once

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "../libft/libft.h"

// Delete before push
#include <stdio.h>

#define ERRNO_RESET 0

// Mandatory flags
#define NO_OPTION 0x0
#define OPT_LIST 0x1
#define OPT_RECRSV 0x2
#define OPT_HIDDN 0x4
#define OPT_REVRS 0x8
#define OPT_TIME 0x10

#define OPT_ISLIST(opt) (opt & OPT_LIST)
#define OPT_ISRECRSV(opt) (opt & OPT_RECRSV)
#define OPT_ISHIDDN(opt) (opt & OPT_HIDDN)
#define OPT_ISREVRS(opt) (opt & OPT_REVRS)
#define OPT_ISTIME(opt) (opt & OPT_TIME)

// Bonus flags
#define u_OPTION 0x20
#define f_OPTION 0x40
#define g_OPTION 0x80
#define d_OPTION 0x100

#define INVALID_OPTION 1

#define MIN_COLUMN_WIDTH 3

typedef u_int32_t opt;

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

typedef struct
{
    bool valid;
    int line_len;
    int col_arr[256];
} column_info;

void ls(char *argv, opt option);

t_file *lst_new(const char *file_name, struct stat sb);
void lst_addback(t_file **lst, t_file *new);
void lst_print(t_file *lst, bool ascending, bool l_option);
void lst_clear(t_file **lst);
void lst_sort(t_file **head_ref, int sort_by);
size_t lst_size(t_file *lst);

void print_dir(t_file *file_lst);

// Utils
void get_complete_path(char *str1, char *str2, char *str3);