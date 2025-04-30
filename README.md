# Minishell - A Custom Unix Shell in C

## Overview

This project is a full-featured **Unix shell implementation written in C**, built from scratch without relying on external shell interpreters.  
It supports command parsing, process creation, redirection, piping, environment variable handling, wildcards, logical operators, and interactive terminal behavior.  
Inspired by the POSIX shell, this project demonstrates deep system programming, memory management, and process control — all under strict constraints.

---

## Features

- **Prompt and Input Handling**
  - Custom prompt using `readline` with history support
  - Proper handling of quotes (`'`, `"`) and command separators (`&&`, `||`, parentheses)

- **Command Execution**
  - Executes built-in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
  - Executes binaries using `$PATH` resolution or relative/absolute paths
  - Handles exit codes and `$?` expansion

- **Redirections**
  - `<`, `>`, `>>`, `<<` (heredoc with delimiter support)

- **Pipes and Chaining**
  - Support for pipelines (`cmd1 | cmd2 | cmd3`)
  - Logical operators: `&&`, `||` with proper precedence handling via parentheses

- **Environment Expansion**
  - `$VARIABLE` and wildcard `*` expansion in current directory
  - Full shell-like parameter expansion behavior

- **Signal Handling**
  - `Ctrl-C`: Interrupt current command and show new prompt
  - `Ctrl-D`: Exit shell cleanly
  - `Ctrl-\`: Ignored (like in Bash)

- **Single Global Variable Policy**
  - Only one global variable is used (for signal state), the rest is fully modularized

---

## Build & Run

```bash
git clone https://github.com/luizakar2002/minishell.git
cd minishell
make
./minishell
