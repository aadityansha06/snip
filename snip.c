#include <sys/stat.h> // struct stat and stat()
#include <unistd.h>   // sometimes needed for access() or getcwd()
#include <stdio.h>
#include <direct.h> // Windows only
#include <limits.h> // max limit of the directory it will create for example buffer[MAX_path] it will dynamically count the number of char and maker buffer of that size
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

void initial(char command[100]);

void addfile(char command[100], char path[PATH_MAX]);

char *file_exist(char command[100], char path[PATH_MAX]);
char *checkvsc();
void commit_msg(char command[100], char path[PATH_MAX]);
void writingfile(char path[PATH_MAX], char filename[100], char prev_version[PATH_MAX]); // it will check for the file which user want to commit let say main.c
// it will find it in whole folder googin upward and then read data from it and write it inside the prev-version i.e main-01.c
void printlog(); // print all previous logs
void overwrite(char command[100], char path[PATH_MAX]);

void help();
char *current_time()
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    static char buffer[100];

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", t);
    return buffer;
}

typedef struct node
{
    char msg[100], time[100];
    int version;

    struct node *next;

} node;

node *createnode()
{
    node *addr;
    addr = (node *)malloc(sizeof(node));
    return (addr);
}
node *start = NULL;

void readlog(char log_path[PATH_MAX]);

FILE *fp;
char *trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        end--;

    *(end + 1) = '\0';

    return str;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Welcome to snip\n Enter your command..");

        char command[100];

        fgets(command, sizeof(command), stdin);
        printf("your command is %s", command);
        // The strcspn() function searches for the first occurrence in a string of any of the specified characters and returns the length of the string up to that point.
        // If none of the characters are found then the length of the string is returned.

        command[strcspn(command, "\n")] = '\0';
        int ret; // whatever sscanf returns
        char cmd_1[100], cmd_2[100];
        char *path;

        char cmd[10]; // to detetct snip
        char arrg[20];

        ret = sscanf(command, "%s %s ", cmd, arrg); // slipt string and stor in diff vairbale
                                                    //  ret will ocntain the number of words the sscanf returend
                                                    // forinstance snip init ; ret = 2
        if (ret == 2 && strcmp(cmd, "snip") == 0)
        {
            if (strcmp(arrg, "init") == 0)
            {
                initial(command);        // snip init // intiate first time to create vsc in that particular folder
                char *path = checkvsc(); // check whether .snip folder exist or not

                if (path != NULL)
                {
                    printf("snip is already initiated\n");
                    exit(1);
                }
            }
            else if (strcmp(arrg, "add") == 0) // snip add -hello.c

            {
                path = checkvsc();
                if (path == NULL)
                {
                    printf("snip is not initiated\n");
                }
                else
                {
                    // printf(".snip found : %s",path);
                    addfile(command, path);
                }
            }
            else if (strcmp(arrg, "commit") == 0) // snip commit -hello.c -m"fixed-bugs" where m stands for mesggae
            {

                char *path = checkvsc();
                char *exist = file_exist(command, path);
                if (path == NULL)
                {
                    printf("snip is not initiated\n");
                }
                else if (exist == NULL)
                {
                    printf("snip is not added pls use  - snip add\n");
                }
                // exsit conatian file path till .snip/hello(folder for all hello.c file)/
                commit_msg(command, exist);
            }
            else if (strcmp(arrg, "log") == 0) // snip log -hello.c
            {

                char *path = checkvsc();
                char *exist = file_exist(command, path); // does hello.c exist or not
                if (path == NULL)
                {
                    printf("snip is not initiated\n");
                }
                else if (exist == NULL)
                {
                    printf("snip file is not added pls use ->  snip add -filename or snip --help for help\n");
                }

                readlog(exist); // exist contain path till folder except log file
                printlog();
            }
            else if (strcmp(arrg, "write") == 0) // snip write -hello.c -1 where 1 stand for version 1
            {

                char *path = checkvsc();
                char *exist = file_exist(command, path); // does hello.c exist or not
                if (path == NULL)
                {
                    printf("snip is not initiated\n");
                    exit(1);
                }
                else if (exist == NULL)
                {
                    printf("snip is not added pls use  - snip add\n");
                    exit(1);
                }

                readlog(exist); // exist contain path till folder except log file

                overwrite(command, exist);
            }
        }
    }
    else if (argc > 1)
    {
        char full_command[512] = "";
        for (int i = 0; i < argc; ++i)
        {
            // Add quotes back only for arguments that contain spaces (optional)
            if (strchr(argv[i], ' ') != NULL)
            {
                strcat(full_command, "\"");
                strcat(full_command, argv[i]);
                strcat(full_command, "\"");
            }
            else
            {
                strcat(full_command, argv[i]);
            }

            if (i < argc - 1)
                strcat(full_command, " ");
        }

        printf("full command through command line argument is : %s\n", full_command);

        // Now reuse the same command parsing logic you use for interactive input
        int ret;
        char cmd[20], arg1[100];
        ret = sscanf(full_command, "%s %s", cmd, arg1);

        if (ret == 2 && strcmp(cmd, "snip") == 0)
        {
            if (strcmp(arg1, "init") == 0)
            {
                initial(full_command);
            }
            else if (strcmp(arg1, "add") == 0)
            {
                char *path = checkvsc();
                if (!path)
                {
                    printf("snip is not initiated\n");
                    return 1;
                }
                addfile(full_command, path);
            }
            else if (strcmp(arg1, "commit") == 0)
            {
                char *path = checkvsc();
                if (!path)
                {
                    printf("snip is not initiated\n");
                    return 1;
                }
                char *exist = file_exist(full_command, path);
                if (!exist)
                {
                    printf("snip is not added, please use - snip add\n");
                    return 1;
                }
                commit_msg(full_command, exist);
            }
            else if (strcmp(arg1, "log") == 0)
            {
                char *path = checkvsc();
                char *exist = file_exist(full_command, path);
                if (!path || !exist)
                {
                    printf("snip not initialized or file not added\n");
                    return 1;
                }
                readlog(exist);
                printlog();
            }
            else if (strcmp(arg1, "write") == 0)
            {
                char *path = checkvsc();
                char *exist = file_exist(full_command, path);
                if (!path || !exist)
                {
                    printf("snip not initialized or file not added\n");
                    return 1;
                }
                readlog(exist);
                overwrite(full_command, exist);
            }
            else
            {
                printf("Unknown snip command: %s\n", arg1);
            }
        }
        else
        {
            printf("Invalid command format.\n");
        }
    }

    return 0;
}
void initial(char command[100])
{
    struct stat s; // checking folder whether vsc(version-system control) exist or not
    if (stat(".snip", &s) == 0 && S_ISDIR(s.st_mode))
    {
        printf("snip is already initated");
    }
    else
    {
        if (_mkdir(".snip") == 0)
        {
            printf("snip initiated");
        }
    }
}
char *checkvsc()
{
    // A static char variable, whether declared inside or outside a function, has a lifetime that spans the entire duration of the program's execution.
    // code
    char path[PATH_MAX];
    getcwd(path, sizeof(path));
    static char check_path[PATH_MAX];
    while (1)
    {
        snprintf(check_path, sizeof(check_path), "%s\\.snip", path); // combine path + %s// and put it in check path
        // it wil make a string  path\.snip ex-problems -c\snip\.snip and put into chekc_path

        if (access(check_path, F_OK) == 0)
        {
            return check_path; // found .snip
        }

        // If we reach root, stop
        if (strlen(path) <= 3 && path[1] == ':' && path[2] == '\\')
        {
            break;
        }

        // Move one folder up
        char *last_slash = strrchr(path, '\\');
        if (last_slash != NULL)
        {
            *last_slash = '\0';
        }
        else
        {
            break;
        }
    }

    return NULL;
}

void addfile(char command[100], char path[PATH_MAX])
{

    char filename[30];
    // tekne contain hello.c
    char file[PATH_MAX];
    char log_path[PATH_MAX];
    char ver[PATH_MAX];
    if (sscanf(command, "snip add -%s ", filename) == 1)
    {
        snprintf(file, sizeof(file), "%s\\%s", path, filename);
    }
    else
    {
        printf("\nworng command pls use snip add -filename\n");
        exit(1);
    }

    // '\\' is used to print \ as charcter not as command
    // so if we want to go like .snip/hello.c in direcotry ; we have to pass .snip(path)//hello.c(token)

    // now we'll create a direcotry with thne name hello.c inside vsc which contain all logs and hsitory of hello.c
    //  filevariable conatin .snip/hello.c

    struct stat s; // checking folder whether hello.c(version-system control) exist or not

    if (stat(file, &s) == 0 && S_ISDIR(s.st_mode))
    {
        printf("snip is already added,Pls enter snip commit to make commit");
    }
    else
    {

        if (_mkdir(file) == 0)
        {
            snprintf(ver, sizeof(ver), "%s\\%s\\.version", path, filename);
            _mkdir(ver);
            snprintf(log_path, sizeof(log_path), "%s\\log.txt", file);
            //.snip->hello.c(dirc)->log
            // and
            // .snip->hello.c(dirc)->version->1,2,....

            fp = fopen(log_path, "w");
            if (fp == NULL)
            {
                printf("\n Unable to create logs");
                exit(1);
            }
            else
            {

                fclose(fp);
                printf("snip added %s Sucessfully..\n", filename);
            }
        }
    }
}

char *file_exist(char command[100], char path[PATH_MAX])
{
    char cmd[100];
    char filename[20];

    if (sscanf(command, "snip commit -%s", filename) == 1 || sscanf(command, "snip log -%s", filename) == 1 || sscanf(command, "snip write -%s", filename) == 1 || sscanf(command, "snip add -%s", filename) == 1)
    {
        // printf("\n%s is filename in file-exist",filename);
        static char file[PATH_MAX];
        char log_path[PATH_MAX];
        snprintf(file, sizeof(file), "%s\\%s", path, filename);
        struct stat s; // checking folder whether hello.c in .snip(version-system control) exist or not

        // S_ISDIR check for direcotry
        if (stat(file, &s) == 0 && S_ISDIR(s.st_mode))
        {
            // printf("snip exist %s",file);  debugg
            return file;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        printf("\nworng command pls use snip --help to know more\n");
        exit(1);
    }
}
void commit_msg(char command[100], char path[PATH_MAX])
{
    char check_log[PATH_MAX], prev_version[PATH_MAX];
    char *time = current_time();
    char cmd[100];
    strcpy(cmd, command);

    char filename[100];
    char msg[100] = "no-commit-msg"; // Default message
    
    // Extract filename first
    if (sscanf(cmd, "snip commit -%s", filename) != 1)
    {
        printf("Invalid command format. Use: snip commit -filename.ext [-m\"message\"]\n");
        exit(1);
    }

    // Clean filename - remove any trailing message parts
    char *space_pos = strchr(filename, ' ');
    if (space_pos != NULL) {
        *space_pos = '\0'; // Cut at first space
    }

    // Validate filename
    if (filename[0] == '-' || strchr(filename, '.') == NULL)
    {
        printf("Invalid filename format: %s\n", filename);
        exit(1);
    }

    // Extract commit message - look for -m" pattern
    char *msg_start = strstr(command, "-m\"");
    if (msg_start != NULL)
    {
        if (sscanf(msg_start, "-m\"%[^\"]", msg) != 1) {
            // If quoted parsing fails, try without quotes
            sscanf(msg_start + 2, "%s", msg); // Skip "-m"
        }
    }
    else
    {
        // Try -m without quotes for command line arguments
        char *msg_flag = strstr(command, "-m");
        if (msg_flag)
        {
            if (*(msg_flag + 2) == '"')
            {
                sscanf(msg_flag, "-m\"%[^\"]", msg);
            }
            else
            {
                sscanf(msg_flag + 2, "%s", msg); // Skip "-m"
            }
        }
    }

    printf("Commit message: %s\n", msg);
    printf("Debug: Extracted filename: '%s'\n", filename);

    struct stat s;
    snprintf(check_log, sizeof(check_log), "%s\\log.txt", path);
    
    // Reset start
    start = NULL;
    readlog(path);
    int commit_version;

    if (start == NULL || start->version < 1)
    {
        commit_version = 1;
    }
    else
    {
        commit_version = start->version + 1;
    }

    if (stat(check_log, &s) == 0 && S_ISREG(s.st_mode))
    {
        fp = fopen(check_log, "a");
        if (fp == NULL)
        {
            printf("\nerror in Log file opening\n");
            exit(1);
        }
        fprintf(fp, "%s\t%s\t%d\n", msg, time, commit_version);
        printf("committed\n");
        fclose(fp);

        snprintf(prev_version, sizeof(prev_version), "%s\\.version\\%d", path, commit_version);
        writingfile(path, filename, prev_version); // Pass filename, not full path
    }
    else
    {
        printf("log doesn't exist\n");
    }
}

void writingfile(char path[PATH_MAX], char filename[100], char prev_version[PATH_MAX])
{
    char check_path[PATH_MAX]; // original file path
    char current_dir[PATH_MAX];
    char ch;
    int found = 0;
    struct stat s;
    getcwd(current_dir, sizeof(current_dir));
    
    while (1)
    {
        snprintf(check_path, sizeof(check_path), "%s\\%s", current_dir, filename);
        
        // check for the file which user wants to commit
        if (stat(check_path, &s) == 0 && S_ISREG(s.st_mode))
        {
            FILE *ver, *current;
            current = fopen(check_path, "r");
            ver = fopen(prev_version, "w");
            if (current == NULL || ver == NULL)
            {
                printf("Error during reading and writing file\n");
                exit(1);
            }

            while ((ch = fgetc(current)) != EOF)
            {
                fputc(ch, ver);
            }
            fclose(current);
            fclose(ver);

            found = 1;
            break; // Important: break after finding the file
        }
        
        // If we reach root, stop
        if (strlen(current_dir) <= 3 && current_dir[1] == ':' && current_dir[2] == '\\')
        {
            break;
        }

        // Move one folder up
        char *last_slash = strrchr(current_dir, '\\');
        if (last_slash != NULL)
        {
            *last_slash = '\0';
        }
        else
        {
            break;
        }
    }
    
    if (found != 1)
    {
        printf("\nNO such file named %s found!!\nPls check whether the file and .snip exist in same directory or not", filename);
    }
    else
    {
        printf("\nSuccessfully inserted..!!");
    }
}

void overwrite(char command[100], char path[PATH_MAX])
{
    char ver_path[PATH_MAX]; // to find the path of actual code where the version is to be overwrite
    char current_dir[PATH_MAX];
    getcwd(current_dir, sizeof(current_dir));

    char filename[50];
    int found = 0;
    int ver = 0; // version
    char pre_version[PATH_MAX];
    
    if (sscanf(command, "snip write -%s -%d", filename, &ver) == 2)
    {
        snprintf(pre_version, sizeof(pre_version), "%s\\.version\\%d", path, ver); // Fixed path separators
        struct stat s;

        while (1)
        {
            snprintf(ver_path, sizeof(ver_path), "%s\\%s", current_dir, filename);

            // check for the file which user wants to overwrite
            if (stat(ver_path, &s) == 0 && S_ISREG(s.st_mode))
            {
                FILE *pre_ver, *current;
                
                // Check if version file exists first
                if (stat(pre_version, &s) != 0 || !S_ISREG(s.st_mode)) {
                    printf("Version %d doesn't exist for file %s\n", ver, filename);
                    return;
                }
                
                current = fopen(ver_path, "r+"); // Open in write mode to overwrite
                pre_ver = fopen(pre_version, "r");
                
                if (current == NULL || pre_ver == NULL)
                {
                    printf("Error during reading and writing file or version %d doesn't exist\n", ver);
                    exit(1);
                }
                
                char ch;
                while ((ch = fgetc(pre_ver)) != EOF)
                {
                    fputc(ch, current);
                }
                fclose(current);
                fclose(pre_ver);
                printf("\nOver-write Successfully of version-%d!!", ver);
                found = 1;
                break; // Important: break after successful overwrite
            }
            
            // If we reach root, stop
            if (strlen(current_dir) <= 3 && current_dir[1] == ':' && current_dir[2] == '\\')
            {
                break;
            }

            // Move one folder up
            char *last_slash = strrchr(current_dir, '\\');
            if (last_slash != NULL)
            {
                *last_slash = '\0';
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        printf("Invalid command format. Use: snip write -filename.ext -version\n");
        return;
    }
    
    if (found == 0)
    {
        printf("File not found: %s\n", filename);
    }
}
void readlog(char path[PATH_MAX])
{
    char log_path[PATH_MAX];

    snprintf(log_path, sizeof(log_path), "%s\\log.txt", path);

    // exact log  till log.txt
    struct stat s;

    if (stat(log_path, &s) == 0 && S_ISREG(s.st_mode))
    {
        fp = fopen(log_path, "r");
        char msg[100], time[100];
        int ver = 0;
        char buffer[300];
        node *temp;

        while (fgets(buffer, sizeof(buffer), fp))
        {
            if (sscanf(buffer, "%[^\t]\t%[^\t]\t%d\n", msg, time, &ver) == 3) // check all 3 values are read
            {
                if (start == NULL)
                {
                    start = createnode();
                    strcpy(start->msg, msg);
                    strcpy(start->time, time);
                    start->version = ver;
                    start->next = NULL;
                    temp = start;
                }
                else
                {
                    temp = createnode();
                    strcpy(temp->msg, msg);
                    strcpy(temp->time, time);
                    temp->version = ver;
                    temp->next = start;
                    start = temp;
                }
            }
        }

        fclose(fp);
    }
    else
    {
        printf("\nlogs doesn't exist");
        exit(1);
    }
}

void printlog()
{

    if (start == NULL)
    {
        printf("NO log exist pls make some commit\n");
        exit(0);
    }
    else
    {
        printf("\t+-----+-----------------------------------------------------------------------------+\n");
        printf("\t| %-20s | %-25s | %-6s |\n", "Time", "\t\tCommit-msg", "version");
        printf("\t+-----+-----------------------------------------------------------------------------+\n");
        node *temp;
        temp = start;
        while (temp != NULL)
        {
            printf("\t| %-20s | %-38s | %-6d |\n", temp->time, temp->msg, temp->version);
            temp = temp->next;
        }
        printf("\t+-----+-----------------------------------------------------------------------------+\n");
        printf("\nUse snip write \"-filename -version\"..  to overwrite previous version\n");
    }
}

void help()
{

    printf("\t+-----+------------------------snip-HELP-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+\n");
    printf("\n\t| %-40s | %-70s\n", "Command", "\t\tDiscription");
    printf("\n\t| %-40s | %-70s\n", "snip init", "\t\tTo initialize snip for that particular folder \n\t\t\t\t\t\t\t\t!!!It will only work with the file inside that folder which contain '.snip' and the subfolder of that folder containing '.snip' ");
    printf("\n\t| %-40s | %-70s\n", "snip add -filename.ext", "\t\tTo add the file for the first time , so that any chnages made to them later will be stored here");
    printf("\n\t| %-40s | %-70s\n", "snip commit -filename.ext -m\"commit-msg\" ", "\t\tTo save the changes made to that file along with the msg that what chnages has made ");
    printf("\n\t| %-40s | %-70s\n", "snip log -filename.ext", "\t\tTo see the all previous commit with timestamp and it's version as 1,2,... !!!'Remember the version shown at top will be the recent version'");
    printf("\n\t| %-40s | %-70s\n", "snip write -filename.ext -version", "\t\tTo overwrite the previous version to your current file");
    printf("\n\t| %-40s | %-70s\n", "snip --version", "\t\tTo check the your  version or to download the latest version, also to check out the repo link for feedback and contributation");

    printf("\t+-----+---------------------------------------------------------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+\n");
}