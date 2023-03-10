/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joohekim <joohekim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 15:26:50 by joohekim          #+#    #+#             */
/*   Updated: 2022/12/29 20:56:43 by joohekim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include <stdio.h>

// int BUFFER_SIZE = 1;

static t_gnl_node	*new_fd(int fd)
{
	t_gnl_node	*new;

	new = (t_gnl_node *)malloc(sizeof(t_gnl_node));
	if (new == NULL)
		return (NULL);
	new->fd = fd;
	new->backup = ft_strdup("");
	if (!new->backup)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

static t_gnl_node	*find_fd(int fd, t_gnl_list *list)
{
	while (list->cur != NULL)
	{
		if (list->cur->fd == fd)
			return (list->cur);
		list->pre = NULL;
		list->pre = list->cur;
		list->cur = list->cur->next;
	}
	list->pre = NULL;
	list->cur = new_fd(fd);
	if (list->pre != NULL)
		list->pre->next = list->cur;
	if (!list->cur)
		return (NULL);
	return (list->cur);
}

static char	*read_line(t_gnl_list *list, int *cnt)
{
	char	buf[BUFFER_SIZE + 1];
	int		i;
	char	*temp;

	buf[0] = '\0';
	if (list->cur->backup[0] == '\0')
	{
		*cnt = read(list->cur->fd, buf, BUFFER_SIZE);
		if (*cnt == 0)
			return (NULL);
	}
	while (*cnt > -1)
	{
		if (*cnt == 0)
			return (list->cur->backup);
		buf[*cnt] = '\0';
		temp = ft_strjoin(list->cur->backup, buf);
		free(list->cur->backup);
		list->cur->backup = temp;
		i = 0;
		while (list->cur->backup[i])
		{
			if (list->cur->backup[i] == '\n')
				return (list->cur->backup);
			i++;
		}
		*cnt = read(list->cur->fd, buf, BUFFER_SIZE);
	}
	return (NULL);
}

static char	*substr_line(t_gnl_list *list)
{
	char	*line;
	char	*temp;
	size_t	len;
	int		i;

	i = 0;
	while (list->cur->backup[i] && list->cur->backup[i] != '\n')
		i++;
	if (!list->cur->backup[i])
		return (list->cur->backup);
	line = ft_substr(list->cur->backup, 0, i + 1);
	if (!line)
		return (NULL);
	len = ft_strlen(list->cur->backup);
	temp = ft_substr(list->cur->backup, i + 1, len);
	if (!temp)
		return (NULL);
	free(list->cur->backup);
	list->cur->backup = ft_strjoin("", temp);
	free(temp);
	return (line);
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*list;
	char				*line;
	char				*temp;
	int					cnt;

	cnt = 1;
	if (fd < 0)
		return (NULL);
	if (list == NULL)
	{
		list = (t_gnl_list *)malloc(sizeof(t_gnl_list));
		if (!list)
			return (NULL);
		list->cur = NULL;
	}
	if (!(find_fd(fd, list)))
	{
		free(list);
		list = NULL;
		return (NULL);
	}
	if (!(read_line(list, &cnt)))
	{
		free(list->cur->backup);
		free(list->cur);
		free(list);
		list = NULL;
		return (NULL);
	}
	line = substr_line(list);
	// printf("%p\n", list);
	// printf("%p\n", list->cur);
	if (cnt == 0)
	{
		temp = substr_line(list);
		if (!temp)
			return (NULL);
		line = ft_strdup(temp);
		del_fd(list);
	}
	return (line);
}

// #include <fcntl.h>
// #include <stdio.h>
// #include "get_next_line_utils.c"

// int main(void)
// {
// 	char *result = "";
// 	int fd[4];
// 	fd[0] = open("files/41_with_nl", O_RDWR);
// 	fd[1] = open("files/42_with_nl", O_RDWR);
// 	fd[2] = open("files/43_with_nl", O_RDWR);
// 	fd[3] = open("files/nl", O_RDWR);
// 	result = get_next_line(1000);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[0]);
// 	printf("%s\n", result);
	
// 	result = get_next_line(1001);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[1]);
// 	printf("%s\n", result);
	
// 	result = get_next_line(1002);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[2]);
// 	printf("%s\n", result);
	
// 	result = get_next_line(1003);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[0]);
// 	printf("%s\n", result);
	
// 	result = get_next_line(1004);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[1]);
// 	printf("%s\n", result);
	
// 	result = get_next_line(1005);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[2]);
// 	printf("%s\n", result);
	
// 	result = get_next_line(fd[0]);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[1]);
// 	printf("%s\n", result);
// 	result = get_next_line(fd[2]);
// 	printf("%s\n", result);
	
// 	system("leaks --list -- a.out");
// }

