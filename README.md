# reccat

**reccat** (Recursive Cat) is a lightweight C utility that recursively merges all text files in a directory into a single output file with clear headers. Perfect for creating consolidated codebases for documentation, AI context, or archival purposes.

## Features

- 📂 **Recursive directory traversal** - Processes all subdirectories automatically
- 🎯 **Smart filtering** - Skips binary files, common directories (`.git`, `node_modules`), and lock files
- 📝 **Clear file headers** - Each file's content is prefixed with its path for easy navigation
- ⚡ **Fast and efficient** - Written in C with minimal dependencies
- 🛡️ **Safe** - Automatically skips the output file to prevent recursion

## Installation

### Build from source

```bash
chmod +x build.sh
./build.sh
```

Or manually:

```bash
gcc reccat.c -o reccat
```

## Usage

```bash
./reccat <source_directory> <output_file>
```

### Example

```bash
./reccat ./my-project merged-output.txt
```

This will create `merged-output.txt` containing all text files from `my-project` with headers like:

```
===== FILE: ./my-project/src/main.c =====
[file contents here]

===== FILE: ./my-project/include/header.h =====
[file contents here]
```

## What Gets Skipped

### Directories
- `.git`
- `node_modules`
- `.idea`
- `.vscode`

### Files
- Binary files (detected heuristically)
- `package-lock.json`
- `yarn.lock`
- The output file itself

## Use Cases

- 🤖 **AI Context** - Create a single file to feed into AI models
- 📚 **Documentation** - Generate a complete codebase snapshot
- 🔍 **Code Review** - Share entire project structure in one file
- 📦 **Archival** - Create text-based backups of projects
- 🎓 **Learning** - Study open-source projects more easily

## Output Format

Each file is separated with a clear header:

```
===== FILE: <relative-path> =====
<file contents>
```

## Requirements

- GCC or any C compiler
- POSIX-compliant system (Linux, macOS, BSD)
- Standard C library

## Author

**Md Noornawaz Rahman**

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

---

**Note**: This tool is designed for text files. Binary files are automatically detected and skipped.
