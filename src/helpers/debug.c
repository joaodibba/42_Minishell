/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalves-c < jalves-c@student.42lisboa.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 23:05:14 by rajphuyal         #+#    #+#             */
/*   Updated: 2024/01/25 19:06:11 by jalves-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_argv(int argc, char **argv)
{
	int	i;

	i = 0;
	printf("argc: %d\n", argc);
	while (i < argc)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
		i++;
	}
}

void	print_vector(char **vector)
{
	int	i;

	i = 0;
	while (vector[i])
	{
		printf("%d: %s\n", i, vector[i]);
		i++;
	}
}

void	print_envs(t_envs *envs)
{
	while (envs)
	{
		printf("[ (key): %s | (value): %s | (internal): %d | (session): %d ]\n",
				envs->key, envs->value, envs->internal, envs->session);
		envs = envs->next;
	}
}

void    print_tokens(t_token *head)
{
    while (head)
    {
        printf("TOKEN: %s | TYPE: %d\n", head->content, head->type);
        head = head->next;
    }
}