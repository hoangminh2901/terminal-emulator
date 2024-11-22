#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headers/split.h"

char **split_line(char *line, int *num_tokens)
{
    int max_tokens = strlen(line) / 2 + 1;
    char **tokens = malloc(max_tokens * sizeof(char *));
    *num_tokens = 0;

    char *current = line;
    bool in_quotes = false;
    char quote_char = '\0';

    while (*current)
    {
        while (*current == ' ' || *current == '\t')
        {
            current++;
        }

        if (!*current)
            break;

        char *token_start = current;

        //Check if the token is a quoted string
        while (*current)
        {
            if (!in_quotes)
            {
                if (*current == '"' || *current == '\'')
                {
                    in_quotes = true;
                    quote_char = *current;
                }
                else if (*current == ' ' || *current == '\t')
                    break;
            }
            else
            {
                if (*current == quote_char)
                    in_quotes = false;
            }
            current++;
        }

        int token_len = current - token_start;
        tokens[*num_tokens] = malloc(token_len + 1);
        strncpy(tokens[*num_tokens], token_start, token_len);
        tokens[*num_tokens][token_len] = '\0';

        (*num_tokens)++;

        if (*current)
            current++;
    }

    return tokens;
}

int main(int argc, char *argv[])
{
    char line[] = "Hello, bro! 'Hope this skibidi'";
    int num_tokens;
    char **tokens = split_line(line, &num_tokens);
    for (int i = 0; i < num_tokens; i++)
    {
        printf("Token %d: %s\n", i, tokens[i]);
        free(tokens[i]);
    }

    return 0;
}
