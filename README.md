# 🧠 snip - Minimal Git-like Version Tracker for Code Snippets

`snip` is a lightweight command-line tool that works like a mini Git for your code snippets. It allows you to **init**, **add**, and **commit** files while tracking changes over time.

---

## 📦 Features

- Initialize a local `.snip` tracking folder
- Add files to track
- Commit changes with messages
- Works both as a normal `.exe` and via system-wide commands

---

## 🛠 Installation & Usage

### 🔸 Option 1: Run as `.exe` directly

Just build the code and run the `snip.exe` file from your terminal:

```bash
snip.exe init
snip.exe add file.c
snip.exe commit -file.c -m"initial version"
````
Option 2: Use via system environmental variable (like Git)

1. Place the snip.exe in a folder (e.g., C:\snip\).


2. Add that folder to System Environment Variables → Path.



Now you can run snip from any terminal:
```` bash

snip init
snip add file.c
snip commit -file.c -m"added main logic"
````

---

🔧 Commands

Command	Usage

snip init	Initializes .snip directory to start tracking
snip add <filename>	Adds file to tracking list
snip commit -<filename> -m"<message>"	Commits file changes with a message
snip status	Shows files and their tracked status
snip log	Displays commit history
snip help or snip --help	Shows help menu


> ✅ Tip: -<filename> in commit must begin with a dash (e.g., -main.c) just like git.




---

⚠️ Known Issues

🐛 Problem with spaces in PowerShell or VS Code terminal

When using system-wide snip command from folders with spaces in path, or if arguments have . or quotes:

snip commit -example.c -m"message with space"

It may reconstruct command incorrectly like:

snip commit -example .c -mexample

This is due to how arguments are split in certain shells (especially PowerShell). The tool currently assumes space as a delimiter and doesn't yet fully support quoted strings or escape characters robustly.

> ❌ Urgent: Avoid using paths or argument values with spaces, or use Command Prompt (cmd.exe) instead of PowerShell.
> Need someone to fix this , pls reach me out 




---

👨‍💻 Developer Notes

This project is built in C language, and manually handles argument parsing without getopt.

Feel free to fork and contribute:

Adding support for full quoted strings

Improving commit metadata

Creating visual diff logs



---
---

## 📫 Reach Me

- 🐦 X (Twitter): [@aadityansha_06](https://x.com/aadityansha_06)
- 📬 Telegram: [@Aadityansha](https://t.me/Aadityansha)
<hr>
📄 License

MIT License — use freely and credit appreciated.


---
