// reccat.c — Recursive folder-to-text merger
// Author: Md Noornawaz Rahman 
// Description: Reads all text files in a directory (recursively)
// and merges them into a single output text file with headers.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#define BUF_SIZE 4096

// Forward declarations
void process_directory(const char *dirpath, FILE *out, const char *output_path);
void process_file(const char *filepath, FILE *out);
int is_binary(const char *filepath);
int should_skip_dir(const char *dirname);
int should_skip_file(const char *filename);



int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source_dir> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src_dir = argv[1];
    const char *output_path = argv[2];

    FILE *out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "Error: cannot create output file %s: %s\n", output_path, strerror(errno));
        return EXIT_FAILURE;
    }

    process_directory(src_dir, out, output_path);

    fclose(out);
    printf("✅ Merged text written to %s\n", output_path);
    return EXIT_SUCCESS;
}


// Recursively process directories
void process_directory(const char *dirpath, FILE *out, const char *output_path) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        fprintf(stderr, "Error: cannot open directory %s: %s\n", dirpath, strerror(errno));
        return;
    }

    printf("📂 Entering directory: %s\n", dirpath);

    struct dirent *entry;
    char path[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (should_skip_dir(entry->d_name)) {
            printf("⏭️  Skipping directory: %s/%s\n", dirpath, entry->d_name);
            continue;
        }
            

        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);

        struct stat st;
        if (stat(path, &st) == -1) {
            fprintf(stderr, "Warning: cannot stat %s: %s\n", path, strerror(errno));
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // Recurse into subdirectory
            process_directory(path, out, output_path);
        } else if (S_ISREG(st.st_mode)) {
            // Skip output file itself (to avoid recursion)
            if (realpath(path, NULL) && realpath(output_path, NULL)
                && strcmp(realpath(path, NULL), realpath(output_path, NULL)) == 0) {
                continue;
            }

             // Skip specific files
            if (should_skip_file(entry->d_name)) {
                printf("⏭️  Skipping file: %s/%s\n", dirpath, entry->d_name);
                continue;
            }
            // Process text files only
            if (!is_binary(path)) {
                process_file(path, out);
            } else {
                printf("⚙️  Skipped binary file: %s\n", path);
            }

        }
    }

    closedir(dir);
    printf("📁 Exiting directory: %s\n", dirpath);
}

// Write one file’s contents to the output file with header
void process_file(const char *filepath, FILE *out) {
    FILE *in = fopen(filepath, "r");
    if (!in) {
        fprintf(stderr, "Warning: cannot open file %s: %s\n", filepath, strerror(errno));
        return;
    }

    printf("📝 Processing file: %s\n", filepath);

    fprintf(out, "\n===== FILE: %s =====\n", filepath);

    char buffer[BUF_SIZE];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        fwrite(buffer, 1, n, out);
    }

    fclose(in);
}

// Heuristic check if file is binary
int is_binary(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) return 1;

    unsigned char buf[512];
    size_t n = fread(buf, 1, sizeof(buf), f);
    fclose(f);

    for (size_t i = 0; i < n; i++) {
        if (buf[i] == '\0') return 1;  // null byte → binary
        if (!isprint(buf[i]) && !isspace(buf[i])) {
            // Too many non-printables → likely binary
            static int count = 0;
            if (++count > 10) return 1;
        }
    }
    return 0;
}


int should_skip_dir(const char *dirname) {
    const char *skip_list[] = {".git", "node_modules", ".idea", ".vscode", NULL};
    for (int i = 0; skip_list[i] != NULL; i++) {
        if (strcmp(dirname, skip_list[i]) == 0)
            return 1;
    }
    return 0;
}


int should_skip_file(const char *filename) {
    const char *skip_list[] = {"package-lock.json", "yarn.lock", NULL};
    for (int i = 0; skip_list[i] != NULL; i++) {
        if (strcmp(filename, skip_list[i]) == 0)
            return 1;
    }

    // Skip .svg files
    const char *ext = strrchr(filename, '.');
    if (ext && strcmp(ext, ".svg") == 0)
        return 1;
    return 0;
}
