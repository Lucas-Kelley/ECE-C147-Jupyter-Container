#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    // Use argc so gcc won't yell at me.
    if (argc < 3) {
        perror("incorrect number of parameters");
        exit(1);
    }

    // LORE: The placeholder 'PYTHON-CODE-HERE' was previously named 'CODE HERE'. However, when the
    // perl script would run, it would recursively replace the 'CODE HERE' with the content of the
    // file since, in the python files, there is a 'YOUR CODE HERE'. Realizing this took about 45
    // minutes. I will never get those 45 minutes back.
    //
    // File block template:
    // The first block is the filename in Markdown. The placeholder is FILE-NAME-HERE....
    // The second block is the python code. The placeholder is PYTHON-CODE-HERE.
    const char *file_block =
        "        {\n"
        "            \"cell_type\": \"markdown\",\n"
        "            \"metadata\": {},\n"
        "            \"source\": [\n"
        "                FILE-NAME-HERE\n"
        "            ]\n"
        "        },\n"
        "        {\n"
        "            \"cell_type\": \"code\",\n"
        "            \"execution_count\": null,\n"
        "            \"metadata\": {},\n"
        "            \"outputs\": [],\n"
        "            \"source\": [\n"
        "                PYTHON-CODE-HERE\n"
        "            ]\n"
        "        }";

    // The start of the ipynb template.
    const char* start =
        "{\n"
        "    \"cells\": [\n";

    // The end of the ipynb template. It ends the "cells" array as well as appends the necessary
    // metadata that ipynb needs. To be honest, I don't know what most of the metadata means so I
    // just copy pasted it from a blank ipynb.
    const char *end =
        "    ],\n"
        "    \"metadata\": {\n"
        "        \"kernelspec\": {\n"
        "            \"display_name\": \"Python 3 (ipykernel)\",\n"
        "            \"language\": \"python\",\n"
        "            \"name\": \"python3\"\n"
        "        },\n"
        "        \"language_info\": {\n"
        "            \"codemirror_mode\": {\n"
        "                \"name\": \"ipython\",\n"
        "                \"version\": 3\n"
        "            },\n"
        "            \"file_extension\": \".py\",\n"
        "            \"mimetype\": \"text/x-python\",\n"
        "            \"name\": \"python\",\n"
        "            \"nbconvert_exporter\": \"python\",\n"
        "            \"pygments_lexer\": \"ipython3\",\n"
        "            \"version\": \"3.10.12\"\n"
        "        }\n"
        "    },\n"
        "    \"nbformat\": 4,\n"
        "    \"nbformat_minor\": 5\n"
        "}";

    // The target file pointer is passed in as the first argument (argv[1]).
    FILE *target_fp = fopen(argv[1], "w");

    if (!target_fp) {
        perror("fopen() failed");
        exit(1);
    }

    // Write the start of the template.
    fprintf(target_fp, "%s", start);

    // Write n file blocks, where n is the number of python files to submit, passed in as the
    // second argument (argv[2]).
    for (size_t i = 0; i < strtoul(argv[2], NULL, 10) - 1; i++) {
        fprintf(target_fp, "%s,\n", file_block);
    }

    // Append the nth block but do NOT add a comma (,) at the end.
    fprintf(target_fp, "%s\n", file_block);

    // Append the end of the template.
    fprintf(target_fp, "%s", end);

    return 0;
}
