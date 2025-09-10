/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:00:00 by akovalch          #+#    #+#             */
/*   Updated: 2025/03/18 11:27:52 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static const char	*trim_whitespace(const char *nptr, int *sign)
{
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			*sign = -1;
		nptr++;
	}
	return (nptr);
}

long	ft_atol(const char *nptr)
{
	long	num;
	int		negative;

	num = 0;
	negative = 1;
	nptr = trim_whitespace(nptr, &negative);
	while (*nptr >= '0' && *nptr <= '9')
	{
		if (num > (LONG_MAX - (*nptr - '0')) / 10)
		{
			if (negative == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		num = num * 10 + (*nptr - '0');
		nptr++;
	}
	return (num * negative);
}
