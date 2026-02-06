# snip - Lightweight Git-like Version Control System

`snip` is a minimal yet powerful command-line version control system written in C. It provides Git-like functionality for tracking file changes, managing versions, and comparing code across different commits - all stored locally in a `.snip` directory.

Ready to use! No compilation needed - just download and run.

---

## What it does

The tool lets you track changes to your files locally without needing a full Git setup. It's got basic version control features like committing changes, viewing history, restoring old versions, and comparing different versions side-by-side. Everything gets stored in a `.snip` folder in your project directory.

It works from any subdirectory in your project, which is pretty convenient. You can run it in two modes - either type `snip` to get an interactive prompt, or just run commands directly from your terminal. Works on Windows, Linux, and macOS.

## Installation

### Quick Setup (Recommended)

Get the latest `snip.exe` from the [Releases page](https://github.com/aadityansha06/snip/releases) and put it somewhere like `C:\tools\snip\`. Add that folder to your PATH so you can run it from anywhere. Restart your terminal and you're good to go.

Check if it works:
```bash
snip --version
```

### Building from source

If you want to compile it yourself:
```bash
gcc -o snip snip.c
```

---

## How to use it

### Interactive mode

Just run `snip` without any arguments and you'll get a command prompt:

```bash
snip
# Welcome to snip
# Enter snip --help for help
# Enter snip --exit to exit
# 
# Enter your command..
```

### Direct commands

Or run commands straight away:
```bash
snip init
snip add -main.c
snip commit -main.c -m"initial commit"
```

---

## Commands

| Command | What it does | Example |
|---------|-------------|---------|
| `init` | Start tracking in current directory | `snip init` |
| `add` | Track a specific file | `snip add -main.c` |
| `commit` | Save current version with a message | `snip commit -main.c -m"fixed bug"` |
| `log` | Show all saved versions | `snip log -main.c` |
| `write` | Restore an old version | `snip write -main.c v-2` |
| `compare` | Compare two versions | `snip compare -main.c v-1 and v-2` |
| `--version` | Check which version you're running | `snip --version` |
| `--help` | Show help info | `snip --help` |
| `--exit` | Exit interactive mode | `snip --exit` |

Note: In interactive mode, you still type the full command including `snip` at the start.

---

## Getting started

### Basic workflow

First, check everything's working:
```bash
snip --version
```

Start tracking your project:
```bash
snip init
```

Add a file you want to track:
```bash
snip add -hello.c
```

Make your first commit:
```bash
snip commit -hello.c -m"initial version"
```

See what you've saved:
```bash
snip log -hello.c
```

The output looks something like:
```
	+-----+-----------------------------------------------------------------------------+
	| Time                 | 		Commit-msg                | version |
	+-----+-----------------------------------------------------------------------------+
	| 05-08-2025 14:30:22 | initial version                        | 1      |
	+-----+-----------------------------------------------------------------------------+

Use snip write "-filename -version"..  to overwrite previous version
```

If you mess something up and want to go back:
```bash
snip write -hello.c v-1
```

---

## Comparing versions

You can compare any two versions in your code editor:
```bash
snip compare -main.c v-1 and v-3
```

The tool will try to open the diff in whatever IDE you have installed. It checks for VS Code, PyCharm, Sublime Text, and on Linux it also tries Meld. If it can't find anything, it'll let you know.

After you're done comparing, press `1` to clean up the temporary files it created.

## How it finds your files

The nice thing about snip is you don't have to be in the exact directory where you initialized it. It searches upward through parent directories to find the `.snip` folder and your source files. So you can run commands from anywhere in your project tree.

## Directory structure

When you run `snip init`, it creates a `.snip` folder with subfolders for each file you track. Inside those are version files and a log.txt with your commit history.

Full disclosure: I used AI to generate the fancy tree diagrams and formatted output examples in this documentation because honestly I don't have time to mess with that UI stuff. The code itself is all me though.

---

## Example workflows

### Tracking multiple files

```bash
snip init
snip add -main.c
snip add -utils.h
snip add -config.txt

snip commit -main.c -m"added main function"
snip commit -utils.h -m"helper functions"

snip log -main.c
```

### Working on something experimental

```bash
# Save your working version first
snip commit -script.py -m"working version before optimization"

# Make your changes to script.py...

# Commit the new version
snip commit -script.py -m"optimized algorithm performance"

# See what changed
snip compare -script.py v-1 and v-2

# Roll back if the optimization broke something
snip write -script.py v-1
```

### Using interactive mode

```bash
snip

Enter your command.. snip init
Enter your command.. snip add -main.c
Enter your command.. snip commit -main.c -m"first commit"
Enter your command.. snip log -main.c
Enter your command.. snip --exit
```

---

## Things to keep in mind

### Command syntax rules

File names need a dash in front: `-main.c`, `-script.py`

Commit messages with spaces need quotes: `-m"your message here"`

Versions use `v-` prefix: `v-1`, `v-2`

When comparing, use `and` between versions: `v-1 and v-2`

### Requirements

Files have to exist somewhere in your current directory or above it. The `.snip` folder needs to be in the same directory tree as your files. File extensions get preserved when versions are stored.

### How versioning works

Versions start at 1 and go up automatically. The most recent version shows up at the top when you run `log`. Each commit makes a complete copy of your file, not just the changes. There's no size limit except your disk space.

---

## Known problems

I built this in about 3 days so there's definitely some rough edges.

### Parsing issues

Don't use tab characters in your commit messages because those are used as delimiters internally. File names with spaces might cause problems. Some special characters in file names don't work properly.

### Platform stuff

The code uses Windows-style path separators internally which might cause issues if you're mixing environments. There are some differences in how arguments get parsed between command line and interactive mode. IDE detection depends on having the programs in your system PATH.

### Performance and memory

It creates complete copies of files, so it's not efficient for really large files. If you have tons of versions, loading the log reads everything into memory at once. Old versions never get cleaned up automatically, they just sit there forever.

---

## Things that could be better

Some ideas for future improvements:

Better argument parsing with something like `getopt`

Support for binary files

Branching and merging

Configuration files

Store diffs instead of complete copies

Automatic cleanup of old versions

Better error handling and validation

Unit tests

Better cross-platform path handling

---

## Contact and support

**Report bugs or suggest features:** [GitHub Issues](https://github.com/aadityansha06/snip/issues)

**View releases and download:** [GitHub Releases](https://github.com/aadityansha06/snip/releases)

**Follow for updates:**
- Twitter: [@aadityansha_06](https://x.com/aadityansha_06)
- Telegram: [@Aadityansha](https://t.me/Aadityansha)

**Project Repository:** [https://github.com/aadityansha06/snip](https://github.com/aadityansha06/snip)

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

You are free to:
- Use this software for any purpose
- Copy, modify, and distribute it
- Include it in proprietary applications

The only requirement is to include the original license notice.

---

## Contributing

Want to contribute? Great! Please check out the [Contributing Guide](Developer-Docs/CONTRIBUTING.md) to learn how to follow our code conventions and contribute to the project.

---

## Credits

Got help from AI tools for the IDE detection logic. Thanks to everyone who gave feedback and suggestions.

---

Pro tip: This works best for small to medium text files where you need quick version tracking without Git's complexity. Perfect for scripts, config files, and small projects.

Performance note: Each commit stores a complete copy of your file, so it's simple but can use a lot of disk space if you have large files or tons of versions.
