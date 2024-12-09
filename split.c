#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "headers/split.h"

// Function to split line into tokens, considering ;, &&, || as delimiters
char **split_line(char *line, int *num_tokens)
{
    int max_tokens = strlen(line) / 2 + 1;
    char **tokens = malloc(max_tokens * sizeof(char *));
    *num_tokens = 0;

    char *current = line;
    bool in_quotes = false;
    char quote_char = '\0';
    char *token_start = NULL;

    while (*current)
    {
        while (*current == ' ' || *current == '\t')
        {
            current++;
        }

        if (!*current)
            break;

        // Check if the token is a quoted string
        if (!in_quotes)
        {
            if (*current == '"' || *current == '\'')
            {
                in_quotes = true;
                quote_char = *current;
                current++;
                token_start = current;
                continue;
            }
            else if (*current == ';' || *current == '&' || *current == '|')
            {
                // Handle logical operators and semicolons
                if (*current == '&' && *(current + 1) == '&')
                {
                    tokens[*num_tokens] = malloc(3 * sizeof(char));
                    strncpy(tokens[*num_tokens], "&&", 3);
                    *num_tokens += 1;
                    current += 2;
                    continue;
                }
                else if (*current == '|' && *(current + 1) == '|')
                {
                    tokens[*num_tokens] = malloc(3 * sizeof(char));
                    strncpy(tokens[*num_tokens], "||", 3);
                    *num_tokens += 1;
                    current += 2;
                    continue;
                }
                else if (*current == ';')
                {
                    tokens[*num_tokens] = malloc(2 * sizeof(char));
                    strncpy(tokens[*num_tokens], ";", 2);
                    *num_tokens += 1;
                    current++;
                    continue;
                }
            }
            else if (*current == ' ' || *current == '\t')
            {
                break;
            }
        }
        else
        {
            if (*current == quote_char)
                in_quotes = false;
        }

        if (!in_quotes)
        {
            token_start = current;
            while (*current && (*current != ';' && *current != '&' && *current != '|'))
            {
                current++;
            }
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
