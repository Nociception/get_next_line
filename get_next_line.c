/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nstoutze <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 14:27:29 by nstoutze          #+#    #+#             */
/*   Updated: 2022/12/13 18:16:45 by nstoutze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	icis(char *s, char c)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if (s[i] == c)
			return (i);
	}
	return (i);
}

static void	janitor(char **dust)
{
	if (*dust)
	{
		free(*dust);
		*dust = NULL;
	}
}

static char	*read_fd(char *proto, int fd)
{
	char	buffer[BUFFER_SIZE + 1];
	int		nb_read_bytes;
	int		i;

	i = -1;
	while (++i <= BUFFER_SIZE)
		buffer[i] = '\0';
	nb_read_bytes = 1;
	while (!ft_strchr(buffer, '\n') && nb_read_bytes)
	{
		nb_read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (nb_read_bytes == -1 || (!nb_read_bytes && !proto))
		{
			janitor(&proto);
			return (0);
		}
		buffer[nb_read_bytes] = '\0';
		if (proto)
			proto = ft_strjoin(proto, buffer);
		else
			proto = ft_strdup(buffer);
	}
	return (proto);
}

static char	*gen_line(int fd, char *proto, char **tank)
{
	char	*line;

	line = read_fd(proto, fd);
	proto = ft_strdup(line);
	janitor(&line);
	if (proto && ft_strchr(proto, '\n'))
	{
		line = ft_substr(proto, 0, icis(proto, '\n') + 1);
		if (ft_strlen(line) != ft_strlen(proto))
		{
			*tank = ft_substr(proto, icis(proto, '\n') + 1, ft_strlen(proto));
			janitor(&proto);
			return (line);
		}
		else
		{
			janitor(&line);
			return (proto);
		}
	}
	else if (proto)
		return (proto);
	janitor(&proto);
	return (0);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*proto;
	static char	*tank = NULL;

	proto = NULL;
	if (!tank)
		return (gen_line(fd, proto, &tank));
	else
	{
		if (ft_strchr(tank, '\n'))
		{
			line = ft_substr(tank, 0, icis(tank, '\n') + 1);
			proto = ft_substr(tank, icis(tank, '\n') + 1, ft_strlen(tank));
			janitor(&tank);
			tank = ft_strdup(proto);
			janitor(&proto);
			return (line);
		}
		else
		{
			proto = ft_strdup(tank);
			janitor(&tank);
			return (gen_line(fd, proto, &tank));
		}
	}
}
/*
int main()
{
	int fd = open("test.txt", O_RDONLY);
	int n = 0;
	while (n++ < 20)
	{
		printf("%s", get_next_line(fd));
	}
	//system("leaks a.out");
	close(fd);
	return 0;
}
*/