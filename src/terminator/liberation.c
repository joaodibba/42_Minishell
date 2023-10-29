
#include "minishell.h"

void	liberate_vector(char **vector)
{
	int	i;

	i = 0;
	if (!vector)
		return ;
	while (vector[i])
		free(vector[i++]);
	free(vector);
}

// liberates the memory allocated for the tokens
void    liberate_tokens(t_token *head)
{
    t_token *temp;

    while (head)
    {
        temp = head->next;
        free(head->token);
        free(head);
        head = temp;
    }
}

void    liberate_envs(t_envs *head)
{
    t_envs *temp;

    while (head)
    {
        temp = head->next;
        free(head->key);
        free(head->value);
        free(head);
        head = temp;
    }
}

// void    liberate_baobab(t_baobab *head)
// {
//     t_baobab *temp;

//     while (head)
//     {
//         temp = head->next;
//         free(head);
//         head = temp;
//     }
// }

// liberates the memory allocated for the tokens and the baobab tree
void    liberation(t_minivault *minivault)
{
    liberate_envs(minivault->envs);
    liberate_tokens(minivault->tokens);
    // liberate_baobab(minivault->baobab);
}