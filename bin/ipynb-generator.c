#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    // Use argc so gcc won't yell at me.
    if (argc < 3) {
        perror("incorrect number of parameters");
        exit(1);
    }

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

    const char* start =
        "{\n"
        "    \"cells\": [\n";

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

    FILE *target_fp = fopen(argv[1], "w");

    if (!target_fp) {
        perror("fopen() failed");
        exit(1);
    }

    fprintf(target_fp, "%s", start);

    for (size_t i = 0; i < strtoul(argv[2], NULL, 10) - 1; i++) {
        fprintf(target_fp, "%s,\n", file_block);
    }

    fprintf(target_fp, "%s\n", file_block);
    fprintf(target_fp, "%s", end);

    return 0;
}
