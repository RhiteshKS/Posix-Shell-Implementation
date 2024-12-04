# POSIX Shell Implementation

A custom **POSIX-compliant shell** implemented in **C/C++**, supporting multiple built-in commands, process management, input/output redirection, pipelines, and other advanced features. This project is developed as part of the Advanced Operating Systems coursework.

---

## Features

- **Custom Shell Interface**: Interactive shell with user-specific prompts and support for complex command sequences.
- **Built-in Commands**: Includes `cd`, `ls`, `pwd`, `echo`, and `pinfo` with additional functionality like flags and recursive search.
- **Process Management**:
  - Supports background (`&`) and foreground processes with real-time process ID tracking.
  - Handles job control via signals (`CTRL-C`, `CTRL-Z`, `CTRL-D`).
- **I/O Redirection**: Handles input (`<`), output (`>`), and append (`>>`) redirection.
- **Pipelines**: Supports multiple command pipelines (`|`) with redirection.
- **Autocomplete**: Tab-based file and directory autocomplete.
- **Command History**:
  - Stores up to 20 commands persistently across sessions.
  - Features history navigation with arrow keys.
- **Recursive Search**: Search for files or folders within the current directory.

---

## Requirements

- **Language**: C/C++
- **Libraries**: Standard C/C++ libraries for process and file system management.
- **Environment**: Compatible with Linux/Unix systems.

---

