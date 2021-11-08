#!/usr/bin/env python3
import cgi,cgitb
cgitb.enable()
input_data = cgi.FieldStorage()
name = input_data.getvalue('fname')
print("Name of the user is:",name)


print('Content-Type: text/html')	# HTML is following
print('')							# Leave a blank line
print('<h1>Input data</h1>')
name = input_data["name"].value
email = input_data["email"].value
print('<output>{0} and {1}</output>'.format(name, email))