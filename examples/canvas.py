import js

# get the canvas
canvas = js.document.getElementById('mycanvas')
ctx = canvas.getContext("2d")

# bounding box / topleft
boundings = canvas.getBoundingClientRect()
top_left = boundings.left.pythonize(), boundings.top.pythonize()

# is the mouse down?
is_down = [False]

def mouse_down(e):
    x = e.x.pythonize() - top_left[0]
    y = e.y.pythonize() - top_left[1]
    is_down[0] = True
    ctx.moveTo(x, y);

def mouse_move(e):
    if is_down[0]:
        x = e.x.pythonize() - top_left[0]
        y = e.y.pythonize() - top_left[1]
        ctx.lineTo(x, y);
        ctx.stroke();

def mouse_up(e):
    is_down[0] = False

canvas['onmousedown'] = js.js_callback(mouse_down)
canvas['onmousemove'] = js.js_callback(mouse_move)
canvas['onmouseup'] = js.js_callback(mouse_up)
