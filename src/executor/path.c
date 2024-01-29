#include "minishell.h"

bool file_exists(const char *path)
{
    return (access(path, F_OK) == 0);
}

bool file_is_executable(const char *path) 
{
    return (access(path, X_OK) == 0);
}

bool	print_check_path(t_minivault *minivault, char *path)
{
	struct stat statbuf;

    if (!file_exists(path)) 
	{
		error(minivault, CMDNOTFOUND, true, path, ": ", "No such file or directory", NULL); // ? check this later
		return (false);
	}
    if (!file_is_executable(path)) 
	{
		error(minivault, CMDNOTFOUND, true, path, ": ", "Permission denied", NULL);
        return (false); // Not executable
	}
    if (stat(path, &statbuf) == 0 && ft_strchr(path, '/')) 
	{
        if (S_ISDIR(statbuf.st_mode))
		{
			error(minivault, CMDNOTFOUND, true, path, ": ", "Is a directory", NULL);
            return (false); // It's a directory, bro
		}
		else if (S_ISREG(statbuf.st_mode))
			return (true); // its a regular file, good to go
    }
	error(minivault, CMDNOTFOUND, true, path, ": ", "No such file or directory", NULL);
    return (false); // 
}

bool	check_path(char *path)
{
	struct stat statbuf;

    if (!file_exists(path)) 
		return (false);
    if (!file_is_executable(path)) 
        return (false); // Not executable
    if (stat(path, &statbuf) == 0 && ft_strchr(path, '/')) 
	{
        if (S_ISDIR(statbuf.st_mode))
            return (false); // It's a directory, bro
		else if (S_ISREG(statbuf.st_mode))
			return (true); // its a regular file, good to go
	}
    return (false); // 
}

char *search_file_in_path(char **path_list, char *command) 
{
    char *full_path;
    char *path_with_slash;
    int i = 0;

    while (command && path_list && path_list[i]) 
	{
        path_with_slash = ft_strjoin(path_list[i], "/");
		if (!path_with_slash) // print_error("Memory allocation failed");
			return (NULL);
        full_path = ft_strjoin(path_with_slash, command);
		free(path_with_slash);
		if (!full_path) // print_error("Memory allocation failed");	
			return (NULL);
        if (check_path(full_path))
        	return (full_path);
        free(full_path);
        i++;
    }
    return (NULL);
}

char *handle_path(t_minivault *minivault, char *cmd)
{
	char *path_to_cmd;
	char *env_path;
	char **paths;

	if (!cmd)
		return (NULL);
	env_path = get_env(minivault, "PATH");
	if (env_path)
	{
		paths = ft_split(env_path, ':');
		path_to_cmd = search_file_in_path(paths, cmd);
		if (path_to_cmd)
		{
			liberate_vector(paths);
			return (path_to_cmd);
		}
		liberate_vector(paths);
	}
	if (check_path(cmd))
		return (cmd);
	print_check_path(minivault, cmd);
	return (NULL);
}