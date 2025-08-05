Concept	Description
getcwd()	Get current working directory
chdir("..")	Move one level up in directory
access(".snip",F_OK)	Check if .snip exists in current directory
0 menas exist,-1 not


| 🔧 Function             | 📝 Purpose                             | 📘 Description                          | ✅ Example Usage                          |
| ----------------------- | -------------------------------------- | --------------------------------------- | ---------------------------------------- |
| `getcwd()`              | Get current working directory          | Stores current path in a buffer         | `getcwd(buffer, PATH_MAX);`              |
| `chdir()`               | Change current directory               | Moves you up/down in the directory tree | `chdir(".."); // go up`                  |
| `access()`              | Check if file/folder exists            | Use `F_OK` for existence check          | `access(".snip", F_OK);`                  |
| `snprintf()`            | Safely build strings (like file paths) | Works like `sprintf()` but safer        | `snprintf(path, size, "%s\\.snip", cwd);` |
| `mkdir()` or `_mkdir()` | Create folder                          | Use `_mkdir()` on Windows               | `_mkdir(".snip");` 0 sucess , -1 fails                       |
| `stat()`                | Check if a directory/file exists       | Reads file status                       | `stat("folder", &s);`                    |


**sscanf()**

  char command[100],cmd[10]; // to detetct snip 
  char arrg[20];
  ret=sscanf(command,"%s %s ",cmd,arrg); // slipt string and stor in diff vairbale
//  ret will ocntain the number of words the sscanf returend
// forinstance snip init ; ret = 2
<hr>

**snprintf()**

char path[100];
snprintf(path, sizeof(path), "%s\\.snip\\index.log", currentDir);
This will combine currentDir + .snip\index.log, like:
C:\Users\admin\project\.snip\index.log
and store it in path[100]

````Example
char path[PATH_MAX] = "C:\\Users\\Admin\\Project";
char full[PATH_MAX];

snprintf(full, sizeof(full), "%s\\.snip", path);
 snprintf(file, sizeof(file), "%s\\%s", path,token);// '\\' is used to print \ as charcter not as command 
    // so if we want to go like .snip/hello.c in direcotry ; we have to pass .snip(path)//hello.c(token)

// full now contains: "C:\\Users\\Admin\\Project\\.snip"
`````
** stat s**
path: name of file/folder

buf: struct that stores the result (like size, type, etc.)

Returns: 0 if file/folder exists, -1 if not

struct stat s;
if (stat(".snip", &s) == 0 && S_ISDIR(s.st_mode)) {
    printf(".snip exists and is a folder");
}
// S_ISDIR check for direcotry
 // S_ISREG check for regular file 

<hr>
<h3>strtok()</h3>
str: string to split (only for first call)

delim: what to split at (e.g., " " or "-")

Returns: pointer to the next token, or NULL if done

✅ Example:
c
Copy
Edit
char input[] = "snip init -hello.c";

char *token = strtok(input, " "); // "snip"
token = strtok(NULL, " ");        // "init"
token = strtok(NULL, "-");        // "hello.c"
<hr>

<h4>strcspn()</h4>
//The strcspn() function searches for the first occurrence in a string of any of the specified characters and returns the length of the string up to that point. 
   //If none of the characters are found then the length of the string is returned.

    command[strcspn(command,"\n")]='\0';