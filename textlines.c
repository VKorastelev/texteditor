#include "definitions.h"
#include "textlines.h"


#include <errno.h>
#include <stdlib.h>
#include <string.h>


static struct textline *add_textline(
        char const *buffer_ptr,
        ssize_t buffer_size);


int loading_textlines(
        struct textedit_session *const t_s,
        FILE *const ptf)
{
    int ret = 0;

    char *buffer_ptr = NULL;
    size_t buffer_size = 0;
    ssize_t nread = -1;

    if (NULL == t_s || NULL == ptf)
    {
        ret = -1;
        goto finally;
    }

    free_textlines(t_s);
    memset(t_s->textline, 0, sizeof(t_s->textline));
    t_s->num_textlines = 0;

    do
    {
        errno = 0;
        nread = getline(&buffer_ptr, &buffer_size, ptf);
        if (0 != errno)
        {
            perror("Error in getline(...)");
            ret = -1;
            break;
        } 
        else if (-1 == nread)
        {
            puts("Read EOF!");
            break;
        }

        while (nread > 0
                && (buffer_ptr[nread - 1] == '\n' || buffer_ptr[nread - 1] == '\r'))
        {
            nread--;
        }

        buffer_ptr[nread] = '\0';

        t_s->textline[t_s->num_textlines] = add_textline(
                buffer_ptr,
                nread + 1);

        if (NULL == t_s->textline[t_s->num_textlines])
        {
            fprintf(stderr, "Error in textline_add(...)!\n");
            ret = -1;
            break;
        }

        t_s->num_textlines++;

    } while (t_s->num_textlines < sizeof(t_s->textline) / sizeof(*t_s->textline));

    if (NULL != buffer_ptr)
    {
        free(buffer_ptr);
        buffer_ptr = NULL;
    }

    if (-1 == ret)
    {
        free_textlines(t_s);
    }

 finally:

    return ret;
}


void free_textlines(struct textedit_session *const t_s)
{
    int num_lines = t_s->num_textlines;

    for (int i = 0; i < num_lines; i++)
    {
        if (NULL != t_s->textline[i])
        {
            if (NULL != t_s->textline[i]->text)
            {
                free(t_s->textline[i]->text);
            }
            free(t_s->textline[i]);
            t_s->textline[i] = NULL;
            t_s->num_textlines--;
        }
    }
}


static struct textline *add_textline(
        char const *const buffer_ptr,
        ssize_t const buffer_size)
{
    struct textline *ptl = NULL;

    ptl = malloc(sizeof(struct textline));
    if (NULL == ptl)
    {
        fprintf(stderr, "Error in malloc(...) for struct textline. Out of memory!\n");
        goto finally;
    }

    ptl->text = malloc(buffer_size);
    if (NULL == ptl->text)
    {
        free(ptl);
        ptl = NULL;

        fprintf(stderr, "Error in malloc(...) for text in textline. Out of memory!\n");
        goto finally;
    }

    memcpy(ptl->text, buffer_ptr, buffer_size);

    ptl->len = buffer_size - 1;

finally:

    return ptl;
}
