ChatIPX v1.0
************


1: What is it?
==============

This is an example application to show how to use the NetWare
graphics library I've written. It is not intended as an example of
how to write a NetWare app, and even less an example of how to write
a C program...

It is a 2-user chat utlity that uses IPX as the transport system.


2: Why bother?
==============

I wanted to start learning how to use IPX and the Novell chat utility
is fairly basic, so I thought i'd spice it up a bit.


3: What does it do?
===================

You can select a user to chat with, and it will send a broadcast
message every 20 seconds to let the user know. The other user accepts
the call and you start chatting. When you want to terminate press ESC
and the session will end.


4: Notes/Bugs.
==============

- It only shows a list of users for the primary connection ID. If the
  user is not logged into this server it wont see them.
- It only picks the first connection for the user on this server. If
  the user is logged in more than once you wont be able to contact them
  on any other connection.
  
* This is built with a later version of the NetWare graphics library
  than is currently available (v1.1a @ the time of this release).
  This was because I was using this to debug the library, and add a few
  more minor functions.

* The build of the graphics library was a debug one, so the cursor
  will appear at the top-left of the screen so that any debug output
  will be visible. Functions keys F11, F12 and CTRL-F12 will given
  additional status info.


5: Wishlist.
============

- Sort the users/groups alphabetically.
- Allow a choice of server from which to select the user.
- Allow a choice of connection number when the user is logged in more
  than once.
- Show the callers connection number in the broadcast message.


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

Steve Sorrell at GST:- For testing my ChatIPX applet.

Novell Inc:- For the sample ipxchat utility which I used as the basis for
this applet.

Chris Wood.
17/07/95.
