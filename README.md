# 🚀 MiniShell - Modern Terminal Experience

A beautiful, modern C++ shell implementation with full command history support, styled interface, and enhanced user experience. Features arrow key navigation, persistent history storage, and opens in a new terminal window with stunning visual design.

## ✨ Features

- **🎨 Modern Styled Interface**: Beautiful ASCII art banner, colorful prompts, and emoji-enhanced output
- **🪟 New Terminal Window**: Automatically opens in a new terminal window with custom title
- **⚡ Command Execution**: Execute both built-in and external commands with enhanced error messages
- **📜 Smart Command History**: Navigate through previous commands using ↑/↓ arrow keys
- **💾 Persistent History**: Automatically saves/loads history from `.minishell_history`
- **🎯 Enhanced Built-in Commands**:
  - `exit` - Exit the shell with a stylized goodbye message
  - `cd [directory]` - Change directory with visual feedback
  - `pwd` - Print working directory with folder emoji
  - `history` - Display command history in a beautiful table format
  - `clearhistory` - Clear both in-memory and file history with confirmation
  - `clear` - Clear the terminal screen
  - `help` - Show comprehensive help with styled tables
- **👤 System Information**: Displays user, hostname, date, and shell version on startup
- **🔧 Smart Terminal Detection**: Automatically detects and uses available terminal emulators

## 📋 Dependencies

This program requires the GNU Readline library for advanced line editing and history management.

### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install libreadline-dev
```

### macOS:
```bash
brew install readline
```

### CentOS/RHEL/Fedora:
```bash
# CentOS/RHEL
sudo yum install readline-devel

# Fedora
sudo dnf install readline-devel
```

## 🔨 Compilation

### Using Make (Recommended):
```bash
make
```

### Manual Compilation:
```bash
g++ -std=c++11 -Wall -Wextra -g -o minishell minishell.cpp -lreadline
```

## 🎮 Usage

### Run the shell:
```bash
./minishell
```

### Example Session:
```
╔══════════════════════════════════════════════════════════════════════════════╗
║                                                                              ║
║  ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗             ║
║  ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║             ║
║  ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║             ║
║  ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║             ║
║  ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗        ║
║  ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝        ║
║                                                                              ║
║  🚀 Modern Terminal Experience                                               ║
║  ✨ Enhanced with Command History & Arrow Key Navigation                     ║
║                                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

┌─ System Information ─────────────────────────────────────────────────┐
│ 👤 User: john                🖥️  Host: laptop                        │
│ 📅 Date: 2024-01-15 14:30:25   🐚 Shell: MiniShell v2.0            │
└───────────────────────────────────────────────────────────────────────┘

💡 Type 'help' for available commands
🔥 Use ↑/↓ arrow keys to navigate command history

┌─[john@laptop]─[home]
└─🚀 $ ls -la
total 24
drwxr-xr-x 2 user user 4096 Jan 15 10:30 .
drwxr-xr-x 3 user user 4096 Jan 15 10:29 ..
-rwxr-xr-x 1 user user 8760 Jan 15 10:30 minishell
-rw-r--r-- 1 user user 2847 Jan 15 10:30 minishell.cpp

┌─[john@laptop]─[home]
└─🚀 $ pwd
📁 /home/user

┌─[john@laptop]─[home]
└─🚀 $ exit
👋 Thanks for using MiniShell! Goodbye!
```

## 🔍 Key Features Explained

### 1. Arrow Key Navigation
- **↑ (Up Arrow)**: Navigate to previous commands in history
- **↓ (Down Arrow)**: Navigate to next commands in history
- Uses GNU Readline for smooth terminal interaction

### 2. Persistent History
- Commands are automatically saved to `.minishell_history`
- History is loaded when the shell starts
- History persists across shell sessions

### 3. Built-in Commands
- **Integrated seamlessly** with the command execution loop
- **Smart history management**: `history` and `clearhistory` commands are not added to history
- **Error handling**: Proper error messages for failed operations

### 4. Modern Visual Design
- **Colorful Interface**: Uses ANSI color codes for beautiful terminal output
- **ASCII Art Banner**: Eye-catching startup banner with shell branding
- **Styled Tables**: Command history and help displayed in formatted tables
- **Emoji Enhancement**: Visual indicators and icons throughout the interface

### 4. External Command Execution
- Uses `fork()` and `execvp()` for external command execution
- Proper process management with `waitpid()`
- Maintains shell state after command execution

## 🛠️ Implementation Details

### History Management
- In-memory vector stores command history
- GNU Readline library handles arrow key navigation
- File I/O for persistent storage
- Thread-safe operations

### Command Parsing
- Tokenizes input using string streams
- Handles multiple arguments correctly
- Distinguishes between built-in and external commands

### Process Management
- Fork-exec model for external commands
- Proper cleanup and error handling
- Signal handling for child processes

### Terminal Management
- Automatic terminal emulator detection
- New window spawning with custom titles
- ANSI color code support for styling
- Screen clearing and cursor positioning

## 🔧 Troubleshooting

### Compilation Errors
If you get linking errors related to readline:
```bash
# Make sure readline is installed
sudo apt-get install libreadline-dev

# Try explicit linking
g++ -std=c++11 -o minishell minishell.cpp -lreadline -lncurses
```

### Runtime Issues
- Ensure you have execute permissions: `chmod +x minishell`
- Check that readline library is properly installed
- Verify your terminal supports ANSI escape sequences
- If new terminal doesn't open, the shell will run in the current terminal

### Terminal Compatibility
- Supports: gnome-terminal, konsole, xterm, alacritty, kitty, terminator
- Falls back to current terminal if no compatible emulator is found
- ANSI colors work in most modern terminals

## 📁 File Structure
```
.
├── minishell.cpp      # Main source file with enhanced styling
├── Makefile          # Build configuration
├── README.md         # This file
└── .minishell_history # History file (created at runtime)
```

## 🎯 Supported Terminal Emulators
- GNOME Terminal
- Konsole (KDE)
- XTerm
- Alacritty
- Kitty
- Terminator

## 📄 License
This project is provided as-is for educational purposes.