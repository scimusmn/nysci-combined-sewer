import sys
from tkinter import *
from tkinter import ttk
import serial


# port = serial.Serial('usb:0/140000/0/1')
port = serial.Serial(sys.argv[1])


class App:
  def __init__(self):
    self.row = 0
    self.root = Tk()
    self.frame = ttk.Frame(self.root, padding=10)
    self.start = self.addSpinner("start", (0,100000))
    self.end = self.addSpinner("end", (0,100000))
    self.bump = self.addSpinner("bump", (-100, 100))
    self.bump.set(10)
    Button(self.frame, command=self.startToEnd, text="start to end").grid(column=1, row=self.row)
    self.frame.pack()

  def addSpinner(self, label, rng):
    Label(self.frame, text=label).grid(column=0, row=self.row)
    lo, hi = rng
    value = IntVar()
    spin = Spinbox(self.frame, from_=lo, to=hi, textvariable=value, command=self.transmit)
    spin.grid(column=1, row=self.row)
    Button(self.frame, command=lambda : self.bumpDownVal(value), text="-bump").grid(column=2, row=self.row)
    Button(self.frame, command=lambda : self.bumpUpVal(value), text="+bump").grid(column=3, row=self.row)
    self.row += 1
    return value

  def bumpUpVal(self, value):
    value.set(value.get() + self.bump.get())
    self.transmit()
  def bumpDownVal(self, value):
    value.set(value.get() - self.bump.get())
    self.transmit()

  def startToEnd(self):
    self.start.set(self.end.get())
    self.transmit()

  def transmit(self):
    string = f"{{setStart:{self.start.get()}}} {{setEnd:{self.end.get()}}}"
    port.write(bytes(string, 'ascii'))

  def mainloop(self):
    self.root.mainloop()
    
app = App()
app.mainloop()

port.close()
print("bye! c:")
