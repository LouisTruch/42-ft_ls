#pragma once

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/xattr.h>
#include "../usr/include/sys/acl.h"
#include "../libft/libft.h"

#define ERRNO_RESET 0
// Mandatory flags
#define EXIT_INVALID_OPTION 1
#define NO_OPTION 0x0
#define RESET_BIT(opt, bit) (opt &= ~(bit << 0))

#define OPT_LIST 0x1
#define OPT_IS_LIST(opt) (opt & OPT_LIST)
#define OPT_RECRSV 0x2
#define OPT_IS_RECRSV(opt) (opt & OPT_RECRSV)
#define OPT_HIDDN 0x4
#define OPT_IS_HIDDEN(opt) (opt & OPT_HIDDN)
#define OPT_REVERS 0x8
#define OPT_IS_REVERS(opt) (opt & OPT_REVERS)
#define OPT_COLOR 0x10
#define OPT_IS_COLOR(opt) (opt & OPT_COLOR)
#define OPT_FORCE 0x20
#define OPT_IS_FORCE(opt) (opt & OPT_FORCE)
#define OPT_NO_USERLIST 0x40
#define OPT_IS_NO_USERLIST(opt) (opt & OPT_NO_USERLIST)
#define OPT_ONLYDIR 0x80
#define OPT_IS_ONLYDIR(opt) (opt & OPT_ONLYDIR)
#define OPT_NO_GRPLIST 0x400
#define OPT_IS_NO_GRPLIST(opt) (opt & OPT_NO_GRPLIST)
#define OPT_SHOW_ATTR_ACL 0x800
#define OPT_IS_SHOW_ATTR_ACL(opt) (opt & OPT_SHOW_ATTR_ACL)

#define PRINT_DIR_NAME 0x100
#define DO_PRINT_DIR_NAME(opt) (opt & PRINT_DIR_NAME)
#define NOTDIR 0x200
#define ISNOTDIR(opt) (opt & NOTDIR)

#define LS_SUCCESS 0
#define MALLOC_FAIL 666

#define XATTR_SIZE 10000

#define COLOR_RESET "\x1b[0m"
#define COLOR_EXEC "\x1b[1;32m"
#define COLOR_DIR "\x1b[1;34m"
#define COLOR_LINK "\x1b[1;36m"
#define COLOR_FIFO "\x1b[40;33m"
#define COLOR_SOCK "\x1b[1;35m"
#define COLOR_CHR "\x1b[40;33;1m"
#define COLOR_BROKEN "\x1b[40;31;1m"

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
} e_idx_col;

typedef enum abc
{
    SORT_ALPHABETICAL,
    NO_SORT,
    SORT_LAST_MODIF,
    SORT_LAST_ACCESS,
    SORT_SIZE,
} e_sort_option;

typedef enum
{
    PRINT_LAST_MODIF,
    PRINT_LAST_ACCESS
} e_time_to_print;

typedef u_int32_t t_opt;
typedef u_int32_t t_col_width;

typedef struct
{
    t_opt option;
    e_sort_option sort_by;
    e_time_to_print time_to_print;
    t_col_width col_width[NCOLS];
    blkcnt_t total_blcks_alloc;
} t_print_opt;

#define MAX_WIDTH_DATE 8
#define MAX_WIDTH_INT64 21

typedef struct
{
    char perms[12];
    char color[11];
    char nlink[MAX_WIDTH_INT64];
    char size[MAX_WIDTH_INT64];
    char month[MAX_WIDTH_DATE];
    char day[MAX_WIDTH_DATE];
    char hour[MAX_WIDTH_DATE];
} t_str_file_info;

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
    time_t last_access;
    t_str_file_info str_file_info;
} t_metadata;

typedef struct s_file
{
    t_metadata *metadata;
    struct s_file *next;
    struct s_file *prev;
} t_file;

typedef struct
{
    int i;
    char buff[4096];
} t_printer;

typedef struct
{
    bool valid;
    int line_len;
    int col_arr[256];
} column_info;

int ls(char *argv, t_print_opt *print);

// Parsing
void parse_option(char **argv, t_print_opt *print);
void remove_flags_argv(int *argc, char **argv, t_print_opt *print);

// Linked list functions
t_file *lst_new(const char *file_name, const struct stat *sb, t_print_opt *print);
void lst_addback(t_file **lst, t_file *new);
void lst_clear(t_file **lst);
size_t lst_size(t_file *lst);

// Sort
typedef int (*cmp_func)(const t_metadata *, const t_metadata *);
void sort_lst_file(t_file **head_ref, t_print_opt *print);

// Print
void print_default_format(t_file *file_lst, t_print_opt *print);
void print_list_format(char *argv, t_file *file_lst, t_print_opt *print);

void get_cols_max_width(t_metadata *metadata, t_col_width col_width[NCOLS]);

// Color
void get_color(char *color, mode_t mode);

// Utils
void get_complete_path(char *str1, size_t len, char *str2, char *str3);
int max(int a, int b);
void lltoa_no_alloc(char *str, long long nb);
int mini_sprintf(char *buff, const char *format, ...);
void reverse_str(char *str);