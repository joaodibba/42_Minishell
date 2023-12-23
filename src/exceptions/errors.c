# include "minishell.h"

void    error(t_minivault *minivault, t_status status, char *message, char *token)
{
    minivault->error->status = status;
    minivault->error->message = message;
    minivault->error->err_token = token;
    printf("minibaiters: %s: %s\n", message, token);
}
