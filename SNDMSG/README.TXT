SendMsg v1.0
************


1: What is it?
==============

This is an example application to show how to use the NetWare
graphics library I've written. It is not intended as an example of
how to write a NetWare app, and even less an example of how to write
a C program...

It is a broadcast message sending utility, that does the same as SEND
but with a far better interface.


2: Why bother?
==============

It's too laborious typing in long command lines for a number of users
who aren't in a special group!


3: What does it do?
===================

You can either send a message to a number of users or a single group
of users on a file server, i.e. the same as SEND.


4: Notes/Bugs.
==============

* This is built with a later version of the NetWare graphics library
  than is currently available (v1.1a @ the time of this release).
  This was because I was using this to debug the library.

* The build of the graphics library was a debug one, so the cursor
  will appear at the top-left of the screen so that any debug output
  will be visible. Functions keys F11, F12 and CTRL-F12 will given
  additional status info.


5: Wishlist.
============

- Sort the users/groups alphabetically.
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

Ian Masters/Steve Sorrell, all at GST:- for testing my SendMsg applet.

Chris Wood.
10/07/95.
