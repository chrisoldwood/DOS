PrintCap v1.0
*************


1: What is it?
==============

This is an example application to show how to use the NetWare
graphics library I've written. It is not intended as an example of
how to write a NetWare app, and even less an example of how to write
a C program...

It is a clone of the CAPTURE utility, but with a GUI.


2: Why bother?
==============

Can you remeber all those parameters, and don't mind typing in long
command lines?


3: What does it do?
===================

It allows you to set the capture up for LPT ports 1 to 3, so that
they can be captured to a print queue, or not at all. It allows you
to set the form, whether to print a banner, expand tabs and send a
form feed. These are the settings I find that are most commonly
used.


4: Build Notes.
===============

* This is built with a later version of the NetWare graphics library
  than is currently available (v1.1a @ the time of this release).
  This was because I was using this to debug the library.

* The build of the graphics library was a debug one, so the cursor
  will appear at the top-left of the screen so that any debug output
  will be visible. Functions keys F11, F12 and CTRL-F12 will given
  additional status info.


5: Wishlist.
============

- Allow you to change the tab size, banner name and number of copies.
- Add in some more of captures' functionality (e.g. Notify).
- Write some other makefiles.


6: The NetWare Graphics Library.
================================

Nearby you should find the library itself. At the time of writing the
latest version is nwgfx11a.zip (v1.1 alpha). I still have the dialog
stuff to finish. It includes a debug and non-debug version of the
library built with MS c7.

Unfortunately I have yet to document the API, so the source is your
only guide.


7: Warrenty/Distribution.
=========================

Yeah sure! I think you know the score...

Feel free to stick this anywhere you like. Basically, the more people
that see and use it; the more jokes I get! (see below)


8: JokeWare.
============

This software is released under my JokeWare scheme. This means that
if you use this applet at all, you MUST send me your favourite joke.
If you do not have access to email then find someone who does...


9: Bugs/Contacting me.
======================

My email address is cwood@gst-soft.demon.co.uk. 
Any bugs/comments, are welcome; well perhaps not bugs... 


10: Acknowledgments.
====================

GST Technology:- For the use of their machine and tools without which I
would not have been able to write or upload this useless rubbish.

Ian Masters/Steve Sorrell/Paul Shelford/Derek Murrell, all at GST:- for 
testing my applet.

Chris Wood.
12/07/95.
