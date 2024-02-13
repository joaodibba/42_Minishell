#include "minishell.h"

static	void	_cleaner(t_strexp *data)
{
	data->quoted = false;
	data->singleq = false;
	data->expandable = false;
	data->pos = NULL;
}

static void	_exp_validator(char *str, t_strexp *data)
{
	char		*end;

	end = str;
	while (end && *(end + 1))
		end++;
	while (str && *str && *str != DOLLAR && *str != '\'' && *str != '"')
		str++;
	if (*str == DOLLAR)
		unpack_var(data, false, false, true);
	if (*str == *end && *end == '\'')
	{
		unpack_var(data, true, true, false);
		return ;
	}
	else if (*str == *end && *end == '"' && *(str + 1) == '\'')
		unpack_var(data, true, true, true);
	else if (*str == *end && *end == '"')
		unpack_var(data, true, false, true);
	data->expandable = true;
}

static	void	_put_end_break(char *start, t_strexp *data)
{
	char	*end;

	end = start;
	while (end && *(end + 1))
		end++;
	if (data->quoted && *(start + 1) != PREVEXITSTAT[FIRST_ELEM])
		*(end - (data->singleq)) = '\0';
}

static	char	*alchemy(t_minivault *minivault, t_strexp *data, char *start)
{
	char			temp;
	char			*value;
	char			*_built;
	static	char	*suffix = NULL;

	data->pos = start;
	start++;
	temp = '\0';
	suffix = NULL;
	while (start && *start)
	{
		if (!*(start + 1) || *start == DOLLAR || \
			*start == '\'' || *start == '"')
		{
			if (*(start + 1))
			{
				temp = *start;
				*start = '\0';
			}
			if (data->pos && *(data->pos + 1) == PREVEXITSTAT[FIRST_ELEM])
			{
				suffix = get_suffix(data->pos + 2);
				value = get_env(minivault, PREVEXITSTAT);
			}
			else if (data->pos)
				value = get_env(minivault, data->pos + 1);
			if (value)
			{
				if (*(start) == '\'' && *(data->pos - 1) == '\'')
				{
					if (!_built)
						_built = exe_concat(_built, value, suffix, NULL);
					else
						_built = exe_concat(_built, "'", \
							_built, "'", value, suffix, NULL);
				}
				if (!_built)
					_built = exe_concat(_built, "", value, suffix, NULL);
				else
					_built = exe_concat(_built, _built, value, suffix, NULL);
				if (suffix)
				{
					free(suffix);
					suffix = NULL;
				}
			}
			if (temp)
			{
				*start = temp;
				temp = '\0';
			}
			if (*start == DOLLAR)
				data->pos = start;
			else
				data->pos = NULL;
		}
		start++;
	}
	if (!_built)
		_built = exe_concat(NULL, PLACEHOLDER, NULL);
	else if (_built && data->singleq)
		_built = exe_concat(_built, "'", _built, "'", NULL);
	return (_built);
}

void	strexpand(t_minivault *minivault, char **vector)
{
	t_strexp	data;
	char		*s_iter;
	char		*_magic;
	char		**v_iter;

	v_iter = vector;
	while (v_iter && *v_iter)
	{
		s_iter = *v_iter;
		_cleaner(&data);
		_exp_validator(s_iter, &data);
		if (_check_heredoc_deli(s_iter, vector))
		{
			while (s_iter && *s_iter)
			{
				if (*s_iter == DOLLAR && data.expandable)
				{
					if (!*(s_iter + 1))
						break ;
					_put_end_break(start, data);
					_magic = alchemy(minivault, &data, s_iter);
					if (!_magic)
						break ;
					*(s_iter - (data.quoted + data.singleq)) = '\0';
					_magic = exe_concat(_magic, *v_iter, _magic, NULL);
					free(*v_iter);
					*v_iter = _magic;
					break ;
				}
				s_iter++;
			}
		}
		v_iter++;
	}
}
