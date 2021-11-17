#!/usr/bin/env python3
import cgi, cgitb
cgitb.enable()
input_data = cgi.FieldStorage()
# name = input_data.getvalue('fname')
# print("Name of the user is:",name)


print('Content-Type: text/html')	# HTML is following
print('')							# Leave a blank line
print('<h1>Input data</h1>')
name = input_data["name"].value
email = input_data["email"].value
print('<output>{0} and {1}</output>'.format(name, email))

# cgi.parse()

# http://www.cgi101.com/book/ch3/text.html
# DOCUMENT_ROOT # ROOT DIRECTORY OF THE SERVER
# HTTP_HOST # HOST NAME OF THE PAGE
# SCRIPT_FILENAME #FULL PATH OF THIS SCRIPT
# SCRIPT_NAME #NAME OF THIS SCRIPT
# HTTP_USER_AGENT #BROWSER TYPE OF THE VISITOR
# HTTPS #SECURE CONNECTION
# PATH #SYSTEM PATH THE SERVER IS RUNING UNDER
