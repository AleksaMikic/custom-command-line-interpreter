# Architecture

This document explains how the interpreter is built: how one line of input travels through the program, which classes take part, and which design patterns hold them together.

## 1. The processing pipeline

Every line goes through the same four stages. Each stage either passes its result to the next one or stops the line with an error message, so nothing runs unless the whole line is valid.

```
"echo "hi" | wc -c > out.txt"
        │
        ▼
┌──────────────┐   tokens: COMMAND echo, QUOTED_STRING hi, PIPE,
│    Lexer     │           COMMAND wc, OPTION -c, REDIRECT_OUTPUT, FILE_NAME out.txt
└──────┬───────┘
       ▼
┌──────────────┐   command blocks: [echo "hi"]  [wc -c > out.txt]
│    Parser    │
└──────┬───────┘
       ▼
┌──────────────────┐   command objects: Echo, Wc (with an output Redirect)
│ SemanticAnalyzer │
└──────┬───────────┘
       ▼
┌──────────────┐   runs Echo, passes "hi" on, runs Wc, writes "2" to out.txt
│   Executor   │
└──────────────┘
```

The `Shell` class owns one instance of each stage and drives them in a loop: print the prompt, read a line, run the four stages, repeat. The loop ends when the user types `exit` or the input ends.

## 2. Stage by stage

### 2.1 Lexer

**Classes:** `Lexer`, `Token`, `TokenType`, `LexerProcessor`, `LexerException`, `TextHandler`

1. `LexerProcessor` validates the raw line first:
   - cuts it to 512 characters,
   - marks every forbidden character outside quotes,
   - detects an unclosed `"`.
   On failure it throws a `LexerException` that prints the line with a `^` under each bad position.
2. `Lexer::tokenize` walks the line character by character and produces a `Token` for each piece:

| Input looks like | Token type |
|---|---|
| `\|` | `PIPE` |
| `<` | `REDIRECT_INPUT` |
| `>` | `REDIRECT_OUTPUT` |
| `>>` | `REDIRECT_APPEND` |
| `"..."` | `QUOTED_STRING` |
| word starting with `-` | `OPTION` |
| word containing `.` | `FILE_NAME` |
| any other word | `COMMAND` |

Each token remembers its position in the line, which is used in error messages. `TextHandler` contains the small character tests (`isWhiteSpace`, `isAlphaNumeric`, `isAllowedCharacter`, ...).

### 2.2 Parser

**Classes:** `Parser`, `CommandBlock`, `ParserProcessor`, `ParserException`

1. `Parser::breakUp` splits the token list into `CommandBlock`s at every `PIPE` token. A block whose command name is only digits (for example `1`) is then replaced by the block with that number from the same line, if it exists.
2. `ParserProcessor` checks each block:
   - the first token must be a `COMMAND`,
   - every redirect must be followed by a `FILE_NAME`,
   - at most one command, option, input redirect and output redirect, and at most three arguments,
   - tokens must appear in the order command → option → arguments → input redirect → output redirect.
3. Errors from all blocks are collected and reported together in one `ParserException`.

### 2.3 Semantic analysis

**Classes:** `SemanticAnalyzer`, `SemanticAnalyzerProcessor`, `SemanticAnalyzerException`, `CommandFactory`, `CommandProcessorFactory`, the `*Processor` classes for each command

1. `SemanticAnalyzerProcessor` holds the set of known command names. An unknown name is an error.
2. For each block, `CommandFactory` creates the matching command object (`Echo`, `Wc`, ...), and `CommandProcessorFactory` creates the matching validator (`EchoProcessor`, `WcProcessor`, ...).
3. The command accepts its validator, which checks the command-specific rules, for example:
   - `wc`: the second token must be `-w` or `-c`,
   - `head`: the option must be `-n` followed by 1 to 5 digits,
   - `touch`: the file must not exist yet; `rm` and `truncate`: it must exist,
   - all file arguments must end with `.txt`, and files used for input must exist.
4. If the command is valid, `Command::create` reads its tokens and fills in its fields (argument, option, redirects).
5. All errors are gathered into one `SemanticAnalyzerException`, which prints the whole line and the reason for each failing block.

### 2.4 Execution

**Classes:** `Executor`, `StreamController`, `MyInput`, `MyOutput`, `Command` and the 11 command classes

1. `Executor::execute` calls `execute` on each command in order, passing one shared `StreamController`.
2. Inside `execute`, a command:
   1. applies its redirects (`Command::changeStreams`), which points the input at a file or the output at a file,
   2. applies its argument, which replaces the input with the quoted string or the file contents,
   3. reads all of its input as one string,
   4. computes its result,
   5. prints the result if it is the last command in the line or if its output is redirected to a file,
   6. stores the result as the input string for the next command.
3. After the last command, `StreamController::setDefault` resets input to the keyboard and output to the console.

`MyInput` and `MyOutput` wrap the real stream, which can be the console, a file or an in-memory string. This lets every command read and write the same way regardless of where the data comes from or goes to.

## 3. Design patterns

### Visitor

Validation is separated from the data it checks. Each checked object (`Lexer`, `Parser`, `SemanticAnalyzer`, every command) implements `VisitableElement::accept`, and each validator implements `visit`.

```
VisitableElement                 Visitor
   accept(Visitor&)  ───────►       positions, messages
        ▲                                 ▲
        │                                 │
   Lexer, Parser,                  LexerProcessor, ParserProcessor,
   SemanticAnalyzer,               SemanticAnalyzerProcessor,
   Echo, Wc, ...                   EchoProcessor, WcProcessor, ...
```

The interface classes (`LexerVisitor`, `ParserVisitor`, `SemanticAnalyzerVisitor`, `CommandVisitor`, and one `*Visitor` per command) declare the `visit` method each processor must provide. The base `Visitor` class stores the collected error messages and positions.

Benefit: all rules for one command are in one file, and adding a rule never touches the command's execution code.

### Factory

`CommandFactory` and `CommandProcessorFactory` turn a command name into the right object. They are the only places that know the full list of command classes.

### Singleton (with a stack)

`Shell::getInstance` returns the active interpreter. The instances live on a stack: `batch` pushes a new `Shell`, runs its lines, and pops it again. Commands such as `prompt` change the shell on top of the stack.

### Prototype

Every command implements `clone`, so a command object can be copied without knowing its concrete class.

## 4. Adding a new command

1. Create the command class (`MyCmd.h` / `MyCmd.cpp`) deriving from `Command`. Implement `create`, `execute`, `clone` and `accept`.
2. Create the visitor interface (`MyCmdVisitor`) deriving from `CommandVisitor`.
3. Create the validator (`MyCmdProcessor`) deriving from `Visitor` and `MyCmdVisitor`. Put all rules for the command in its `visit` method.
4. Add the command name to `SemanticAnalyzerProcessor::setAllowedCommands`.
5. Add one branch to `CommandFactory::createCommand` and one to `CommandProcessorFactory::createCommandProcessor`.
6. Put the command class in `commands/` and the visitor and validator in `validation/`, add them to the project, and document the command in the README.
