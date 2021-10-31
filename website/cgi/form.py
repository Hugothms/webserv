import cgi,cgitb
cgitb.enable()
form = cgi.FieldStorage()
name = form.getvalue('fname')
print("Name of the user is:",name)