import js

def py_callback():
  print("the callback fired")

# create a button
button = js.document.createElement("button")

# change text and add callback
button['innerHTML'] = 'py created'
button['onclick'] = js.js_callback(py_callback)

# add the button to the document
js.document.body.appendChild(button)

