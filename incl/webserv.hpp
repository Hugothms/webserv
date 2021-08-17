/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 11:00:07 by hthomas           #+#    #+#             */
/*   Updated: 2021/08/17 16:38:10 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#if DEBUG_ACTIVE == 1
	#define DEBUG(x) std::cout << x << std::endl;
#else
	#define DEBUG(x)
#endif

#include "includes.hpp"

#endif
