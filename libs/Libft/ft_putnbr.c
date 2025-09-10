/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 17:25:15 by akovalch          #+#    #+#             */
/*   Updated: 2025/02/17 18:38:12 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int n)
{
	long	num;
	long	nb;
	int		len;

	num = 0;
	nb = n;
	len = 0;
	if (nb < 0)
	{
		write(1, "-", 1);
		nb *= -1;
		len++;
	}
	if (nb >= 10)
	{
		len += ft_putnbr((nb / 10));
	}
	num = (nb % 10) + '0';
	len += ft_putchar(num);
	return (len);
}
