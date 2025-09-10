/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akovalch <akovalch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:44:03 by akovalch          #+#    #+#             */
/*   Updated: 2025/04/14 12:18:48 by akovalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>

static int	ft_puthex(uintptr_t num, int upper)
{
	char	*hex;
	char	*upper_hex;
	int		len;

	hex = "0123456789abcdef";
	upper_hex = "0123456789ABCDEF";
	len = 0;
	if (num >= 16)
		len += ft_puthex((num / 16), upper);
	if (upper)
		len += ft_putchar(upper_hex[num % 16]);
	else
		len += ft_putchar(hex[num % 16]);
	return (len);
}

static int	ft_putpointer(void *ptr)
{
	int	len;

	len = 0;
	if (!ptr)
		return (ft_putstr("(nil)"));
	len += ft_putstr("0x");
	len += ft_puthex((uintptr_t)ptr, 0);
	return (len);
}

static int	ft_putunsigned(unsigned int num)
{
	int	len;

	len = 0;
	if (num == 0)
		return (ft_putchar('0'));
	if (num >= 10)
		len += ft_putunsigned(num / 10);
	len += ft_putchar((num % 10) + '0');
	return (len);
}

static int	check_format(va_list args, const char *format)
{
	int			count_len;

	count_len = 0;
	if (*format == 'c')
		count_len += ft_putchar(va_arg(args, int));
	else if (*format == 's')
		count_len += ft_putstr(va_arg(args, const char *));
	else if (*format == 'd' || *format == 'i')
		count_len += ft_putnbr(va_arg(args, int));
	else if (*format == 'p')
		count_len += ft_putpointer(va_arg(args, void *));
	else if (*format == 'u')
		count_len += ft_putunsigned(va_arg(args, unsigned int));
	else if (*format == 'x')
		count_len += ft_puthex(va_arg(args, unsigned int), 0);
	else if (*format == 'X')
		count_len += ft_puthex(va_arg(args, unsigned int), 1);
	else if (*format == '%')
		count_len += ft_putchar('%');
	return (count_len);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count_len;

	va_start(args, format);
	count_len = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			count_len += check_format(args, format);
		}
		else
			count_len += ft_putchar(*format);
		format++;
	}
	va_end(args);
	return (count_len);
}
