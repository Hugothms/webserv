/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 11:00:07 by hthomas           #+#    #+#             */
/*   Updated: 2021/07/06 12:57:31 by hthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#if DEBUG_ACTIVE == 1
	#define DEBUG(x) std::cout << x << std::endl;
#else
	#define DEBUG(x)
#endif

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <sys/select.h>

#endif
