#!/usr/bin/env python
import cgi, cgitb
cgitb.enable()
input_data = cgi.FieldStorage()
# name = input_data.getvalue('fname')
# print("Name of the user is:",name)

name = input_data["name"].value
email = input_data["email"].value
print(	"<html lang=\"en\">\n\
			<body style=\"background-color: darkslateblue; color: lightgrey;\">\n\
				<div style=\"display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;\">\n\
					<h1>form.py</h1>\n\
					<p>name: {0}</br>email: {1}</p>\n\
				</div>\n\
			</body>\n\
		</html>".format(name, email))

# http://www.cgi101.com/book/ch3/text.html
# DOCUMENT_ROOT # ROOT DIRECTORY OF THE SERVER
# HTTP_HOST # HOST NAME OF THE PAGE
# SCRIPT_FILENAME #FULL PATH OF THIS SCRIPT
# SCRIPT_NAME #NAME OF THIS SCRIPT
# HTTP_USER_AGENT #BROWSER TYPE OF THE VISITOR
# HTTPS #SECURE CONNECTION
# PATH #SYSTEM PATH THE SERVER IS RUNING UNDER
