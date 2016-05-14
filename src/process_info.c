#include <memwatch.h>
#include <memory_info.h>
#include <common.h>

#define ARR_NUM 40

static void print_items(uint32_t pos, list_navi_t *navi);

void print_process_list(const options_t *options, list_navi_t *navi)
{
    int i;
    clear_screen();
    mvaddstr(1, 1, _("Process List:"));
    attron(A_REVERSE);
    mvprintw(3, 0, " %s%10s%10s%10s%15s", _("PID"), _("OWNER"),
             _("RSS"), _("SWAP"), _("Command"));

    for(i = 49; i < COLS; ++i)
    {
        mvaddch(3, i, ' ');
    }
    attroff(A_REVERSE);

    print_items(4, navi);

    refresh();
}

static void print_items(uint32_t pos, list_navi_t *navi)
{
    uint32_t num;
    int visible_items = LINES - 4;

    const char *items[] = {
        "item1",
        "item2",
        "item3",
        "item4",
        "item5",
        "item6",
        "item7",
        "item8",
        "item9",
        "item10",
        "item11",
        "item12",
        "item13",
        "item14",
        "item15",
        "item16",
        "item17",
        "item18",
        "item19",
        "item20",
        "item21",
        "item22",
        "item23",
        "item24",
        "item25",
        "item26",
        "item27",
        "item28",
        "item29",
        "item30",
        "item31",
        "item32",
        "item33",
        "item34",
        "item35",
        "item36",
        "item37",
        "item38",
        "item39",
        "item40",
        NULL
    };

    const char **itemp = items;

    /* up key pressed at the top of list, goto last element */
    if (navi->flags & NAVI_GO_LAST_FL)
    {
        /* items count greater lines count, shift list */
        if (ARR_NUM > visible_items)
        {
            navi->offset = ARR_NUM - visible_items;
            navi->highlight = visible_items;
        }
        else
        {
            navi->highlight = ARR_NUM;
        }
        navi->flags &= ~NAVI_GO_LAST_FL;
    }

    if (ARR_NUM > visible_items)
    {
        if (navi->offset <= ARR_NUM - visible_items)
        {
            itemp += navi->offset;
        }
        else
        {
            navi->highlight = 1;
            navi->offset = 0;
        }
    }
    else
    {
        if (navi->highlight > ARR_NUM)
        {
            navi->highlight = 1;
        }
        /* this is dirty trick, try to fix it in future. */
        if (navi->offset > 0)
        {
            navi->highlight = 1;
            navi->offset = 0;
        }
    }

    for (num = 1; *itemp; num++, pos++, itemp++)
    {
        if (num == navi->highlight)
        {
            attron(A_REVERSE | COLOR_PAIR(1));
            mvprintw(pos, 0, " %s", *itemp);
            attroff(A_REVERSE | COLOR_PAIR(1));
            continue;
        }

        mvprintw(pos, 0, " %s", *itemp);
    }
}
