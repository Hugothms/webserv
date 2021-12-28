/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 18:55:27 by edal--ce          #+#    #+#             */
/*   Updated: 2021/12/28 19:05:33 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "includes.hpp"

#ifndef LOG_HPP
#define LOG_HPP

class Log
{
	public :
	Log(std::string msg)
	{
		std::time_t t = std::time(0);   // get time now
    	std::tm* now = std::localtime(&t);
   	 	
   	 	std::cout << "\033[31m[";

   	 	std::cout << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-'
         <<  now->tm_mday << " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "] " << msg
         << "\n";
	}
};
#endif