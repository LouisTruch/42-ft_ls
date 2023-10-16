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

#define EXIT_INVALID_OPTION 1
// Mandatory flags
#define NO_OPTION 0x0
#define OPT_LIST 0x1
#define OPT_RECRSV 0x2
#define OPT_HIDDN 0x4
#define OPT_REVRS 0x8
#define OPT_SORT_TIME 0x10

#define OPT_ISLIST(opt) (opt & OPT_LIST)
#define OPT_ISRECRSV(opt) (opt & OPT_RECRSV)
#define OPT_ISHIDDN(opt) (opt & OPT_HIDDN)
#define OPT_ISREVRS(opt) (opt & OPT_REVRS)
#define OPT_ISSORT_TIME(opt) (opt & OPT_SORT_TIME)

// Bonus flags
#define u_OPTION 0x20
#define f_OPTION 0x40
#define g_OPTION 0x80
#define d_OPTION 0x100

#define MIN_COLUMN_WIDTH 3 // 2 spaces + 1 character

#define _6MONTHS_IN_SECONDS 15780000
#define HOUR_WIDTH 5
#define YEAR_WIDTH 4
#define NCOLS 8
typedef enum
{
    PERMS,
    NB_LINKS,
    OWNER,
    GROUP,
    SIZE,
    MONTH,
    DAY,
    HOUR
} e_idx_col_info;

typedef u_int32_t opt;

typedef enum
{
    ALPHABETICAL,
    CHRONOLOGICAL
} e_sort_option;

typedef struct s_metadata
{
    char *name;
    nlink_t nlink;
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
void lst_clear(t_file **lst);
size_t lst_size(t_file *lst);

typedef int (*cmp_func)(const t_metadata *, const t_metadata *);
void sort_lst_file(t_file **head_ref, opt option);

void print_default(t_file *file_lst);
void print_list(char *argv, t_file *file_lst);

// Utils
void get_complete_path(char *str1, char *str2, char *str3);
int max(int a, int b);
int mini_sprintf(char *buff, const char *format, ...);

void reverse_str(char *str);