/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalves-c < jalves-c@student.42lisboa.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:04:34 by rajphuyal         #+#    #+#             */
/*   Updated: 2023/12/14 21:23:46 by jalves-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "inputs.h"
# include "executor.h"

// handy macros
# define GET 0
# define PUT 1
# define ERROR -1
# define STDIN 0
# define STDOUT 1

typedef enum e_status
{
	SUCCESS=0,
	FAILURE=1,
	CMDNOTFOUND=127,
	SIGINTERRUPT=130,
}	t_status;

// this has the data for errors
typedef struct s_error
{
	t_status	status;
	char		*message;
	char		*err_token;
}	t_error;

// the env list
typedef struct s_envs
{
	char			*key;
	char			*value;
	struct s_envs	*next;
}	t_envs;

// the minieverything
typedef struct s_minivault
{
	char		**input;
	char		**path;
	char		**builtin_list;
	char		**env_list;
	t_envs		*envs;
	t_baobab	*baobab;
	t_token		*tokens;
	t_error		*error;
}	t_minivault;

t_minivault	*minishell(void);

// minishell
int			init_minivault(t_minivault *minivault, char **envs);

// input functions
char		*readaline(void);
void		close_readline(void);
void		handle_input(t_minivault *minivault, char *input, char **envs);

// error handeler
void    error(t_minivault *minivault, t_status status, char *message, char *token);

// environment functions
char		**envsort(t_envs *envs);
char		*get_env(t_minivault *minivault, char *key);
void		unset_env(t_minivault *minivault, char *key);
t_envs		*add_env_node(t_envs *envs, char *key, char *value);
void		set_env(t_minivault *minivault, char *key, char *value);

// debug functions
void		call_debug(t_minivault *minivault);
void		print_argv(int argc, char **argv);
void		print_envs(t_envs *envs);
void		print_vector(char **vector);
void		print_tokens(t_token *head);
void		print_baobab(t_baobab *root, int indent_level);

// the liberator
void		liberation(t_minivault *minivault);
void		liberate_envs(t_envs *head);
void		liberate_vector(char **vector);
void		liberate_tokens(t_token *head);
void		liberate_baobab(t_baobab *head);

// utils
void    	clean_exit(t_minivault *minivault, int status);

#endif