In this config file we should be able to
	choose the port and host of each "server"
	setup the server_names or not
	The first server for a host:port will be the default for this host:port (meaning it will answer to all request that doesn’t belong to an other server)
	setup default error pages
	limit client body size
	setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):
		define a list of accepted HTTP Methods for the route
		define an HTTP redirection.
		define a directory or a file from where the file should be search (for example if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet)
		turn on or off directory listing
		default file to answer if the request is a directory
		execute CGI based on certain file extension (for example .php)
			You wonder what a CGI is ?
			Because you won’t call the CGI directly use the full path as PATH_INFO
			Just remembers that for chunked request, your server needs to unchunked it and the CGI will expect EOF as end of the body.
			Same things for the output of the CGI. if no content_length is returned from the CGI, EOF will mean the end of the returned data.
			Your program should call the cgi with the file requested as first argument
			The cgi should be run in the correct directory for relativ path file access
			Your server should work with one CGI (php-cgi, python...)
		Make the route able to accept uploaded files and configure where it should be saved