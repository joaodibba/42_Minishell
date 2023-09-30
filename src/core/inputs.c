/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rajphuyal <rajphuyal@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 23:15:36 by rajphuyal         #+#    #+#             */
/*   Updated: 2023/09/30 21:33:10 by rajphuyal        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_readline(void)
{
	rl_initialize();
	rl_bind_key('\t', rl_complete);
	return (1);
}

void	close_readline(void)
{
	clear_history();
}

char	*readaline(void)
{
	char	*line;

	line = readline("minibaiters (⌘ ↩︎) $ ");
	if (line && *line)
		add_history(line);
	return (line);
}

void	handle_input(char *input)
{
	lexer(input);
	// parser();
	// executor();
	// liberation();
}