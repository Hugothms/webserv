# TODO LIST

## Unassigned tasks:
- Implement CGI (PHP etc) using fork
- File upload/removal : POST / DELETE (+ get)

## Assigned tasks:

	Enzo:
		- Select, non blocking networking : OK
		- Port management : WIP

	Hugo:
		- HTTP 1.1 Compliant : What is it, and do it
		- Parsing HTTP requests
		- Config file : Define available options, parse the file and apply changes


Issues:
- Sometimes (not systematicly) a broken pipe occurs when requesting big files
- The Requests are created multiple times, with the same parameters.
