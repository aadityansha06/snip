#include <direct.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

// Data structure for storing commit log entries
typedef struct node {
  char msg[100];
  char time[100];
  int version;
  struct node *next;
} node;

// Global variables
int arg_count = 1;
node *start = NULL;
FILE *fp;

// Function declarations (organized by execution order)
int main(int argc, char *argv[]);
void initial(char command[100]);
char *check_vsc(void);
void add_file(char command[100], char path[PATH_MAX]);
char *file_exist(char command[100], char path[PATH_MAX]);
void commit_msg(char command[100], char path[PATH_MAX]);
void write_file(char path[PATH_MAX], char filename[100], char prev_version[PATH_MAX]);
void overwrite(char command[100], char path[PATH_MAX]);
void read_log(char path[PATH_MAX]);
void print_log(void);
void compare(char command[100], char filepath[PATH_MAX]);
void open_diff_view(char *file1, char *file2);
void help(void);
char *current_time(void);
char *trim_whitespace(char *str);
node *create_node(void);
// ============================================================================
// MAIN FUNCTION - Entry point for the program
// ============================================================================
int main(int argc, char *argv[]) {
  int exit_request = 0;

  // Interactive mode: no command line arguments
  if (argc == 1) {
    printf("Welcome to snip\n");
    printf("\nEnter snip --help for help\n");
    printf("Enter snip --exit to exit\n");

    while (exit_request != 1) {
      printf("\nEnter your command..");

      char command[100];
      fgets(command, sizeof(command), stdin);
      command[strcspn(command, "\n")] = '\0';

      int ret;
      char cmd[10];
      char arg[20];

      ret = sscanf(command, "%s %s", cmd, arg);

      if (ret == 2 && strcmp(cmd, "snip") == 0) {
        if (strcmp(arg, "init") == 0) {
          initial(command);
          char *path = check_vsc();

          if (path != NULL) {
            printf("snip is already initiated\n");
            continue;
          }
        } else if (strcmp(arg, "add") == 0) {
          path = check_vsc();
          if (path == NULL) {
            printf("snip is not initiated\n");
          } else {
            add_file(command, path);
          }
        } else if (strcmp(arg, "commit") == 0) {
          char *path = check_vsc();
          char *exist = file_exist(command, path);
          if (path == NULL) {
            printf("snip is not initiated\n");
          } else if (exist == NULL) {
            printf("snip is not added. Please use: snip add\n");
          } else {
            commit_msg(command, exist);
          }
        } else if (strcmp(arg, "log") == 0) {
          char *path = check_vsc();
          char *exist = file_exist(command, path);
          if (path == NULL) {
            printf("snip is not initiated\n");
          } else if (exist == NULL) {
            printf("snip file is not added. Please use: snip add -filename or snip --help for help\n");
          } else {
            read_log(exist);
            print_log();
          }
        } else if (strcmp(arg, "write") == 0) {
          char *path = check_vsc();
          char *exist = file_exist(command, path);
          if (path == NULL) {
            printf("snip is not initiated\n");
            continue;
          } else if (exist == NULL) {
            printf("snip is not added. Please use: snip add\n");
            continue;
          }

          read_log(exist);
          overwrite(command, exist);
        } else if (strcmp(arg, "compare") == 0) {
          char *path = check_vsc();
          char *exist = file_exist(command, path);
          if (path == NULL) {
            printf("snip is not initiated\n");
            continue;
          } else if (exist == NULL) {
            printf("snip is not added. Please use: snip add\n");
            continue;
          }
          compare(command, exist);
        } else if (strcmp(arg, "--version") == 0) {
          printf("snip version 1.01 is currently running on your system\n");
          printf("Check out: https://github.com/aadityansha06/snip for new releases, feedback and contributions\n");
        } else if (strcmp(arg, "--help") == 0) {
          help();
        } else if (strcmp(arg, "--exit") == 0) {
          exit_request = 1;
        } else {
          printf("snip unable to recognize '%s'. Please use snip init to get started or use snip --help\n", arg);
        }
      } else {
        printf("\nUnable to recognize '%s'. Please use snip init to get started or use snip --help\n", command);
      }
    }
  } else if (argc > 1) {
    arg_count = 2;

    // Build command string from arguments, skipping program name
    char command[100];
    strcpy(command, argv[1]);

    for (int i = 2; i < argc; i++) {
      strcat(command, " ");
      strcat(command, argv[i]);
    }

    // Parse without expecting "snip" prefix
    char arg[20];
    int ret = sscanf(command, "%s", arg);

    if (ret == 1) {
      if (strcmp(arg, "--version") == 0) {
        printf("snip version 1.01 is currently running on your system\n");
        printf("Check out: https://github.com/aadityansha06/snip for new releases, feedback and contributions\n");
      } else if (strcmp(arg, "--help") == 0) {
        help();
      } else {
        printf("snip unable to recognize '%s'\n", arg);
      }
    } else {
      printf("\nUnable to recognize '%s'. Please use snip init to get started or use snip --help\n", arg);
    }
  }

  if (exit_request == 1) {
    printf("\nsnip exited successfully\n");
  }

  return 0;
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Get current time in formatted string
char *current_time(void) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  static char buffer[100];

  strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", t);
  return buffer;
}

// Remove leading and trailing whitespace from string
char *trim_whitespace(char *str) {
  char *end;

  // Trim leading space
  while (*str == ' ' || *str == '\t')
    str++;

  if (*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && (*end == ' ' || *end == '\t'))
    end--;

  *(end + 1) = '\0';

  return str;
}

// Create new node for linked list
node *create_node(void) {
  node *addr;
  addr = (node *)malloc(sizeof(node));
  return addr;
}

// ============================================================================
// VERSION CONTROL INITIALIZATION
// ============================================================================

// Initialize snip version control system in current directory
void initial(char command[100]) {
  struct stat s;

  if (stat(".snip", &s) == 0 && S_ISDIR(s.st_mode)) {
    printf("snip is already initiated\n");
  } else {
    if (_mkdir(".snip") == 0) {
      printf("snip initiated\n");
    }
  }
}

// Check if .snip folder exists by searching upward through parent directories
char *check_vsc(void) {
  char path[PATH_MAX];
  getcwd(path, sizeof(path));
  static char check_path[PATH_MAX];

  while (1) {
    snprintf(check_path, sizeof(check_path), "%s\\.snip", path);

    if (access(check_path, F_OK) == 0) {
      return check_path;  // Found .snip
    }

    // If we reach root, stop
    if (strlen(path) <= 3 && path[1] == ':' && path[2] == '\\') {
      break;
    }

    // Move one folder up
    char *last_slash = strrchr(path, '\\');
    if (last_slash != NULL) {
      *last_slash = '\0';
    } else {
      break;
    }
  }

  return NULL;
}

// ============================================================================
// FILE MANAGEMENT
// ============================================================================

// Add a file to version control
void add_file(char command[100], char path[PATH_MAX]) {
  char filename[30];
  char file[PATH_MAX];
  char log_path[PATH_MAX];
  char ver[PATH_MAX];

  if (sscanf(command, "snip add -%s", filename) == 1) {
    snprintf(file, sizeof(file), "%s\\%s", path, filename);
  } else {
    printf("\nWrong command. Please use: snip add -filename\n");
    return;
  }

  struct stat s;

  if (stat(file, &s) == 0 && S_ISDIR(s.st_mode)) {
    printf("snip is already added. Please use: snip commit to make a commit\n");
  } else {
    if (_mkdir(file) == 0) {
      snprintf(ver, sizeof(ver), "%s\\%s\\.version", path, filename);
      _mkdir(ver);
      snprintf(log_path, sizeof(log_path), "%s\\log.txt", file);

      fp = fopen(log_path, "w");
      if (fp == NULL) {
        printf("\nUnable to create logs\n");
        exit(1);
      } else {
        fclose(fp);
        printf("snip added %s successfully\n", filename);
      }
    }
  }
}

// Check if file exists in version control
char *file_exist(char command[100], char path[PATH_MAX]) {
  char filename[20];

  if (sscanf(command, "snip commit -%s", filename) == 1 ||
      sscanf(command, "snip log -%s", filename) == 1 ||
      sscanf(command, "snip write -%s", filename) == 1 ||
      sscanf(command, "snip add -%s", filename) == 1 ||
      sscanf(command, "snip compare -%s", filename) == 1) {

    static char file[PATH_MAX];
    snprintf(file, sizeof(file), "%s\\%s", path, filename);

    struct stat s;
    if (stat(file, &s) == 0 && S_ISDIR(s.st_mode)) {
      return file;
    } else {
      return NULL;
    }
  } else {
    printf("\nWrong command. Please use snip --help to know more\n");
    return NULL;
  }
}

// Write file content to version storage
void write_file(char path[PATH_MAX], char filename[100], char prev_version[PATH_MAX]) {
  char check_path[PATH_MAX];
  char current_dir[PATH_MAX];
  char ch;
  int found = 0;
  struct stat s;

  getcwd(current_dir, sizeof(current_dir));

  while (1) {
    snprintf(check_path, sizeof(check_path), "%s\\%s", current_dir, filename);

    // Check for the file which user wants to commit
    if (stat(check_path, &s) == 0 && S_ISREG(s.st_mode)) {
      FILE *ver, *current;
      current = fopen(check_path, "r");
      ver = fopen(prev_version, "w");

      if (current == NULL || ver == NULL) {
        printf("Error during reading and writing file\n");
        exit(1);
      }

      while ((ch = fgetc(current)) != EOF) {
        fputc(ch, ver);
      }

      fclose(current);
      fclose(ver);

      found = 1;
      break;
    }

    // If we reach root, stop
    if (strlen(current_dir) <= 3 && current_dir[1] == ':' && current_dir[2] == '\\') {
      break;
    }

    // Move one folder up
    char *last_slash = strrchr(current_dir, '\\');
    if (last_slash != NULL) {
      *last_slash = '\0';
    } else {
      break;
    }
  }

  if (found != 1) {
    printf("\nNo such file named %s found!!\nPlease check whether the file and .snip exist in the same directory\n", filename);
  } else {
    printf("\nSuccessfully inserted!\n");
  }
}

// ============================================================================
// COMMIT AND VERSION MANAGEMENT
// ============================================================================

// Handle commit message and version creation
void commit_msg(char command[100], char path[PATH_MAX]) {
  char check_log[PATH_MAX], prev_version[PATH_MAX];
  char *time_str = current_time();
  char cmd[100];
  strcpy(cmd, command);

  char filename[100];
  char msg[100] = "no-commit-msg";  // Default message

  // Extract filename first
  if (arg_count == 1) {
    if (sscanf(cmd, "snip commit -%s", filename) != 1) {
      printf("Invalid command format. Use: snip commit -filename.ext [-m\"message\"]\n");
      return;
    }

    // Clean filename - remove any trailing message parts
    char *space_pos = strchr(filename, ' ');
    if (space_pos != NULL) {
      *space_pos = '\0';
    }

    // Extract commit message - look for -m" pattern
    char *msg_start = strstr(command, "-m\"");
    if (msg_start != NULL) {
      if (sscanf(msg_start, "-m\"%[^\"]", msg) != 1) {
        sscanf(msg_start + 2, "%s", msg);
      }
    } else {
      char *msg_flag = strstr(command, "-m");
      if (msg_flag) {
        if (*(msg_flag + 2) == '"') {
          sscanf(msg_flag, "-m\"%[^\"]", msg);
        } else {
          sscanf(msg_flag + 2, "%s", msg);
        }
      }
    }
  } else {
    // Command line argument mode
    char temp_command[100];
    strcpy(temp_command, command);

    // Use strtok to parse tokens
    char *token = strtok(temp_command, " ");
    token = strtok(NULL, " ");  // skip "snip"
    token = strtok(NULL, " ");  // skip "commit"

    // Get filename (should start with -)
    if (token != NULL && token[0] == '-') {
      strcpy(filename, token + 1);  // Remove the '-' prefix
    } else {
      printf("Invalid command format. Use: snip commit -filename.ext -m\"message\"\n");
      return;
    }

    // Look for -m flag
    token = strtok(NULL, " ");
    if (token != NULL && strcmp(token, "-m") == 0) {
      token = strtok(NULL, "\"");
      if (token != NULL) {
        strcpy(msg, token);
        // Remove trailing quote if present
        int len = strlen(msg);
        if (len > 0 && msg[len - 1] == '"')
          msg[len - 1] = '\0';
      }
    }
  }

  struct stat s;
  snprintf(check_log, sizeof(check_log), "%s\\log.txt", path);

  // Reset start
  start = NULL;
  read_log(path);
  int commit_version;

  if (start == NULL || start->version < 1) {
    commit_version = 1;
  } else {
    commit_version = start->version + 1;
  }

  if (stat(check_log, &s) == 0 && S_ISREG(s.st_mode)) {
    fp = fopen(check_log, "a");
    if (fp == NULL) {
      printf("\nError in log file opening\n");
      exit(1);
    }

    fprintf(fp, "%s\t%s\t%d\n", msg, time_str, commit_version);
    printf("committed\n");
    fclose(fp);

    snprintf(prev_version, sizeof(prev_version), "%s\\.version\\%d", path, commit_version);
    write_file(path, filename, prev_version);
  } else {
    printf("log doesn't exist. Please make sure your file and .snip are inside the same directory\n");
  }
}

// Overwrite file with previous version
void overwrite(char command[100], char path[PATH_MAX]) {
  char ver_path[PATH_MAX];
  char current_dir[PATH_MAX];
  getcwd(current_dir, sizeof(current_dir));

  char filename[50];
  int found = 0;
  int version = 0;
  char previous_version[PATH_MAX];

  if (sscanf(command, "snip write -%s v-%d", filename, &version) == 2) {
    snprintf(previous_version, sizeof(previous_version), "%s\\.version\\%d", path, version);
    struct stat s;

    while (1) {
      snprintf(ver_path, sizeof(ver_path), "%s\\%s", current_dir, filename);

      // Check for the file which user wants to overwrite
      if (stat(ver_path, &s) == 0 && S_ISREG(s.st_mode)) {
        FILE *pre_ver, *current;

        // Check if version file exists first
        if (stat(previous_version, &s) != 0 || !S_ISREG(s.st_mode)) {
          printf("Version %d doesn't exist for file %s\n", version, filename);
          return;
        }

        current = fopen(ver_path, "r+");
        pre_ver = fopen(previous_version, "r");

        if (current == NULL || pre_ver == NULL) {
          printf("Error during reading and writing file or version %d doesn't exist\n", version);
          exit(1);
        }

        char ch;
        while ((ch = fgetc(pre_ver)) != EOF) {
          fputc(ch, current);
        }

        fclose(current);
        fclose(pre_ver);
        printf("\nOverwrite successful for version %d!\n", version);
        found = 1;
        break;
      }

      // If we reach root, stop
      if (strlen(current_dir) <= 3 && current_dir[1] == ':' && current_dir[2] == '\\') {
        break;
      }

      // Move one folder up
      char *last_slash = strrchr(current_dir, '\\');
      if (last_slash != NULL) {
        *last_slash = '\0';
      } else {
        break;
      }
    }
  } else {
    printf("Invalid command format. Use: snip write -filename.ext version-num (e.g v-3 for version 3)\n");
    return;
  }

  if (found == 0) {
    printf("File not found: %s\nPlease make sure your filename is correct or your file and .snip exist in the same folder\n", filename);
  }
}

// ============================================================================
// LOG MANAGEMENT
// ============================================================================

// Read log file and populate linked list
void read_log(char path[PATH_MAX]) {
  char log_path[PATH_MAX];
  snprintf(log_path, sizeof(log_path), "%s\\log.txt", path);

  struct stat s;

  if (stat(log_path, &s) == 0 && S_ISREG(s.st_mode)) {
    fp = fopen(log_path, "r");
    char msg[100], time[100];
    int ver = 0;
    char buffer[300];
    node *temp;

    while (fgets(buffer, sizeof(buffer), fp)) {
      if (sscanf(buffer, "%[^\t]\t%[^\t]\t%d\n", msg, time, &ver) == 3) {
        if (start == NULL) {
          start = create_node();
          strcpy(start->msg, msg);
          strcpy(start->time, time);
          start->version = ver;
          start->next = NULL;
          temp = start;
        } else {
          temp = create_node();
          strcpy(temp->msg, msg);
          strcpy(temp->time, time);
          temp->version = ver;
          temp->next = start;
          start = temp;
        }
      }
    }

    fclose(fp);
  } else {
    printf("\nLogs don't exist\n");
    exit(1);
  }
}

// Print all log entries in formatted table
void print_log(void) {
  if (start == NULL) {
    printf("No log exists. Please make some commit\n");
    exit(0);
  } else {
    printf("\t+-----+-----------------------------------"
           "---------------------+\n");
    printf("\t| %-20s | %-25s | %-6s |\n", "Time", "Commit-msg", "version");
    printf("\t+-----+-----------------------------------"
           "---------------------+\n");

    node *temp;
    temp = start;

    while (temp != NULL) {
      printf("\t| %-20s | %-38s | %-6d |\n", temp->time, temp->msg, temp->version);
      temp = temp->next;
    }

    printf("\t+-----+-----------------------------------"
           "---------------------+\n");
    printf("\nUse snip write \"-filename -version\" to overwrite previous version\n");
  }
}

// ============================================================================
// COMPARISON AND DIFF
// ============================================================================

// Detect OS and launch appropriate IDE for diff view
// (Uses platform-specific code to detect available IDEs)
void open_diff_view(char *file1, char *file2) {
  char command[PATH_MAX * 3] = "";

#ifdef _WIN32
  if (system("where code >nul 2>&1") == 0) {
    snprintf(command, sizeof(command), "code --diff \"%s\" \"%s\"", file1, file2);
  } else if (system("where pycharm64.exe >nul 2>&1") == 0) {
    snprintf(command, sizeof(command), "pycharm64 \"%s\" \"%s\"", file1, file2);
  } else if (system("where sublime_text >nul 2>&1") == 0) {
    snprintf(command, sizeof(command), "subl \"%s\" \"%s\"", file1, file2);
  } else {
    MessageBox(NULL, "No supported IDE found. Please open the files manually.",
               "Snip Compare", MB_OK);
    return;
  }

#elif __linux__
  if (system("which code > /dev/null 2>&1") == 0) {
    snprintf(command, sizeof(command), "code --diff \"%s\" \"%s\"", file1, file2);
  } else if (system("which pycharm > /dev/null 2>&1") == 0) {
    snprintf(command, sizeof(command), "pycharm \"%s\" \"%s\" &", file1, file2);
  } else if (system("which subl > /dev/null 2>&1") == 0) {
    snprintf(command, sizeof(command), "subl \"%s\" \"%s\" &", file1, file2);
  } else if (system("which meld > /dev/null 2>&1") == 0) {
    snprintf(command, sizeof(command), "meld \"%s\" \"%s\" &", file1, file2);
  } else {
    printf("No supported editor found. Please open \"%s\" and \"%s\" manually.\n", file1, file2);
    return;
  }

#elif __APPLE__
  if (system("which code > /dev/null 2>&1") == 0) {
    snprintf(command, sizeof(command), "code --diff \"%s\" \"%s\"", file1, file2);
  } else if (system("which pycharm > /dev/null 2>&1") == 0) {
    snprintf(command, sizeof(command), "pycharm \"%s\" \"%s\" &", file1, file2);
  } else if (system("which subl > /dev/null 2>&1") == 0) {
    snprintf(command, sizeof(command), "subl \"%s\" \"%s\" &", file1, file2);
  } else {
    printf("No supported editor found. Please open \"%s\" and \"%s\" manually.\n", file1, file2);
    return;
  }
#endif

  // Finally, launch the command if set
  if (strlen(command) > 0) {
    system(command);
  }
}

// Compare two versions of a file
void compare(char command[100], char filepath[PATH_MAX]) {
  char filename[30], ver_path1[PATH_MAX], ver_path2[PATH_MAX];
  int ver1, ver2;

  if (sscanf(command, "snip compare -%s v-%d and v-%d", filename, &ver1, &ver2) == 3) {
    char current_dir[PATH_MAX];
    snprintf(ver_path1, sizeof(ver_path1), "%s\\.version\\%d", filepath, ver1);
    snprintf(ver_path2, sizeof(ver_path2), "%s\\.version\\%d", filepath, ver2);

    struct stat s1, s2;

    if (stat(ver_path1, &s1) == 0 && S_ISREG(s1.st_mode) &&
        stat(ver_path2, &s2) == 0 && S_ISREG(s2.st_mode)) {

      getcwd(current_dir, sizeof(current_dir));
      char temp_path1[PATH_MAX], temp_path2[PATH_MAX];
      snprintf(temp_path1, sizeof(temp_path1), "%s\\%d-%s", current_dir, ver1, filename);
      snprintf(temp_path2, sizeof(temp_path2), "%s\\%d-%s", current_dir, ver2, filename);

      FILE *v1, *v2, *temp1, *temp2;
      v1 = fopen(ver_path1, "r");
      v2 = fopen(ver_path2, "r");
      temp1 = fopen(temp_path1, "w");
      temp2 = fopen(temp_path2, "w");

      if (v1 == NULL || v2 == NULL || temp1 == NULL || temp2 == NULL) {
        printf("Error opening file during comparison\n");
        exit(1);
      }

      char ch;

      // Copy version 1 content
      while ((ch = fgetc(v1)) != EOF) {
        fputc(ch, temp1);
      }

      // Copy version 2 content
      while ((ch = fgetc(v2)) != EOF) {
        fputc(ch, temp2);
      }

      fclose(v1);
      fclose(v2);
      fclose(temp1);
      fclose(temp2);

      open_diff_view(temp_path1, temp_path2);

      printf("\nAfter comparing press 1 to exit and clean up temporary files: ");
      int del = 0;

      while (del != 1) {
        scanf("%d", &del);
        if (del == 1) {
          // Delete the temporary files
          if (remove(temp_path1) == 0) {
            printf("");
          } else {
            printf("Error deleting %s\n", temp_path1);
          }

          if (remove(temp_path2) == 0) {
            printf("Comparison closed successfully. Now you can remove the file\n");
          } else {
            printf("Error deleting %s\n", temp_path2);
          }
        } else {
          printf("Please press 1 to exit: ");
        }
      }
    }
  } else {
    printf("\nInvalid format. Use: snip compare -filename.ext v-X and v-Y\n");
    printf("Unable to find one of the versions. Check with 'snip log -filename.ext'\n");
    exit(1);
  }
}

// ============================================================================
// HELP AND INFORMATION
// ============================================================================

// Display help information
void help(void) {
  printf("\t+-----+-----------------------------------------------------------"
         "---------------------------------------------+\n");
  printf("\n\t| %-40s | %-70s\n", "Command", "Description");
  printf("\n\t| %-40s | %-70s\n", "snip",
         "Start with command line argument directly");
  printf("\n\t| %-40s | %-70s\n", "snip init",
         "Initialize snip for the current folder");
  printf("\n\t| %-40s | %-70s\n", "snip add -filename.ext",
         "Add a file for version tracking");
  printf("\n\t| %-40s | %-70s\n", "snip commit -filename.ext -m\"msg\"",
         "Save changes with a message");
  printf("\n\t| %-40s | %-70s\n", "snip log -filename.ext",
         "Show all previous commits");
  printf("\n\t| %-40s | %-70s\n", "snip write -filename.ext v-2",
         "Restore a previous version");
  printf("\n\t| %-40s | %-70s\n", "snip compare -filename.ext v-2 and v-3",
         "Compare two versions side by side");
  printf("\n\t| %-40s | %-70s\n", "snip --version",
         "Check version information");
  printf("\t+-----+-----------------------------------------------------------"
         "---------------------------------------------+\n");
}
