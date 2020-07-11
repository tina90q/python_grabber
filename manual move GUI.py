from microscope.microscope_move import Move
import tkinter


# start an object for moving the microscope
move = Move()

# start a window
window = tkinter.Tk()
Button = tkinter.Button

# make 4 buttons, one for each direction
left_btn = Button(window, text=u"\u2190", command=move.move_left)
right_btn = Button(window, text=u"\u2192", command=move.move_right)
forward_btn = Button(window, text=u"\u2193", command=move.move_forward)
backward_btn = Button(window, text=u"\u2191", command=move.move_backward)

# place buttons
left_btn.grid(row=1, column=0)
right_btn.grid(row=1, column=2)
forward_btn.grid(row=2, column=1)
backward_btn.grid(row=0, column=1)

# run the window in a loop, or else it will close immediately after opening
window.mainloop()
