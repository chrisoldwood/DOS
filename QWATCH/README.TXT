Q-WATCH v1.1 (alpha7)
*********************


1: What is it?
==============

This is an example application to show how to use the NetWare
graphics library I've written. It is not intended as an example of
how to write a NetWare app, and even less an example of how to write
a C program...

It is a mini-pconsole, if you will. But hopefully easier to use.

This is still only alpha as I've still some work left. I don't expect
anyone is actually interested, but thought I'd send this out just
in case. (Ever hopeful!).


2: Why bother?
==============

I originally wrote a program to monitor the printer queues, and
decided to write a NetWare library for fun, to make my app more like
Novells'. I am now using this applet as my testbed.


3: What does it do?
===================

It allows you to monitor print queues on a specific or all servers
you are attached to. You can then see all the jobs in the queue, and
look at them in further detail, or delete them. Eventually you will
be able to change a jobs attributes and look at the print server
status.


4: Still to do.
===============

- Allow the jobs settings to be changed. (You can't actually take a
  job off hold, although it may look like it.
- Look at the status of the print server, servicing a selected job.


5: The Build.
=============

This was built with a debug version of the library, so all the
library functions parameters' are verified, and any errors are reported
on the top line. You will see the cursor flashing in the top corner to
notify you of the build type; this also allows any run-time errors to
be displayed. A couple of other useful features that are built in the
debug version is the ability to check the stack and heap space, and
integrity. F-11, F-12 and CTL-12 will show this info at any time.

When you build with a nondebug version of the library these features
automatically disappear and the cursor is hidden.


6: The NetWare Graphics Library.
================================

Nearby you should find the library itself. At the time of writing the
latest version is nwgfx11a.zip (v1.1 alpha). I still have the dialog
stuff to finish. It includes a debug and non-debug version of the
library built with MS c7.

Unfortunately I have yet to document the API, so this is your only
guide.


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

Any bugs/comments, no matter how unimportant, are welcome; 
well perhaps not bugs... I would be grateful if someone could
knock-up some makefiles for other compilers, I've done it for the
library, but not for this applet.


10: Acknowledgments.
===================

GST Technology:- For the use of their machine and tools without which I
would not have been able to write or upload this useless rubbish.

Paul Shelford/Ian Masters/Steve Sorrell, all at GST:- for testing my
QWATCH applet.

Chris Wood.
31/05/95.
