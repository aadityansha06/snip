# 🚀 snip - Lightweight Git-like Version Control System

`snip` is a minimal yet powerful command-line version control system written in C. It provides Git-like functionality for tracking file changes, managing versions, and comparing code across different commits - all stored locally in a `.snip` directory.

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

---

## 🛠️ Installation

### Build from Source
```bash
gcc -o snip snip.c
```

### System-wide Installation (Optional)
1. Place `snip.exe` in a dedicated folder (e.g., `C:\tools\snip\`)
2. Add the folder to your system's PATH environment variable
3. Restart your terminal

---

## 📋 Commands Reference

| Command | Description | Example |
|---------|-------------|---------|
| `snip init` | Initialize version control in current directory | `snip init` |
| `snip add -<filename>` | Add file to tracking | `snip add -main.c` |
| `snip commit -<filename> -m"<message>"` | Commit file changes | `snip commit -main.c -m"fixed bug"` |
| `snip log -<filename>` | Show commit history | `snip log -main.c` |
| `snip write -<filename> -<version>` | Restore specific version | `snip write -main.c -2` |
| `snip compare -<filename> v-<num> and v-<num>` | Compare two versions | `snip compare -main.c v-1 and v-3` |

---

## 🚦 Quick Start

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

### 3. Make Your First Commit
```bash
snip commit -hello.c -m"initial version"
# Output: Commit message: initial version
#         committed
#         Successfully inserted..!!
```

### 4. View Commit History
```bash
snip log -hello.c
```
```
+-----+-----------------------------------------------------------------------------+
| Time                 | 		Commit-msg                | version |
+-----+-----------------------------------------------------------------------------+
| 04-08-2025 14:30:22 | initial version                        | 1      |
+-----+-----------------------------------------------------------------------------+
```

### 5. Restore Previous Version
```bash
snip write -hello.c -1
# Output: Over-write Successfully of version-1!!
```

---

## 🔍 Advanced Features

### Version Comparison
Compare any two versions of your file in your preferred IDE:
```bash
snip compare -main.c v-1 and v-3
```

**Supported IDEs:**
- Visual Studio Code (`code --diff`)
- PyCharm (`pycharm`)
- Sublime Text (`subl`)
- Meld (Linux only)

The tool automatically detects your installed IDE and opens a side-by-side diff view.

### Smart File Discovery
`snip` automatically searches upward through directories to find:
- The `.snip` repository folder
- Source files you want to commit

This means you can run `snip` commands from any subdirectory within your project.

---

## 📁 Directory Structure

When you run `snip init`, it creates:
```
your-project/
├── .snip/
│   └── filename.ext/
│       ├── log.txt          # Commit history
│       └── .version/
│           ├── 1            # Version 1 of file
│           ├── 2            # Version 2 of file
│           └── ...
└── your-files...
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
snip write -script.py -1
```

---

## ⚠️ Known Issues & Limitations

### Command Line Argument Parsing
- **Spaces in commit messages**: Use quotes carefully - `snip commit -file.c -m"message with spaces"`
- **PowerShell compatibility**: Some argument parsing issues in PowerShell; Command Prompt recommended
- **File paths with spaces**: May cause parsing errors

### Platform-Specific Notes
- **Windows**: Uses `\` path separators and Windows-specific directory functions
- **Path limits**: Respects `PATH_MAX` system limits
- **IDE detection**: Automatically detects available editors for comparison feature

---

## 🔧 Technical Details

- **Language**: C (C99 standard)
- **Dependencies**: Standard C library only
- **Storage**: Local filesystem (no network required)
- **Memory**: Dynamic linked list for commit history
- **File handling**: Binary-safe file copying
- **Cross-platform**: Conditional compilation for different OS

---

## 🤝 Contributing

We welcome contributions! Areas for improvement:

- [ ] Better argument parsing (supporting getopt)
- [ ] Enhanced commit message handling
- [ ] Branch support
- [ ] Config file support
- [ ] Better error handling
- [ ] Unit tests

---

## 📞 Contact & Support

- 🐦 **Twitter**: [@aadityansha_06](https://x.com/aadityansha_06)
- 📱 **Telegram**: [@Aadityansha](https://t.me/Aadityansha)

---

## 📄 License

MIT License - Feel free to use, modify, and distribute. Credit appreciated but not required.

---

> 💡 **Pro Tip**: `snip` works great for small projects, scripts, and configuration files where you need quick version tracking without the complexity of Git!
