# This code shows a screen with the live image from the first camera in your PC.
# We add to the graph two filters: one is a source filter corresponding to the first camera connected to your PC,
# the second is the default render, that shows the images from the camera in a window on the screen.
# Then we call prepare, that connects the two filters together, and run, to execute the graph.
# Finally, we need a method to pause the program while watching the camera video.
# I use the Tkinter mainloop function which fetches and handles Windows events, so the application does't seem frozen.

from pygrabber.dshow_graph import FilterGraph
from tkinter import Tk

graph = FilterGraph()  # initiate an object of image filter
graph.add_video_input_device(0)  # set up video input from the first camera indexed 0 (add source filter)
graph.add_default_render()  # add the default render filter to the image signals
graph.prepare_preview_graph()  # combine two filters
graph.run()  # execute the graph
root = Tk()
# root.withdraw()  # hide Tkinter main window
root.mainloop()