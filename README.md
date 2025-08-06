# 🚀 snip - Lightweight Git-like Version Control System

`snip` is a minimal yet powerful command-line version control system written in C. It provides Git-like functionality for tracking file changes, managing versions, and comparing code across different commits - all stored locally in a `.snip` directory.

**🎯 Ready to use! No compilation needed - just download and run!**

---

## ✨ Features

- 🏗️ **Initialize** local repositories with `.snip` tracking
- 📁 **Add** files to version control tracking  
- 💾 **Commit** changes with timestamped messages
- 📖 **View logs** with commit history and versions
- 🔄 **Restore** any previous version of your files
- 🔍 **Compare** different versions side-by-side in your IDE
- 🌐 **Cross-platform** support (Windows, Linux, macOS)
- 🎯 **Smart file discovery** - works from any subdirectory
- 🖥️ **Interactive mode** - run `snip` without arguments for a command prompt
- 📦 **Command line mode** - run commands directly as arguments

---

## 🛠️ Installation

### Quick Installation (✅Recommended)
1. **Download** the latest `snip.exe` from [Releases](https://github.com/aadityansha06/snip/releases)
2. **Place** `snip.exe` in a dedicated folder (e.g., `C:\tools\snip\`)
3. **Add** the folder to your system's PATH environment variable
4. **Restart** your terminal
5. **Verify** installation: `snip --version`
6. **Get started**: `snip`

### Build from Source (Optional)
```bash
gcc -o snip snip.c
```

---

## 🎮 Usage Modes

### Interactive Mode
 
step 1:
Run `snip` without arguments to enter interactive mode:
```bash
snip
# Welcome to snip
# Enter snip --help for help
# Enter snip --exit to exit
# 
# Enter your command..
```

### Command Line Mode  
step 2: Run commands directly as arguments:
```bash
snip init
snip add -main.c
snip commit -main.c -m"initial commit"
```

---


| Command | Description | Interactive Mode | Command Line Mode |
|---------|-------------|------------------|-------------------|
| `init` | Initialize version control | `snip init` | `snip init` |
| `add` | Add file to tracking | `snip add -main.c` | `snip add -main.c` |
| `commit` | Commit file changes | `snip commit -main.c -m"message"` | `snip commit -main.c -m"message"` |
| `log` | Show commit history | `snip log -main.c` | `snip log -main.c` |
| `write` | Restore specific version | `snip write -main.c v-2` | `snip write -main.c v-2` |
| `compare` | Compare two versions | `snip compare -main.c v-1 and v-2` | `snip compare -main.c v-1 and v-2` |
| `--version` | Show version info | `snip --version` | `snip --version` |
| `--help` | Show help | `snip --help` | `snip --help` |
| `--exit` | Exit interactive mode | `snip --exit` | N/A |

> **Note**: In interactive mode, you still need to type the full command including `snip` prefix (e.g., `snip init`, `snip commit`, etc.)

---

## 🚦 Getting Started

### Installation Check
```bash
snip --version
# Output: snip version 1.01 is currently running on your system 
#         Check out : https://github.com/aadityansha06/snip for any new releases, feedback and contribution
```

### Start Interactive Mode
```bash
snip
# Welcome to snip
# Enter snip --help for help
# Enter snip --exit to exit
# 
# Enter your command..
```

### Your First Repository

### 1. Initialize Repository
```bash
snip init
# Output: snip initiated
```

### 2. Add File to Tracking
```bash
snip add -hello.c
# Output: snip added hello.c Successfully..
```
> 


### 3. Make Your First Commit
```bash
snip commit -hello.c -m"initial version"
# Output: Commit message: initial version
#         committed
#         Successfully inserted..!!

---
**Note**: First you must have hello.c as file inside the directory or in the sub-directory in which you are initializing snip
for more check out 📁 Directory Structure below
---
```
### 4. View Commit History
```bash
snip log -hello.c
```
```
	+-----+-----------------------------------------------------------------------------+
	| Time                 | 		Commit-msg                | version |
	+-----+-----------------------------------------------------------------------------+
	| 05-08-2025 14:30:22 | initial version                        | 1      |
	+-----+-----------------------------------------------------------------------------+

Use snip write "-filename -version"..  to overwrite previous version
```

### 5. Restore Previous Version
```bash
snip write -hello.c v-1
# Output: Over-write Successfully of version-1!!
```

---

## 🔍 Advanced Features

### Version Comparison
Compare any two versions of your file in your preferred IDE:
```bash
snip compare -main.c v-1 and v-3
```

**Supported IDEs by Platform:**

**Windows:**
- Visual Studio Code (`code --diff`)
- PyCharm (`pycharm64`)
- Sublime Text (`subl`)
- Falls back to MessageBox if no IDE found

**Linux:**
- Visual Studio Code (`code --diff`)
- PyCharm (`pycharm`)
- Sublime Text (`subl`)
- Meld (`meld`)

**macOS:**
- Visual Studio Code (`code --diff`)
- PyCharm (`pycharm`)
- Sublime Text (`subl`)

The tool automatically detects your installed IDE and opens a side-by-side diff view. After comparison, press `1` to clean up temporary files.

### Smart File Discovery
`snip` automatically searches upward through directories to find:
- The `.snip` repository folder (looks for `.snip` in current and parent directories)
- Source files you want to commit (searches upward from current directory)

This means you can run `snip` commands from any subdirectory within your project.

### Automatic Timestamping
Every commit is automatically timestamped with the format: `DD-MM-YYYY HH:MM:SS`

---

## 📁 Directory Structure

When you run `snip init`, it creates:
```
your-project/
├── .snip/
│   └── filename.ext/           # Folder for each tracked file
│       ├── log.txt            # Commit history with messages and timestamps
│       └── .version/          # Version storage directory
│           ├── 1              # Version 1 of file
│           ├── 2              # Version 2 of file
│           └── ...
└── your-files...
```

### Log File Format
Each line in `log.txt` contains:
```
commit-message	DD-MM-YYYY HH:MM:SS	version-number
```

---

## 💡 Usage Examples

### Basic Workflow
```bash
# Start tracking a new project
snip init

# Add multiple files
snip add -main.c
snip add -utils.h
snip add -config.txt

# Make commits with descriptive messages
snip commit -main.c -m"added main function"
snip commit -utils.h -m"helper functions"

# View what you've done
snip log -main.c
```

### Working with Versions
```bash
# Save current state
snip commit -script.py -m"working version before optimization"

# Make changes to script.py...

# Commit optimized version  
snip commit -script.py -m"optimized algorithm performance"

# Compare versions
snip compare -script.py v-1 and v-2

# Rollback if needed
snip write -script.py v-1
```

### Interactive Mode Workflow
```bash
# Enter interactive mode
snip

# Now you can run commands without the 'snip' prefix:
Enter your command.. snip init
Enter your command.. snip add -main.c
Enter your command.. snip commit -main.c -m"first commit"
Enter your command.. snip log -main.c
Enter your command.. snip --exit
```

---

## ⚠️ Important Notes & Best Practices

### Command Syntax
- **File names**: Always prefix with `-` (e.g., `-main.c`, `-script.py`)
- **Commit messages**: Use quotes for messages with spaces: `-m"your message here"`
- **Version format**: Use `v-` prefix for versions (e.g., `v-1`, `v-2`)
- **Comparison**: Use `and` between versions: `v-1 and v-2`

### File Requirements
- Files must exist in the current directory or its parent directories
- The `.snip` folder must be in the same directory tree as your files
- File extensions are preserved in version storage

### Version Management
- Versions start from 1 and increment automatically
- Most recent version is shown at the top of logs
- Each commit creates a complete copy of the file (not just diffs)
- No commit size limits (limited only by disk space)

---

## 🐛 Known Issues & Limitations
Created in just 3 days so it may have 😅

### Parsing Limitations
- **Complex commit messages**: Avoid using tab characters in commit messages (used as delimiters)
- **File names with spaces**: May cause parsing issues
- **Special characters**: Some special characters in file names may not work properly

### Platform-Specific Issues
- **Windows path separators**: Uses `\\` internally, may cause issues on mixed environments
- **Command line vs Interactive**: Some argument parsing differences between modes
- **IDE detection**: Relies on system PATH for IDE detection

### Memory & Performance  
- **Large files**: Creates complete copies, not efficient for very large files
- **Many versions**: Log reading loads entire history into memory
- **No cleanup**: Old versions are never automatically removed

---

## 🔧 Technical Implementation Details

### Core Components
- **Language**: C (uses Windows-specific functions like `_mkdir`)
- **Data Structure**: Linked list for storing commit history in memory
- **File Operations**: Binary-safe file copying with `fgetc`/`fputc`
- **Path Handling**: Uses `PATH_MAX` and Windows path conventions
- **Time Handling**: Uses `strftime` for consistent timestamp formatting

### Key Functions
- `initial()` - Creates `.snip` directory
- `checkvsc()` - Searches upward for `.snip` folder  
- `addfile()` - Creates tracking structure for new files
- `commit_msg()` - Handles commit logic and message parsing
- `writingfile()` - Copies current file to version storage
- `readlog()` - Loads commit history into linked list
- `compare()` - Creates temporary files and launches IDE diff
- `open_diff_view()` - Cross-platform IDE detection and launch

### Memory Management
- Uses `malloc()` for linked list nodes
- Static buffers for path strings
- No explicit cleanup (relies on program termination)

---

## 🚀 Future Enhancements

Potential areas for improvement:
- [ ] Better argument parsing with `getopt`
- [ ] Support for binary files
- [ ] Branching and merging
- [ ] Configuration file support  
- [ ] Diff-based storage for efficiency
- [ ] Automatic cleanup of old versions
- [ ] Better error handling and validation
- [ ] Unit test suite
- [ ] Cross-platform path handling improvements

---

## 📞 Contact & Support

- 🐛 **Issues**: Report bugs or suggest features
- 🐦 **Twitter**: [@aadityansha_06](https://x.com/aadityansha_06)
- 📱 **Telegram**: [@Aadityansha](https://t.me/Aadityansha)
- 🌐 **GitHub**: [https://github.com/aadityansha06/snip](https://github.com/aadityansha06/snip)
- 📦 **Releases**: [Download Latest Version](https://github.com/aadityansha06/snip/releases)

---

## 📄 License

MIT License - Feel free to use, modify, and distribute.

---

## 🙏 Acknowledgments

- IDE detection logic assisted by AI tools
- Cross-platform compatibility considerations
- Community feedback and suggestions

---

> 💡 **Pro Tip**: `snip` works best for small to medium-sized text files where you need quick version tracking without the complexity of Git. Perfect for scripts, configuration files, and small projects!

> ⚡ **Performance Note**: Each commit stores a complete copy of your file, making it simple but potentially storage-intensive for large files or many versions.
