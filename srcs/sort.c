#include "../inc/ft_ls.h"

static int cmp_alpha_asc(const t_metadata *file1, const t_metadata *file2)
{
    // Ignore first . in filename by +1ing the pointer
    char *cmp1 = !ft_strncmp(file1->name, ".", 1) ? file1->name + 1 : file1->name;
    char *cmp2 = !ft_strncmp(file2->name, ".", 1) ? file2->name + 1 : file2->name;

    // If one file is ".", compare original file name
    if (!ft_strlen(cmp1) || !ft_strlen(cmp2))
        return ft_strcasecmp(file1->name, file2->name);
    return ft_strcasecmp(cmp1, cmp2);
}

static int cmp_alpha_dsc(const t_metadata *file1, const t_metadata *file2)
{
    return -(cmp_alpha_asc(file1, file2));
}

static int cmp_chrono_last_modif_asc(const t_metadata *file1, const t_metadata *file2)
{
    if (!(file2->last_modif - file1->last_modif))
        return cmp_alpha_asc(file1, file2);
    return file2->last_modif - file1->last_modif;
}

static int cmp_chrono_last_modif_dsc(const t_metadata *file1, const t_metadata *file2)
{
    return -(cmp_chrono_last_modif_asc(file1, file2));
}

static int cmp_chrono_last_access_asc(const t_metadata *file1, const t_metadata *file2)
{
    if (!(file2->last_access - file1->last_access))
        return cmp_alpha_asc(file1, file2);
    return file2->last_access - file1->last_access;
}

static int cmp_chrono_last_access_dsc(const t_metadata *file1, const t_metadata *file2)
{
    return -(cmp_chrono_last_access_asc(file1, file2));
}

static cmp_func choose_sort(e_sort_option sort_by, bool reverse)
{
    switch (sort_by)
    {
    case SORT_ALPHABETICAL:
        return reverse ? cmp_alpha_dsc : cmp_alpha_asc;
        break;
    case SORT_LAST_MODIF:
        return reverse ? cmp_chrono_last_modif_dsc : cmp_chrono_last_modif_asc;
        break;
    case SORT_LAST_ACCESS:
        return reverse ? cmp_chrono_last_access_dsc : cmp_chrono_last_access_asc;
        break;
    default:
        return NULL;
        break;
    }
    return NULL;
}

static void bubble_sort(t_file **head_ref, cmp_func cmp_func)
{
    size_t list_size = lst_size(*head_ref);
    for (size_t i = 0; i < list_size; i++)
    {
        t_file *lst = *head_ref;
        for (size_t j = 0; j < list_size - 1; j++)
        {
            if (cmp_func(lst->metadata, lst->next->metadata) > 0)
            {
                t_metadata *tmp = lst->next->metadata;
                lst->next->metadata = lst->metadata;
                lst->metadata = tmp;
            }
            lst = lst->next;
        }
    }
}

void sort_lst_file(t_file **head_ref, t_print_opt *print)
{
    if (*head_ref == NULL || (*head_ref)->next == NULL)
        return;

    cmp_func cmp_func = choose_sort(print->sort_by, OPT_ISREVERS(print->option));
    if (!cmp_func)
        return;
    bubble_sort(head_ref, cmp_func);
}