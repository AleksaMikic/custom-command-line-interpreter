# Custom Command Line Interpreter

A small command line interpreter written in C++ from scratch. It reads a line of input, splits it into tokens, checks the syntax and meaning of every command, and then runs the commands, with support for pipes (`|`) and input/output redirection (`<`, `>`, `>>`).

The project was built to practise compiler-style input processing (lexer → parser → semantic analysis → execution) and object-oriented design in C++, including the Visitor, Factory and Singleton patterns.

## Features

- 11 built-in commands: `echo`, `prompt`, `time`, `date`, `touch`, `truncate`, `rm`, `wc`, `tr`, `head`, `batch`
- Pipelines: the output of one command becomes the input of the next (`echo "a b c" | wc -w`)
- Redirection: read input from a file (`<`), write output to a file (`>`) or append to it (`>>`)
- Batch execution: run a list of commands from a text file
- Configurable prompt, permanent or for one line only
- Detailed error messages for each stage: lexical, syntax and semantic errors are reported with the faulty command block and the reason

## Building and running

### Requirements

- Windows
- Visual Studio 2022 with the **Desktop development with C++** workload (platform toolset v143)

### Steps

1. Clone the repository or download it as a ZIP and extract it.
2. Open `CustomCommandLineInterpreter.sln` in Visual Studio 2022.
3. In the toolbar, choose a configuration (`Debug` or `Release`) and platform (`x64`).
4. Build with **Build → Build Solution** (`Ctrl+Shift+B`).
5. Run with **Debug → Start Without Debugging** (`Ctrl+F5`).
6. A console window opens showing the prompt `$`. Type a command and press Enter.

Type `exit` to stop the interpreter. It also stops when the input ends (`Ctrl+Z` and Enter at the prompt).

## Quick example

```
$ echo "Hello world"
Hello world
$ echo "one two three" | wc -w
3
$ echo "saved line" > notes.txt
$ wc -c notes.txt
11
$ echo "hello world" | tr "o" "0"
hell0 w0rld
$ date
2026-10-31
$ prompt "#"
# time
16:02:49
```

## Command syntax

Every command line is made of one or more **command blocks** separated by `|`. Inside a block the tokens must appear in this order:

```
command [option] [argument(s)] [< input.txt] [> output.txt | >> output.txt]
```

| Token | How it is recognised | Example |
|---|---|---|
| Command | A word made of letters and digits | `echo`, `wc` |
| Option | A word that starts with `-` | `-w`, `-n5`, `-t` |
| File name | A word that contains a `.` | `input.txt` |
| Quoted string | Text inside double quotes, may contain any character | `"Hello, world!"` |
| Pipe | `\|` | `echo "a" \| wc -c` |
| Input redirect | `<` followed by a file name | `< input.txt` |
| Output redirect | `>` followed by a file name (overwrites) | `> out.txt` |
| Append redirect | `>>` followed by a file name (appends) | `>> log.txt` |

General rules:

- The first token of each block must be a command.
- A block may contain at most one option, at most three arguments, one input redirect and one output redirect.
- Outside of quotes only letters, digits, `.`, `-`, `/`, spaces, tabs and `<`, `>`, `|` are allowed.
- A quoted string must be closed with a second `"`.
- Input lines longer than 512 characters are cut to 512 characters.
- File arguments must be `.txt` files.

### Where a command gets its input

Commands that work on text (`echo`, `wc`, `tr`, `head`, `batch`, `prompt`) take their input from the first available source:

1. An argument: a quoted string is used as the text itself, a `.txt` file name means "read the contents of this file".
2. An input redirect: `< file.txt`.
3. The previous command in a pipeline.
4. The keyboard, when nothing else is given. Type the text and finish it with `Ctrl+Z` and Enter.

### Where a command writes its output

- In a pipeline, only the last command prints to the console. Every earlier command passes its result to the next one.
- An output redirect (`>` or `>>`) writes the result to the file instead.

### Reusing a block from the same line

A command name made only of digits, such as `1`, is replaced by the command block with that number from the same line (blocks are counted from 1). A number with no matching block is reported as an unknown command.

```
$ echo "a" | 1
a
```

## Commands

### `echo` — print text

```
echo [argument]
```

Prints its input unchanged. The argument is a quoted string or a `.txt` file. Options are not supported.

```
$ echo "Hello world"
Hello world
$ echo input.txt
(contents of input.txt)
$ echo < input.txt
(contents of input.txt)
```

### `prompt` — change the prompt sign

```
prompt [-t] argument
```

Replaces the `$` sign shown before each command. With `-t` the new sign is used for the next line only, after which the previous sign comes back.

```
$ prompt "#"
# prompt -t "tmp>"
tmp> echo "x"
x
# 
```

### `time` — print the current time

```
time
```

Prints the current local time as `HH:MM:SS`. Takes no options or arguments; only redirection is allowed.

### `date` — print the current date

```
date
```

Prints the current local date as `YYYY-MM-DD`. Takes no options or arguments; only redirection is allowed.

### `touch` — create a new empty file

```
touch file.txt
```

Creates an empty `.txt` file. Reports an error if the file already exists.

### `truncate` — empty a file

```
truncate file.txt
```

Removes all content from an existing `.txt` file, leaving it empty.

### `rm` — delete a file

```
rm file.txt
```

Deletes an existing `.txt` file. Options and quoted strings are not supported.

### `wc` — count words or characters

```
wc -w [argument]
wc -c [argument]
```

| Option | Result |
|---|---|
| `-w` | Number of words (groups of characters separated by whitespace) |
| `-c` | Number of characters, including spaces and line breaks |

The option is mandatory and must come directly after `wc`.

```
$ echo "a b c d" | wc -w
4
$ wc -c input.txt
29
```

### `tr` — remove or replace text

```
tr [argument] "what" ["with"]
```

Replaces every occurrence of `what` with `with`. If `with` is left out, every occurrence of `what` is removed. The input can be a quoted string, a `.txt` file, a redirect, or the previous command in a pipeline.

```
$ tr "hello world" "o" "0"
hell0 w0rld
$ echo "hello world" | tr "o" "0"
hell0 w0rld
$ tr input.txt "line "
one
two
three
```

### `head` — print the first lines

```
head -nCOUNT [argument]
```

Prints the first `COUNT` lines of the input. `COUNT` is written right after `-n`, with no space, and can have at most 5 digits.

```
$ head -n2 input.txt
line one
line two
```

### `batch` — run commands from a file

```
batch [argument]
```

Reads its input line by line and runs every line as a separate command line, in a new interpreter instance. When all lines are done, control returns to the original interpreter.

```
$ batch commands.txt
```

where `commands.txt` contains, for example:

```
echo "first from batch"
time
```

## Error messages

Each stage reports its own kind of error, and nothing is executed if any error is found.

**Lexical error** (forbidden character, unclosed quote). The `^` marks the position:

```
$ echo $abc
Lexical error has occurred!
echo $abc
     ^
```

**Semantic error** (unknown command, wrong option, missing file, wrong number of arguments):

```
$ echo "a" "b"
Semantic errors detected in command line:
echo "a" "b"

Exception/s detected in command block 1:
echo "a" "b" Semantical error, command Echo has too much tokens of type: ARGUMENT
```

## How it works

Every line typed by the user goes through four stages:

```
input line → Lexer → Parser → SemanticAnalyzer → Executor → output
              tokens   command blocks   command objects
```

1. **Lexer** splits the line into tokens (command, option, file name, quoted string, pipe, redirect) and rejects forbidden characters.
2. **Parser** splits the tokens into command blocks at every `|` and checks the order and count of tokens in each block.
3. **SemanticAnalyzer** checks that each command exists and is used correctly (allowed options, existing files, mandatory arguments), then builds a command object for it.
4. **Executor** runs the command objects in order and passes the output of one to the next through the `StreamController`.

Each validation step is implemented as a separate visitor class, so the rules for a command live in one place (for example `EchoProcessor` for `echo`). A detailed description is in [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## Project structure

```
CustomCommandLineInterpreter/
├── app/          entry point and the Shell loop
├── lexer/        tokenizing the input line
├── parser/       splitting into command blocks and syntax checks
├── semantic/     semantic checks and the command factories
├── execution/    running commands and managing streams
├── commands/     the 11 commands and their building blocks
├── validation/   per-command validation rules (visitors)
└── common/       shared base classes and exceptions
```

Solution Explorer in Visual Studio shows the same groups. All eight folders are listed in the project's include directories, so source files include headers by name only (`#include "Lexer.h"`).

## Known limitations

- Windows / Visual Studio only for now (some exception classes do not compile with GCC or Clang without small changes).
- Only `.txt` files are accepted as file arguments.
- Quoted strings cannot contain an escaped `"`.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
