#include <sys/stat.h>  // struct stat and stat()
#include <unistd.h>    // sometimes needed for access() or getcwd()
#include<stdio.h>
#include <direct.h>   // Windows only
#include<limits.h> // max limit of the directory it will create for example buffer[MAX_path] it will dynamically count the number of char and maker buffer of that size
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<time.h>


void initial(char command[100]);

void addfile(char command[100],char path[PATH_MAX]);

char* file_exist(char command[100],char path[PATH_MAX]);
char* checkvsc();
void commit_msg(char command[100],char path[PATH_MAX]);
void  writingfile(char path[PATH_MAX],char filename[100],char prev_version[PATH_MAX]); // it will check for the file which user want to commit let say main.c
// it will find it in whole folder googin upward and then read data from it and write it inside the prev-version i.e main-01.c
void  printlog(); // print all previous logs
void  overwrite(char command[100],char path[PATH_MAX]);

void help();
char* current_time(){
     time_t now = time(NULL);
  struct tm *t = localtime(&now);
 static char buffer[100];

  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", t);
  return buffer;

}

typedef struct node{
    char msg[100],time[100];
    int version;  

    struct node *next;

 
}node;



node* createnode(){
    node *addr;
    addr = (node*)malloc(sizeof(node));
    return(addr);
}
node *start= NULL;

void readlog(char log_path[PATH_MAX]);

FILE *fp;
char* trim_whitespace(char* str) {
    char *end;

    // Trim leading space
    while (*str == ' ' || *str == '\t') str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t')) end--;

    *(end + 1) = '\0';

    return str;
}

int main(int argc, char *argv[]) {
    char command[100];
    
    // Check if command-line arguments are provided
    if (argc > 1) {
        // For command-line arguments, we don't need "snip" prefix
        // Just reconstruct the arguments
        strcpy(command, argv[1]);
        
        for (int i = 2; i < argc; i++) {
            strcat(command, " ");
            strcat(command, argv[i]);
        }
        
        // Now parse without expecting "snip" prefix
        char arrg[20];
        int ret = sscanf(command, "%s", arrg);
        
        if (ret == 1) {
            if (strcmp(arrg, "init") == 0) {
                // Create full command for existing functions
                char full_command[100];
                snprintf(full_command, sizeof(full_command), "snip %s", command);
                initial(full_command);
            }
            else if (strcmp(arrg, "add") == 0) {
                char* path = checkvsc();
                if (path == NULL) {
                    printf("snip is not initiated\n");
                } else {
                    char full_command[100];
                    snprintf(full_command, sizeof(full_command), "snip %s", command);
                    addfile(full_command, path);
                }
            }
            else if (strcmp(arrg, "commit") == 0) {
                char* path = checkvsc();
                char full_command[100];
                snprintf(full_command, sizeof(full_command), "snip %s", command);
                char* exist = file_exist(full_command, path);
                if (path == NULL) {
                    printf("snip is not initiated\n");
                } else if (exist == NULL) {
                    printf("snip is not added pls use - snip add\n");
                } else {
                    commit_msg(full_command, exist);
                }
            }
            else if (strcmp(arrg, "log") == 0) {
                char* path = checkvsc();
                char full_command[100];
                snprintf(full_command, sizeof(full_command), "snip %s", command);
                char* exist = file_exist(full_command, path);
                if (path == NULL) {
                    printf("snip is not initiated\n");
                } else if (exist == NULL) {
                    printf("snip file is not added pls use -> snip add -filename or snip --help for help\n");
                } else {
                    readlog(exist);
                    printlog();
                }
            }
            else if (strcmp(arrg, "write") == 0) {
                char* path = checkvsc();
                char full_command[100];
                snprintf(full_command, sizeof(full_command), "snip %s", command);
                char* exist = file_exist(full_command, path);
                if (path == NULL) {
                    printf("snip is not initiated\n");
                    exit(1);
                } else if (exist == NULL) {
                    printf("snip is not added pls use - snip add\n");
                    exit(1);
                } else {
                    readlog(exist);
                    overwrite(full_command, exist);
                }
            }
            else if (strcmp(arrg, "--help") == 0) {
                help();
            }
            else if (strcmp(arrg, "--version") == 0) {
                printf("\nSnip version 1.0 is currently installed\n");
                printf("Check github.com/aadityansha06 for more\n");
            }
            else {
                printf("snip unable to recognise '%s'\n", arrg);
                printf("Use 'snip --help' for available commands\n");
                exit(1);
            }
        }
    } else {
        // Interactive mode - expects "snip" prefix
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';
        
        int ret;
        char cmd[10];
        char arrg[20];
        
        ret = sscanf(command, "%s %s", cmd, arrg);
        
        if (ret == 2 && strcmp(cmd, "snip") == 0) {
            // Your existing logic for interactive mode
            // ... (same as before)
        }
    }
    
    return 0;
}


void initial(char command[100]){
    struct stat s; // checking folder whether vsc(version-system control) exist or not
    if (stat(".snip", &s) == 0 && S_ISDIR(s.st_mode)) {
    printf("snip is already initated");
}else{
    if (_mkdir(".snip")==0)
    {
        printf("snip initiated");
    }
    
}
    
}
char* checkvsc(){
    // A static char variable, whether declared inside or outside a function, has a lifetime that spans the entire duration of the program's execution.
    //code
   char path[PATH_MAX];
    getcwd(path,sizeof(path));
     static char check_path[PATH_MAX];
    while (1) {
    snprintf(check_path, sizeof(check_path), "%s\\.snip", path); // combine path + %s// and put it in check path
    // it wil make a string  path\.snip ex-problems -c\snip\.snip and put into chekc_path

    if (access(check_path, F_OK) == 0) {
        return check_path;  // found .snip
    }

    // If we reach root, stop
    if (strlen(path) <= 3 && path[1] == ':' && path[2] == '\\') {
        break;
    }

    // Move one folder up
    char* last_slash = strrchr(path, '\\');
    if (last_slash != NULL) {
        *last_slash = '\0';
    } else {
        break;
    }
}
  
    return NULL;
   

}



void  addfile(char command[100],char path[PATH_MAX]){

char filename[30];
 // tekne contain hello.c
char file[PATH_MAX];
 char log_path[PATH_MAX];
 char ver[PATH_MAX];
 if (sscanf(command,"snip add -%s ",filename)==1){
        snprintf(file, sizeof(file), "%s\\%s",path,filename); 

 }else{
    printf("\nworng command pls use snip add -filename\n");
    exit(1);

 }

  // '\\' is used to print \ as charcter not as command 
    // so if we want to go like .snip/hello.c in direcotry ; we have to pass .snip(path)//hello.c(token)

    //now we'll create a direcotry with thne name hello.c inside vsc which contain all logs and hsitory of hello.c
    // filevariable conatin .snip/hello.c

 struct stat s; // checking folder whether hello.c(version-system control) exist or not
 
    if (stat(file, &s) == 0 && S_ISDIR(s.st_mode)) {
    printf("snip is already added,Pls enter snip commit to make commit");
}else{
   
      if (_mkdir(file)==0)
    {
        snprintf(ver, sizeof(ver), "%s\\%s\\.version",path,filename);
        _mkdir(ver);
        snprintf(log_path, sizeof(log_path), "%s\\log.txt", file);
        //.snip->hello.c(dirc)->log
        //and 
        // .snip->hello.c(dirc)->version->1,2,....
        
        fp=fopen(log_path,"w");
        if (fp==NULL)
        {
            printf("\n Unable to create logs");
            exit(1);
        }else{

            fclose(fp);
            printf("snip added %s Sucessfully..\n",filename);
            
        }
        
        
    }

}


}


char* file_exist(char command[100],char path[PATH_MAX]){
    char cmd[100];
    char filename[20];

   if (sscanf(command,"snip commit -%s",filename)==1 || sscanf(command,"snip log -%s",filename)==1|| sscanf(command,"snip write -%s",filename)==1||sscanf(command,"snip add -%s",filename)==1){
        //printf("\n%s is filename in file-exist",filename);
 static char file[PATH_MAX];
char log_path[PATH_MAX];
snprintf(file, sizeof(file), "%s\\%s",path,filename); 
struct stat s; // checking folder whether hello.c in .snip(version-system control) exist or not
 
// S_ISDIR check for direcotry 
    if (stat(file, &s) == 0 && S_ISDIR(s.st_mode)) {
        //printf("snip exist %s",file);  debugg
        return file;


}else{
    return NULL;
}
        

 }else{
    printf("\nworng command pls use snip --help to know more\n");
    exit(1);

 }
 

}


void commit_msg(char command[100],char path[PATH_MAX]){

   
    char check_log[PATH_MAX],prev_version[PATH_MAX];

  // to check the which commit it is
    // path already contains the full path to the file directory (e.g., .snip\hello.c)
    // so we just need to add log.txt to it
    char* time = current_time();
    char cmd[100];
    strcpy(cmd,command);

char filename[100];
char firstname[20],lastname[10],msg[100];
if (sscanf(cmd,"snip commit -%s",filename)!=1)// since one stirng is stored so it should return 1
{
    printf("Invalid command format. Use: snip commit -file -m\"message\"\n");
    exit(1);
}

if  (sscanf(filename,"%[^.].%s",firstname,lastname)!=2)
{
    printf("Invalid filename format: %s\n", filename);
     exit(1);
}
char  *messg = strstr(cmd,"-m\"");
if (messg!=NULL)
{
    sscanf(messg,"-m\"%[^\"]",msg);
}else{
   strcpy(msg,"no-commit-msg\n");
}

    struct stat s;
    snprintf(check_log,sizeof(check_log),"%s\\log.txt",path);
    //printf("\nChecking log at: %s\n",check_log);
    //reset statrt
    start=NULL;
    readlog(path);
     int commit_version;
    // Check if it's a regular file, not a directory
         
              if (start == NULL || start->version < 1)
              {
                 commit_version=1;
                
              }else{
                   commit_version=start->version+1;
                
              }
              
      
      

    
    if (stat(check_log, &s) == 0 && S_ISREG(s.st_mode)) {  // S_ISREG check for regular file 
        // printf("log file exists\n"); //debug

      
       fp=fopen(check_log,"a");
       if (fp==NULL)
       {
         printf("\nerror in Log file opening\n");
         exit(1);
       }
       fprintf(fp,"%s\t%s\t%d\n",msg,time,commit_version);
        printf("commited\n");
        fclose(fp);
        
           snprintf(prev_version,sizeof(prev_version),"%s\\.version\\%d",path,commit_version); // prev-version = hello-01.c
           writingfile(path,filename,prev_version);
    }
   
       
    
    else {
        printf("log doesn't exist\n");
    }
}


void  writingfile(char path[PATH_MAX],char filename[100],char prev_version[PATH_MAX]){
      char check_path[PATH_MAX]; // original file path
       char current_dir[PATH_MAX];
      char ch;
      int found=0;
      struct stat s;
      getcwd(current_dir,sizeof(current_dir));
    while (1) {
   snprintf(check_path, sizeof(check_path), "%s\\%s",current_dir, filename);
 // combine path + %s// and put it in check path to check the actuall file ; from where it would be copied
    // it wil make a string  path\.snip ex-problems -c\snip\.snip and put into chekc_path
 
    // check for the main.c file or some other file which user wants to commit
    if (stat(check_path, &s) == 0 && S_ISREG(s.st_mode)){
            FILE *ver,*current;
            // read from current and write in version(ver) of it which will be sotred
            current=fopen(check_path,"r");
            ver = fopen(prev_version,"w");
            if (current==NULL || ver==NULL)
            {
                printf("Error during reading and writing file\n");
                exit(1);
            }
            
            while ((ch=fgetc(current))!=EOF)
            {
                fputc(ch,ver);
            }
            fclose(current);
            fclose(ver);
           
            found=1;
            

    }
    // If we reach root, stop
    if (strlen(path) <= 3 && path[1] == ':' && path[2] == '\\') {
        break;
    }
   


    // Move one folder up
    char* last_slash = strrchr(path, '\\');
    if (last_slash != NULL) {
        *last_slash = '\0';
    } else {
        break;
    }
}
if (found!=1)
{
    printf("\nNO such file named %s found!!\nPls check whether the file and .snip exist in same directory or not",filename);
}else{
     printf("\nSuccessfully inserted..!!");
}

}
  
  
   

void readlog(char path[PATH_MAX]){
    char log_path[PATH_MAX];
   
    snprintf(log_path,sizeof(log_path),"%s\\log.txt",path);

   //exact log  till log.txt
     struct stat s;
   
    if (stat(log_path, &s) == 0 && S_ISREG(s.st_mode)) { 
        fp=fopen(log_path,"r");
    char msg[100],time[100];
    int ver=0;
    char buffer[300];
        node *temp;

        while (fgets(buffer,sizeof(buffer),fp)) 
        {
          if ( sscanf(buffer,"%[^\t]\t%[^\t]\t%d\n",msg,time,&ver)==3)// check all 3 values are read 
          {
              if (start==NULL)
              {
                start=createnode();
                strcpy(start->msg,msg);
                   strcpy(start->time,time);
                   start->version=ver;
                   start->next=NULL;
                   temp=start;
            }else{
                temp=createnode();
                 strcpy(temp->msg,msg);
                   strcpy(temp->time,time);
                   temp->version=ver;
                   temp->next=start;
                   start=temp;

            }
            
            
            }
          }
           
          
        fclose(fp);
    }
    else{
        printf("\nlogs doesn't exist");
        exit(1);
    }


}



void printlog(){
  
   if (start==NULL)
   {
        printf("NO log exist pls make some commit\n");
        exit(0);
   }else{
              printf("\t+-----+-----------------------------------------------------------------------------+\n");
        printf("\t| %-20s | %-25s | %-6s |\n", "Time", "\t\tCommit-msg", "version");
     printf("\t+-----+-----------------------------------------------------------------------------+\n");
              node* temp;
              temp=start;
              while (temp!=NULL)
              {
                     printf("\t| %-20s | %-38s | %-6d |\n",temp->time,temp->msg,temp->version);
                     temp=temp->next;
              }
 printf("\t+-----+-----------------------------------------------------------------------------+\n");
    printf("\nUse snip write \"-filename -version\"..  to overwrite previous version\n");

   }
   

}




void  overwrite(char command[100],char path[PATH_MAX]){

    char ver_path[PATH_MAX]; // to find the path of actuall code where the version is to be overwrite

    //snprintf(ver_path,sizeof(ver_path),"%s//.version",path); // .snip/hello.c/version where contain version files
 

    char filename[50];
     int found =0;
    int ver=0; // version 
     char pre_version[PATH_MAX];
    if (sscanf(command,"snip write -%s -%d",filename,&ver)==2)
    {
      //  printf("%d is ver\n",ver);
       
                 snprintf(pre_version,sizeof(pre_version),"%s//.version//%d",path,ver); // version-1,verison-2
                  struct stat s;
                    
        while (1) {
            snprintf(ver_path, sizeof(ver_path), "%s\\%s", path, filename);
 
 
    // check for the main.c file or some other file which user wants to commit
    if (stat(ver_path, &s) == 0 && S_ISREG(s.st_mode)){
            FILE *pre_ver,*current;
            // read from current and write in version(ver) of it which will be sotred
            current=fopen(ver_path,"r+");// if it exist then only write
            pre_ver = fopen(pre_version,"r");
            if (current==NULL || pre_ver==NULL)
            {
                printf("Error during reading and writing file\n");
                exit(1);
            }
            char ch;
         
            while ((ch=fgetc(pre_ver))!=EOF)
            {
                fputc(ch,current);
            }
            fclose(current);
            fclose(pre_ver);
            printf("\nOver-write Successfully of version-%d!!",ver);
            found=1;
            

    }
    // If we reach root, stop
    if (strlen(path) <= 3 && path[1] == ':' && path[2] == '\\') {
        break;
    }
   


    // Move one folder up
    char* last_slash = strrchr(path, '\\');
    if (last_slash != NULL) {
        *last_slash = '\0';
    } else {

        break;
        
    }
}
    }
    if (found==0)
    {
                printf("File not found %s",filename);
    }
    
    
}

void help(){
    
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