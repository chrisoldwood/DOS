Q-WATCH v1.3
************


1: What is it?
==============

This is an example application to show how to use the NetWare
graphics library I've written. It is not intended as an example of
how to write a NetWare app, and even less an example of how to write
a C program...

It is a mini-pconsole, if you will. But hopefully easier to use but
still reasonably powerful.


2: Why bother?
==============

I wrote a program that allowed you to just watch the jobs in all the
print queues on a server, something pconsole doesn't do. Then I was
asked if I could make it delete jobs and change forms, because our
default form is not 0, something Windows likes assuming. I wrote the
graphics libaray to give my app the Novell look-and-feel.


3: What does it do?
===================

It allows you to monitor print queues on a specific or all servers
you are attached to. You can then see all the jobs in the queue, and
look at them in further detail, changing the form, status and
position. Finally you can see the progress of the job as it is being
printed.

4: Changes since v1.2.
======================

- Fixed bug in print job dialog, where if you selected a job that was
  active, you could only ever get at the Server field on any subsequent
  print jog dialogs.
- Changed the status strings displayed on the printer status dialog.


5: Changes since v1.1.
======================

- You can place the job on hold, or remove a hold.
- You can change its position to be the first or last in the queue.
- You can change the form number of the job.
- You can watch the jobs' progress as it is being printed.

* v1.0 was distributed with my NetWare graphics library v1.0a.


6: Notes/Bugs.
==============

- The number of forms is hard coded to a max of 50, to save on static
  data.
- You may not be able to connect to a print server that you can with
  pconsole. I am looking into this.

* This is built with a later version of the NetWare graphics library
  than is currently available (v1.1a @ the time of this release).
  This was because I thought it was better to get this out.

* The build of the graphics library was a debug one, so the cursor
  will appear at the top-left of the screen so that any debug output
  will be visible. Functions keys F11, F12 and CTRL-F12 will given
  additional status info.


7: Wishlist.
============

- Make the program smaller by removing most of the static data. This
  was only done for convenience.
- Sort the queues alphabetically.
- Write some other makefiles.


8: The NetWare Graphics Library.
================================

Nearby you should find the library itself. At the time of writing the
latest version is nwgfx11a.zip (v1.1 alpha). I still have the dialog
stuff to finish. It includes a debug and non-debug version of the
library built with MS c7.

Unfortunately I have yet to document the API, so the source is your
only guide.


9: Warrenty/Distribution.
=========================

Yeah sure! I think you know the score...

Feel free to stick this anywhere you like. Basically, the more people
that see and use it; the more jokes I get! (see below)


10: JokeWare.
=============

This software is released under my JokeWare scheme. This means that
if you use this applet at all, you MUST send me your favourite joke.
If you do not have access to email then find someone who does...


11: Bugs/Contacting me.
=======================

My email address is cwood@gst-soft.demon.co.uk. 
Any bugs/comments, are welcome; well perhaps not bugs... 


12: Acknowledgments.
====================

GST Technology:- For the use of their machine and tools without which I
would not have been able to write or upload this useless rubbish.

Paul Shelford/Ian Masters/Steve Sorrell, all at GST:- for testing my
QWATCH applet.

Chris Wood.
03/07/95.
