#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <unistd.h>

void	executor(t_minivault *minivault, t_command **pipeline);

void	config_io(t_minivault	*minivault, t_command *command, int pos);

t_redir *get_last_token(t_redir *head);
void	system_command(t_minivault *minivault, t_command *command, int pos);

#endif