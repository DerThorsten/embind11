import ems

def py_callback():
  print("the callback fired")


document = ems.val.get_global('document')
button = document.call("createElement", "button")
button['innerHTML'] = 'py created'
body = document['body']

button["onclick"] = ems.js_callback(py_callback)


body.call('appendChild', button)

print(fobar())
