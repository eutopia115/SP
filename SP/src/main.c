#include "Move/move.h"
#include "Struct/tree.h"
#include "Print/print_form.h"

extern Node *Link_Arr[BUFSIZE]; /* Saving Node* variable to link*/
extern int idx;                 /* Link Arr Index */

void menu(int selection);
void sidemenu(int selection);

int main(int argc, char *argv[])
{
    /* scanf variable*/
    int selection;

    /* init program */
    CACHE_FILE = OFF;
    CONFIG_FILE = OFF;

    printf("Select what you want? (1: extension 2: Filename 3: Time Distance): ");
    scanf("%d", &selection);
    menu(selection);

    while(1){
        printf("\n\n\n--------------------------------------------------------\n");
        printf("Additional Select Block!\n");
        printf("Select what you want? (0: QUIT 1: extension 2: Filename 3: Time Distance): ");
        scanf("%d", &selection);
        if(selection == 0)
            break;
        else
            sidemenu(selection);
    }

    return 0;
}

void menu(int selection){
    /* Option Variable */
    char ext[BUFSIZE];
    char name[BUFSIZE];

    /* time distance variable */
    struct tm st;
    struct tm ed;
    int st_time;
    int ed_time;

    /* Thread variable */
    pthread_t first_take;
    
    MultipleArg *multiple_arg;
    void *thr_ret;

    switch (selection)
    {
    /* From home directory and Selecting files according to Extension option */
    case 1:
        printf("Input Extension name: ");
        scanf("%s", ext);

        multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));
        strcpy(multiple_arg->filepath, "/home");
        strcpy(multiple_arg->option, ext);

        if (pthread_create(&first_take, NULL, Selecting_Filename, (void *)multiple_arg) != 0)
        {
            puts("pthread_create() error");
            exit(1);
        }

        if (pthread_join(first_take, &thr_ret) != 0)
        {
            puts("pthread_join() error");
            exit(1);
        }

        Print_Success();
        free(thr_ret);

        break;

    case 2:
        printf("Input Filename: ");
        scanf("%s", name);
        multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));
        strcpy(multiple_arg->filepath, "/home");
        strcpy(multiple_arg->option, name);

        if (pthread_create(&first_take, NULL, Selecting_Filename, (void *)multiple_arg) != 0)
        {
            puts("pthread_create() error");
            exit(1);
        }
        if (pthread_join(first_take, &thr_ret) != 0)
        {
            puts("pthread_join() error");
            exit(1);
        }
        Print_Success();
        free(thr_ret);
        break;

    case 3:
        printf("--------------------\n");
        printf("Start Year : ");
        scanf("%d", &st.tm_year);
        printf("Start Month : ");
        scanf("%d", &st.tm_mon);
        printf("Start Day : ");
        scanf("%d", &st.tm_mday);
        printf("--------------------\n");
        printf("End Year : ");
        scanf("%d", &ed.tm_year);
        printf("End Month : ");
        scanf("%d", &ed.tm_mon);
        printf("End Day : ");
        scanf("%d", &ed.tm_mday);

        st_time = MakeLocalTime_t(st.tm_year, st.tm_mon, st.tm_mday);
        ed_time = MakeLocalTime_t(ed.tm_year, ed.tm_mon, ed.tm_mday);

        multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));

        strcpy(multiple_arg->filepath, "/home");
        multiple_arg->start_time = st_time;
        multiple_arg->end_time = ed_time;

        if (pthread_create(&first_take, NULL, Selecting_time_distance, (void *)multiple_arg) != 0)
        {
            puts("pthread_create() error");
            exit(1);
        }
        if (pthread_join(first_take, &thr_ret) != 0)
        {
            puts("pthread_join() error");
            exit(1);
        }

        Print_Success();
        free(thr_ret);
        break;
    default:
        printf("wrong value..\n");
        printf("Try again!! :-)\n");
        break;
    }

}

void sidemenu(int selection){
    /* Option Variable */
    char ext[BUFSIZE];
    char name[BUFSIZE];
    
    /* time distance variable */
    struct tm st;
    struct tm ed;
    int st_time;
    int ed_time;

    /* Thread variable */
    pthread_t Multiple_take;
    
    MultipleArg *multiple_arg;
    void *thr_ret;
    switch (selection)
    {
    /* From home directory and Selecting files according to Extension option */
    case 1:
        printf("Input Extension name: ");
        scanf("%s", ext);

        multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));
        strcpy(multiple_arg->option, ext);
        if (pthread_create(&Multiple_take, NULL, Loop_Filename, (void *)multiple_arg) != 0)
        {
            puts("pthread_create() error");
            exit(1);
        }

        if (pthread_join(Multiple_take, &thr_ret) != 0)
        {
            puts("pthread_join() error");
            exit(1);
        }

        Print_Success();
        free(thr_ret);

        break;

    case 2:
        printf("Input Filename: ");
        scanf("%s", name);
        multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));
        strcpy(multiple_arg->option, name);

        if (pthread_create(&Multiple_take, NULL, Loop_Filename, (void *)multiple_arg) != 0)
        {
            puts("pthread_create() error");
            exit(1);
        }
        if (pthread_join(Multiple_take, &thr_ret) != 0)
        {
            puts("pthread_join() error");
            exit(1);
        }
        Print_Success();
        free(thr_ret);
        break;

    case 3:
        printf("--------------------\n");
        printf("Start Year : ");
        scanf("%d", &st.tm_year);
        printf("Start Month : ");
        scanf("%d", &st.tm_mon);
        printf("Start Day : ");
        scanf("%d", &st.tm_mday);
        printf("--------------------\n");
        printf("End Year : ");
        scanf("%d", &ed.tm_year);
        printf("End Month : ");
        scanf("%d", &ed.tm_mon);
        printf("End Day : ");
        scanf("%d", &ed.tm_mday);

        st_time = MakeLocalTime_t(st.tm_year, st.tm_mon, st.tm_mday);
        ed_time = MakeLocalTime_t(ed.tm_year, ed.tm_mon, ed.tm_mday);

        multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));

        multiple_arg->start_time = st_time;
        multiple_arg->end_time = ed_time;

        if (pthread_create(&Multiple_take, NULL, Loop_Distance, (void *)multiple_arg) != 0)
        {
            puts("pthread_create() error");
            exit(1);
        }
        if (pthread_join(Multiple_take, &thr_ret) != 0)
        {
            puts("pthread_join() error");
            exit(1);
        }

        Print_Success();
        free(thr_ret);
        break;
    default:
        printf("wrong value..\n");
        printf("Try again!! :-)\n");
        break;
    }

}