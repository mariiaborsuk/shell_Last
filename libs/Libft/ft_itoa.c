/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:38:29 by akovalch          #+#    #+#             */
/*   Updated: 2025/02/17 18:36:13 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbr_length(int nb)
{
	int	length;

	length = 0;
	if (nb <= 0)
		length++;
	while (nb != 0)
	{
		nb = nb / 10;
		length++;
	}
	return (length);
}

static void	convert(int nb, int length, char *str)
{
	long	nb1;

	nb1 = nb;
	str[length] = '\0';
	if (nb1 < 0)
	{
		str[0] = '-';
		nb1 *= -1;
	}
	while (nb1 > 0)
	{
		length--;
		str[length] = (nb1 % 10) + '0';
		nb1 /= 10;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		length;

	if (n == 0)
		return (ft_strdup("0"));
	length = nbr_length(n);
	str = malloc((length + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	convert(n, length, str);
	return (str);
}
