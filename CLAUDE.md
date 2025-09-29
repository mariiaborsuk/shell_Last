# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a **minishell** implementation in C that recreates basic shell functionality similar to bash. The project consists of a lexer, parser, and execution engine that handles command execution, pipes, redirections, built-in commands, and environment variables.

## Build and Development Commands

### Build Commands
- `make` or `make all` - Compile the minishell executable
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Clean and rebuild everything

### Testing
- `cd Tester && ./tester` - Run all tests for mandatory part
- `cd Tester && ./tester builtins` - Test built-in commands only
- `cd Tester && ./tester pipes` - Test pipe functionality only
- `cd Tester && ./tester redirects` - Test redirections only
- `cd Tester && ./tester syntax` - Test syntax error handling
- `cd Tester && ./tester os_specific` - Test OS-specific functionality
- `cd Tester && ./tester manual_tests/mandatory` - Test manadatory part
- `cd Tester && ./tester manual_tests/not_mandatory` - Test optional part
- `cd Tester && ./tester manual_tests/heredoc` - Test commands with heredocs
- `cd Tester && ./tester manual_tests/signals` - Test commands with signals

### Memory Debugging
- `valgrind --suppressions=ignore_leaks.supp --leak-check=full ./minishell` - Run with memory leak detection using provided suppressions file

## Code Architecture

### Core Data Structures (types.h)
- **t_minishell**: Main shell state containing tokens, AST, variables, and execution context
- **t_token**: Tokenized input with type, value, and quote information
- **t_ast**: Abstract Syntax Tree for command parsing and execution
- **t_cmd**: Command structure with arguments and redirections
- **t_var**: Environment and shell variables with export status
- **t_redirect**: File redirection information (input, output, append, heredoc)

### Module Organization

#### Core Parsing (srcs/)
- **lexing.c**: Tokenizes input into structured tokens
- **parsing.c**: Builds AST from tokens
- **create_token*.c**: Token creation utilities
- **expand_var*.c**: Variable expansion in tokens
- **validation*.c**: Syntax validation
- **clean*.c**: Memory management and cleanup

#### Execution Engine (mock_tree/)
- **execute.c**: Main execution logic and AST traversal
- **exec.c**: External command execution
- **pipes.c**: Pipe handling between commands
- **redirs.c**: File redirection management
- **heredoc.c**: Here-document implementation
- **builtins.c**: Built-in command implementations (cd, echo, env, exit, export, pwd, unset)
- **variables.c**: Environment variable management
- **signal.c**: Signal handling

#### Libraries
- **libs/Libft/**: Custom C library with string and memory utilities

### Key Architectural Patterns

1. **Three-Phase Processing**: Input → Lexing → Parsing → Execution
2. **AST-Based Execution**: Commands represented as tree nodes for pipes and command sequences
3. **Token-Based Parsing**: All input first tokenized with type and quote state tracking
4. **Modular Built-ins**: Each built-in command has dedicated implementation
5. **Memory Management**: Consistent cleanup patterns with dedicated free functions for each data structure

### Critical Implementation Details

- **Quote Handling**: Tracks single/double quote state throughout parsing for proper variable expansion
- **Variable Expansion**: Handles `$VAR` and `${VAR}` syntax with proper quote context
- **Pipe Implementation**: Uses process forking with proper file descriptor management
- **Signal Handling**: Manages SIGINT and SIGQUIT appropriately for interactive shell behavior
- **Here-documents**: Implements `<<` operator with delimiter-based input collection

### File Dependencies

The project uses a strict include hierarchy:
- Core types defined in `includes/types.h`
- Main shell functions in `includes/minishell.h`
- Execution functions in `includes/execute.h`
- Signal handling in `includes/signals.h`
- Error definitions in `includes/errors.h`

### Memory Management Strategy

Each major data structure has corresponding cleanup functions:
- `free_minishell()` - Complete shell cleanup
- `free_ast()` - AST node cleanup
- `free_tokens()` - Token list cleanup
- `free_var_list()` - Variable list cleanup
- `free_redirect()` - Redirection list cleanup
