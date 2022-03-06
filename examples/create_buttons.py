import ems

def py_callback():
  print("the callback fired")
  
def js_callback(py_function):
  js_py_object = ems.val.py_object(py_function)
  return js_py_object['__call__'].bind(js_py_object)

document = ems.val.get_global('document')
button = document.call("createElement", "button")
button['innerHTML'] = 'py created'
body = document['body']

button["onclick"] = js_callback(py_callback)


body.call('appendChild', button)