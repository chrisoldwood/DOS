NetWare Graphics Library v1.2
*****************************


1: What is it?
==============

This is library for creating NetWare style DOS applications. It
supposedly takes away the effort of writing a GUI that looks and
behaves like a NetWare app.

It supports colour and mono text modes only because of the direct
screen access. It will detect which sort of display and handle the
differences automagically.

This is the first proper release as I hope that the API should be
stable by now. The only area that might change is the dialog code
because I have not written all the controls.


2: Why bother?
==============

I wrote a application to monitor the printer queues, then thought it
would be fun to make it look a bit more NetWare-ish. Then decided it
would be even more fun to write a library so that all my apps looked
NetWare-ish.


3: What does it do?
===================

- Creates and handles popup menus with multiple selection, timer
  callbacks, scrolling and selection by typing the item.
- Creates and handles dialog boxes and their controls.
- Displays things in the correct colours(!)
- Allows basic stacked "windows".
- Handles F1 for on-line help and handles the on-line help display.
- Handles ALT-F10 to exit, including the menu.
- The washing up (I wish).


4: Changes since v1.1 (Alpha).
==============================

- Fixed a bug in the menu "typed selection" code.
- Menus & dialogs now remember the current item highlighted.
- You can now choose which item to have highlighted for menus & dialogs.
- The Exit menu has the yes & no options the right way round.
- You can choose which option is highlighted in a QueryMenu.
- Menus & dialogs now have a help section field built in.
- The menu display code has be cleaned up.
- Fixed a bug in the calculation of -ve relative window positions.
- Added new function to disable help.
- Stopped repetative showings of the debug info messages.


5: Changes since v1.0 (Alpha).
==============================

- Removed all assembler.
- Created my own data types for everything.
- Made it more compiler portable.
- Added debug code for all the low-level functions.
- Added some debug functions for checking the stack and heap.
- Wrote makfiles for Borland, Watcom and NMAKE.
- Changed a number of API calls because of the dialog handler.
- Finished off the menu handler.
- Added in basic window create/destroy functions.
- Added a generic query menu for yes/no answers.


6: Still to do.
===============

v1.3
- Finish off the other dialog controls.
- Document the API; do I have to mum?.

v?.? (Eventually)
- Write some common dialogs (e.g. selecting users, servers etc.)
- Write some C++ classes for common objects (file servers etc.)


7: Example applet.
==================

Nearby you should find a program called QWATCH, this is the applet
that I am using as a testbed for this library. It is a simple
pconsole replacement, but with a more logical UI; I hope.


8: Warrenty/Distribution.
=========================

Yeah sure! I think you know the score...

Feel free to stick this anywhere you like. Basically, the more people
that see and use it; the more jokes I get! (see below)


9: JokeWare.
============

This software is released under my JokeWare scheme. This means that
if you use this library at all, you MUST send me your favourite joke
to the email address below. More than one joke will only be accepted
if they are of reasonable quality!

If you do not have access to email then find someone who does...


10: Bugs/Contacting me.
======================

My email address is cwood@gst-soft.demon.co.uk.

Any bugs/comments, no matter how unimportant, are welcome; 
well perhaps not bugs... I would especially like feedback on the
makefiles, to make sure they are correct.


11: Acknowledgments.
===================

GST Technology:- For the use of their machine and tools without which I
would not have been able to write or upload this useless rubbish.

Paul Shelford/Ian Masters/Steve Sorrell (at GST):- for testing my
QWATCH applet, and hence the library.

Stephen Mott (sam1@cix.compulink.co.uk) for some pointers to help me
write the Borland makefile.

Chris Wood.
31/05/95.
