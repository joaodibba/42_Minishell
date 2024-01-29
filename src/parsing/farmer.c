/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   farmer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalves-c < jalves-c@student.42lisboa.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 18:18:04 by jalves-c          #+#    #+#             */
/*   Updated: 2024/01/29 18:18:06 by jalves-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redirs(t_redir *head)
{
	while (head)
	{
		printf("REDIRECTION: ");
		if (head->operator == GREAT)
			printf(">");
		else if (head->operator == LESS)
			printf("<");
		else if (head->operator == DGREAT)
			printf(">>");
		else if (head->operator == DLESS)
			printf("<<");
		printf(" | WORD: %s\n", head->word);
		head = head->next;
	}
}

void	print_words(t_word *head)
{
	while (head)
	{
		printf("WORD: %s \n", head->word);
		head = head->next;
	}
}

void	print_baobab(t_minivault *minivault)
{
	int	i;

	i = 0;
	if (!minivault->baobab || !minivault->baobab->pipeline)
		return ;
	while (minivault->baobab->pipeline[i])
	{
		if (i != 0)
			printf("\n----------------------\n");
		printf("Command: %d\n", i);
		printf("Command pos: %d", minivault->baobab->pipeline[i]->pos);
		printf("\n----------------------\n");
		if (minivault->baobab->pipeline[i]->redir_in)
			print_redirs(minivault->baobab->pipeline[i]->redir_in);
		if (minivault->baobab->pipeline[i]->words)
			print_words(minivault->baobab->pipeline[i]->words);
		if (minivault->baobab->pipeline[i]->redir_out)
			print_redirs(minivault->baobab->pipeline[i]->redir_out);
		printf(RESET_COLOR);
		i++;
	}
}

//-----------------------  ↑ SHITY PRINTS ↑  ----------------------- //

t_command	*split_list(t_minivault *minivault, \
			t_token *list, t_content_type type)
{
	static t_token	*current;
	t_command		*command;

	current = NULL;
	command = NULL;
	command = (t_command *)malloc(sizeof(t_command));
	command->words = NULL;
	command->redir_in = NULL;
	command->redir_out = NULL;
	if (!command || !list)
		return (NULL);
	if (current == NULL)
		current = list;
	while (current)
	{
		if (current->type == type)
		{
			current = current->next;
			break ;
		}
		if (current && current->next && current->type == REDIRECTION && \
		(current->next->type == LITERAL || current->next->type == QUOTED))
		{
			add_redirection(minivault, &command, current, current->next);
			current = current->next->next;
		}
		if (current && (current->type == LITERAL || current->type == QUOTED))
		{
			add_word(&(command->words), current);
			current = current->next;
		}
	}
	return (command);
}

int	count_tokens(t_content_type token_type, t_token *tokens)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current)
	{
		if (current->type == token_type)
			i++;
		current = current->next;
	}
	return (i);
}

void	open_pipes(t_minivault *minivault, int i, int command_count)
{
	while (i < FOPEN_MAX)
		ft_bzero(minivault->baobab->pipe_fd[i++], 2);
	i = 0;
	while (i < command_count - 1 && i < FOPEN_MAX)
	{
		if (pipe(minivault->baobab->pipe_fd[i]) < 0)
		{
			// Handle error
		}
		i++;
	}
}

/*
 * To calculate the position of the cmd
 * asume there is always a command if it reaches this point
 * start with 1, everything after the first index
 * is asumed as middle (1 + (i > 0))
 * to find the last we can add one more to the first output 
 * making it 3 (1 + (i > 0) + (i == (cmd_count - 1))).
 * Lastly subtract 2 if there is only one command to make the final output 0: 
 * (1 + (i > 0) + (i == (cmd_count - 1))) - (2 * (cmd_count == 1));
*/
void	grow_baobab(t_minivault	*minivault)
{
	int	i;
	int	cmd_count;

	cmd_count = 1 + count_tokens(PIPE, minivault->tokens);
	minivault->baobab = (t_baobab *)malloc(sizeof(t_baobab));
	minivault->baobab->pipeline = (t_command **)malloc(sizeof(t_command *) \
	* (cmd_count + 1));
	i = 0;
	minivault->baobab->pipeline[cmd_count] = NULL;
	while (i < cmd_count)
	{
		minivault->baobab->pipeline[i] = NULL;
		minivault->baobab->pipeline[i] = split_list(minivault, \
		minivault->tokens, PIPE);
		if (minivault->baobab->pipeline[i] == NULL)
			break ;
		minivault->baobab->pipeline[i]->pos = (1 + (i > 0)
				+ (i == (cmd_count - 1)))
			- (2 * (cmd_count == 1));
		i++;
	}
	open_pipes(minivault, 0, cmd_count);
}
