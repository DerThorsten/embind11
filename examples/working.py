import js

def py_callback():
  print("the callback fired")

button = js.document.createElement("button")
button['innerHTML'] = 'py created'
button["onclick"] = js.js_callback(py_callback)
js.document.body.appendChild(button)