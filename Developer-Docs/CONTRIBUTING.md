# Contributing to snip

Thank you for your interest in contributing to **snip**! We welcome contributions from the community to help make this version control system better.

---

## Code of Conduct

- Be respectful and professional
- Provide constructive feedback
- Help others in the community
- Report issues responsibly

---

## How to Contribute

### Reporting Bugs

Found a bug? Please help us fix it!

1. **Check existing issues** - Make sure the bug hasn't already been reported
2. **Create a new issue** with:
   - Clear title describing the bug
   - Steps to reproduce
   - Expected vs actual behavior
   - Your environment (OS, compiler version, etc.)
   - Code examples if applicable

### Suggesting Features

Have an idea for an improvement?

1. **Check the roadmap** - See [Things that could be better](../README.md#things-that-could-be-better) section
2. **Create an issue** with:
   - Clear description of the feature
   - Use cases and benefits
   - Examples of how it would work
   - Any potential challenges

### Submitting Code

Want to write code? Follow these guidelines:

#### 1. Fork and Clone

```bash
git clone https://github.com/aadityansha06/snip.git
cd snip
```

#### 2. Create a Feature Branch

```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/bug-name
```

#### 3. Follow Code Conventions

#### 4. Submit a Pull Request

---

## Code Conventions

### Naming Conventions

Follow **snake_case** for all identifiers:

```c
// Good
int arg_count = 1;
void check_vsc(void);
void add_file(char command[100], char path[PATH_MAX]);

// Bad - Don't use camelCase or unclear abbreviations
int arggc = 1;
void checkvsc();
void addfile();
```

**Function names:**
- Use descriptive verbs
- Use lowercase with underscores
- Examples: `create_node()`, `write_file()`, `read_log()`, `print_log()`

**Variable names:**
- Use meaningful names
- Use full words, not abbreviations (except for common ones like `fp` for file pointer, `ch` for character)
- Examples: `command`, `filename`, `previous_version`, `current_dir`

**Constants:**
- Use UPPERCASE_WITH_UNDERSCORES
- Example: `PATH_MAX`, `MAX_BUFFER`

---

### Comment Style

#### File Header
```c
// Clear description of what the section does
```

#### Section Headers
Use separators for major sections:
```c
// ============================================================================
// SECTION NAME - Brief description
// ============================================================================
```

#### Function Comments
Add purpose above function declarations:
```c
// Initialize snip version control system in current directory
void initial(char command[100]);

// Check if .snip folder exists by searching upward through parent directories
char *check_vsc(void);
```

#### Inline Comments
```c
// Good - Clear and concise
if (stat(check_path, &s) == 0) {
    return check_path;  // Found .snip folder
}

// Bad - Obvious or unclear
if (stat(check_path, &s) == 0) {
    return check_path;  // checking status
}
```

#### Rules
- Add space after `//`
- Keep comments brief and clear
- Avoid stating the obvious
- Remove debug comments before submitting

---

### Formatting and Spacing

#### Indentation
- Use 2-space indentation (consistent with existing code)
- Use spaces, not tabs

```c
if (condition) {
  // 2-space indent
  do_something();
}
```

#### Braces
- Opening brace on same line
- Closing brace on its own line

```c
if (argc == 1) {
  handle_interactive_mode();
} else if (argc > 1) {
  handle_command_line();
}
```

#### Line Length
- Keep lines under 100 characters when possible
- Split long function calls across multiple lines

```c
// Good
snprintf(check_path, sizeof(check_path), "%s\\.snip", path);

// Also acceptable for long lines
printf("Invalid command format. Use: snip commit -filename.ext -m\"message\"\n");
```

#### Blank Lines
- One blank line between functions
- One blank line between logical sections
- No multiple consecutive blank lines

```c
void function1(void) {
  // code
}

void function2(void) {
  // code
}
```

---

### Function Organization

Order functions by execution flow (first used function at top):

1. **Data structures & typedefs** (top)
2. **Global variables**
3. **Function declarations**
4. **Main function**
5. **Utility functions** (helpers used by many functions)
6. **Feature functions** (organized by feature)
7. **Helper functions** (bottom)

Example from snip.c:
```c
// Top - Data structures
typedef struct node {
  // ...
} node;

// Main function
int main(int argc, char *argv[]) { /* ... */ }

// Utility functions
char *current_time(void) { /* ... */ }

// Feature functions
void add_file(...) { /* ... */ }
void commit_msg(...) { /* ... */ }

// Helper functions
void open_diff_view(...) { /* ... */ }
```

---

### Error Handling

Use clear error messages:

```c
// Good
if (path == NULL) {
  printf("snip is not initiated. Please use: snip init\n");
}

// Bad
if (path == NULL) {
  printf("error\n");
}
```

Guidelines:
- Be specific about what went wrong
- Suggest how to fix it
- Use proper grammar and capitalization
- Include relevant filenames or values when helpful

---

### Variable Declarations

```c
// Good - Declare at the beginning of block
void my_function(void) {
  char filename[100];
  int version = 0;
  char *path;
  
  // Use variables after declaration
  strcpy(filename, "test.c");
  version = 1;
}

// Avoid - Declaring in middle of code
void my_function(void) {
  strcpy(filename, "test.c");
  char filename[100];  // Don't do this
}
```

---

### Memory Management

```c
// Good - Always check for NULL
node *addr = malloc(sizeof(node));
if (addr == NULL) {
  printf("Memory allocation failed\n");
  exit(1);
}

// Always free when done
free(addr);
```

---

### String Handling

```c
// Good - Use safe functions
char buffer[PATH_MAX];
snprintf(buffer, sizeof(buffer), "%s\\%s", path, filename);

// Avoid - Unsafe functions
sprintf(buffer, "%s\\%s", path, filename);  // Can overflow

// Use strncpy for safety
strncpy(filename, token + 1, sizeof(filename) - 1);
filename[sizeof(filename) - 1] = '\0';
```

---

### Platform-Specific Code

Use proper preprocessor directives:

```c
#ifdef _WIN32
  // Windows-specific code
  snprintf(path, sizeof(path), "%s\\.snip", base_path);
#elif __linux__
  // Linux-specific code
  snprintf(path, sizeof(path), "%s/.snip", base_path);
#elif __APPLE__
  // macOS-specific code
#endif
```

---

## Testing Your Changes

Before submitting a PR:

1. **Compile without warnings**
   ```bash
   gcc -Wall -Wextra -o snip snip.c
   ```

2. **Test all commands:**
   - `snip init`
   - `snip add -filename.c`
   - `snip commit -filename.c -m"message"`
   - `snip log -filename.c`
   - `snip write -filename.c v-1`
   - `snip compare -filename.c v-1 and v-2`
   - `snip --help`
   - `snip --version`

3. **Test in both modes:**
   - Interactive mode: just type `snip`
   - Command-line mode: `snip init`

4. **Test on multiple platforms if possible:**
   - Windows
   - Linux
   - macOS

---

## Pull Request Process

1. **Update the code** following conventions above
2. **Test thoroughly** on your system
3. **Write a clear commit message:**
   ```
   Add feature: Brief description of change
   
   - Details about what changed
   - Why it was needed
   - Any relevant issue numbers (#123)
   ```

4. **Create a Pull Request** with:
   - Clear title
   - Description of changes
   - Reference to related issue (if applicable)
   - Steps to test your changes
   - Screenshots/examples if relevant

5. **Respond to feedback** - Be open to suggestions and improvements

---

## Commit Message Guidelines

Use clear, descriptive commit messages:

```bash
# Good
git commit -m "Refactor variable naming to follow snake_case convention"
git commit -m "Fix: improve error message clarity in add_file function"
git commit -m "Feature: add support for multiple file comparison"

# Bad
git commit -m "fix stuff"
git commit -m "changes"
git commit -m "update"
```

Format:
```
<type>: <subject>

<body (optional)>
```

Types:
- `Feature:` - New functionality
- `Fix:` - Bug fix
- `Refactor:` - Code restructuring
- `Docs:` - Documentation changes
- `Style:` - Formatting, naming conventions
- `Test:` - Adding/updating tests

---

## Questions?

- **GitHub Issues** - For feature requests and bug reports
- **Discussions** - For questions and discussions about the project
- **Email** - Contact via GitHub profile

---

## Recognition

All contributors will be recognized in the project. Thank you for helping make snip better!

