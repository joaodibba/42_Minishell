#ifndef INPUTS_H
# define INPUTS_H

# include <readline/history.h>
# include <readline/readline.h>

# define WRITE 1
# define READ 0
# define DOLLAR '$'
# define PLACEHOLDER "\31"

// TOKEN LIST
typedef enum e_content_type
{
	LITERAL,
	QUOTED,
	PIPE,
	REDIRECTION,
}				t_content_type;

typedef struct s_token
{
	char			*content;
	t_content_type	type;
	struct s_token	*next; // used initially to build the list
}	t_token;

// AST
// used to identify the redirection node
typedef enum e_operation
{
	GREAT,
	LESS,
	DGREAT,
	DLESS
}			t_operation;

typedef struct s_redir
{
	t_operation		operator;
	int				fd;
	char			*word;
	struct s_redir	*next;
}					t_redir;

// declaration for expansion
typedef struct s_strexp
{
	char	*pos;
	bool	quoted;
	bool	singleq;
	bool	expandable;
}			t_strexp;

/*
TODO 	havig the node called word and the string
TODO 	word aswell is confusing this could change
*/
typedef struct s_word
{
	char			*word;
	struct s_word	*next;

}					t_word;

typedef enum e_cmd_pos
{
	SINGLE,
	FIRST,
	MIDDLE,
	LAST
}			t_cmd_pos;

typedef struct s_command
{
	t_cmd_pos	pos;
	t_word		*words;
	t_redir		*redir_in;
	t_redir		*redir_out;
}				t_command;

typedef struct s_baobab
{
	t_command	**pipeline;
	int			pipe_fd[FOPEN_MAX][2];
}				t_baobab;

typedef struct s_minivault	t_minivault;

// lexing
bool		is_single_quote(char c);
bool		is_double_quote(char c);
t_token		*create_new(char *token);
bool		lexer(t_minivault *minivault, char *input);
void		tokenizer(t_minivault *minivault);
void		remove_token(t_token *head, t_token *node);
void		add_token(t_minivault *minivault, char *token);
void		strextract(t_minivault *minivault, char *input);
void		strexpand(t_minivault *minivault, char **vector);

// expansion utils
char		*get_suffix(char *pos);
bool		_check_heredoc_deli(char *str, char **vector);
bool		unpack_var(t_strexp *data, bool quoted, bool singleq, bool expandable);

// parser
void		remove_quotes(char *str);
void		grow_baobab(t_minivault *minivault);
t_baobab	*search(t_baobab *root, char *token);
void		add_word(t_word **word_list, t_token *token);
int			count_tokens(t_content_type token_type, t_token *tokens);
void		connector(t_baobab *node, t_baobab *parent, \
			t_baobab *left, t_baobab *right);
void		add_redirection(t_minivault *minivault, \
			t_command **command, t_token *token, t_token *next);

#endif