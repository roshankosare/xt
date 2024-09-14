

#include "../../include/asm/helper.h"

void print_data_section(FILE *fp, const char *instruction)
{
    char *buffer = NULL;
    long file_size;
    int data_start = -1;
    long original_pos = ftell(fp); // Save the original file pointer position

    // Get file size
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    // Allocate buffer to read file contents
    buffer = malloc(file_size + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Read the file into the buffer
    fread(buffer, file_size, 1, fp);
    buffer[file_size] = '\0';

    // Find the .data section in the file
    char *data_ptr = strstr(buffer, "section .data");
    if (data_ptr)
    {
        data_start = data_ptr - buffer + strlen("section .data");
    }

    if (data_start != -1)
    {
        // Move file pointer to the .data section and write the new content
        fseek(fp, data_start, SEEK_SET);
        fprintf(fp, "\n%s", instruction);

        // Write the rest of the original file
        fprintf(fp, "%s", buffer + data_start);

        // Adjust original_pos if it's after the insertion point
        if (original_pos >= data_start)
        {
            original_pos += strlen(instruction) + 1; // Adjust for new content
        }
    }
    else
    {
        fprintf(stderr, "Error: .data section not found\n");
    }

    // Clean up
    free(buffer);
    fseek(fp, original_pos, SEEK_SET); // Restore the adjusted file pointer position
}
void print_bss_section(FILE *fp, const char *instruction)
{

    char *buffer = NULL;
    long file_size;
    int bss_start = -1;
    long original_pos = ftell(fp); // Save the original file pointer position

    // Get file size
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    // Allocate buffer to read file contents
    buffer = malloc(file_size + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Read the file into the buffer
    fread(buffer, file_size, 1, fp);
    buffer[file_size] = '\0';

    // Find the .bss section in the file
    char *bss_ptr = strstr(buffer, "section .bss");
    if (bss_ptr)
    {
        bss_start = bss_ptr - buffer + strlen("section .bss");
    }

    if (bss_start != -1)
    {
        // Move file pointer to the .bss section and write the new content
        fseek(fp, bss_start, SEEK_SET);
        fprintf(fp, "\n%s", instruction);

        // Write the rest of the original file
        fprintf(fp, "%s", buffer + bss_start);

        // Adjust original_pos if it's after the insertion point
        if (original_pos >= bss_start)
        {
            original_pos += strlen(instruction) + 1; // Adjust for new content
        }
    }
    else
    {
        fprintf(stderr, "Error: .bss section not found\n");
    }

    // Clean up
    free(buffer);
    fseek(fp, original_pos, SEEK_SET); // Restore the adjusted file pointer position
}
