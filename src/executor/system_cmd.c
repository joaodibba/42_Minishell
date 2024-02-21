/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalves-c <jalves-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:24:03 by jalves-c          #+#    #+#             */
/*   Updated: 2024/02/21 00:34:06 by jalves-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_list_size(t_word *head)
{
	t_word	*current;
	size_t	size;

	size = 0;
	if (!head)
		return (size);
	current = head;
	while (current)
	{
		size++;
		current = current->next;
	}
	return (size);
}

char	**get_arguments(t_word *words)
{
	int		i;
	int		list_size;
	char	**arguments;

	i = 0;
	arguments = NULL;
	list_size = get_list_size(words);
	arguments = (char **)malloc((list_size + 1) * sizeof(char *));
	if (!arguments)
		return (NULL);
	while (words)
	{
		arguments[i] = words->word;
		words = words->next;
		i++;
	}
	arguments[i] = NULL;
	return (arguments);
}

static void	child_exec(t_minivault *minivault, t_command *command, int in, int out)
{
	char		**arg;
	t_status	status;

	set_signals(SIG_STATE_CHILD);
	arg = get_arguments(command->words);
	status = CMDNOTFOUND;
	dup2(out, STDOUT);
	dup2(in, STDIN);
	close_pipes(in, out);
	close_pipes(command->fd[0], command->fd[1]);
	execve(command->exec_path, arg, minivault->env_list);
	error(minivault, FAILURE, true, command->words->word, ": ", \
		"command not executed", NULL);
	status = get_status_owner_can_execute(command->exec_path);
	free(arg);
	liberate_vector(minivault->input);
	liberation(minivault);
	exit(status);
}

void	system_command(t_minivault *minivault, t_command *command, int in, int out)
{
	if(!command->status)
	{
		command->pid = fork();
		if (command->pid == 0)
		child_exec(minivault, command, in, out);
	}
	close_pipes(in, out);
	set_signals(SIG_STATE_PARENT);
}
