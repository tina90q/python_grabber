from microscope.microscope_move import Move
import tkinter


# start an object for moving the microscope
move = Move()

# start a window
window = tkinter.Tk()
Button = tkinter.Button

# stop button
stop_btn = Button(window, text="stop", command=move.stop)  # sends 0

# make 4 buttons, one for each direction
minus_y_btn = Button(window, text=u"\u2190", command=move.minus_y)  # sends 1
plus_y_btn = Button(window, text=u"\u2192", command=move.plus_y)  # sends 2
plus_x_btn = Button(window, text=u"\u2193", command=move.plus_x)  # sends 3
minus_x_btn = Button(window, text=u"\u2191", command=move.minus_x)  # sends 4

# place buttons
stop_btn.grid(row=1, column=3)
minus_y_btn.grid(row=1, column=0)
plus_y_btn.grid(row=1, column=2)
plus_x_btn.grid(row=2, column=1)
minus_x_btn.grid(row=0, column=1)

# run the window in a loop, or else it will close immediately after opening
window.mainloop()
