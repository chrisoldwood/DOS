# DOS Tools

These are a bunch of DOS tools I wrote during my first professional programming
job at GST Software Products. They are all written in C and can be built with
MSC 600 / Visual C++ 1.52. The source and binaries were originally shared via
the [CiX](https://en.wikipedia.org/wiki/CIX_(website)) conferencing service back
in the mid-1990s.

### CHTIPX - IPX Based Chat Utility

This is a GUI based peer-to-peer two-way chat utility that uses IPX. It shows
you a list of users on the network to whom you can chat with, and then
establishes a connection and uses a split screen view for the conversation.

### COMPORTS - COM Ports Lister

This is a simple program that just lists the COM ports section of the BIOS Data
Area.

### ISRCODE - IPX ESR Handler Example

A snippet showing how to write an ESR handler for IPX messages.

### MTOSSUDG - PMail User-Defined Gateway

This is a User-Defined Gateway for PMail/WinPMail that converts emails in
Internet format to PMail format. It takes two command line parameters, the email
message file and the recipient's name and writes out the translated message to a
folder specified in the config file.

### NWGFXLIB - NetWare Graphics Library

This is the graphics library used by all my NetWare applications to give them
the look and feel of the real DOS NetWare tools. It provides functions to create
and manipulate windows, popup menus, message boxes and dialogs.

### PRTCAP - CAPTURE Clone

This is a GUI based version of the CAPTURE NetWare utility that allows you to
set-up the printer port redirection to NetWare printers.

### QWATCH - PCONSOLE Clone

This is a GUI based version of the PCONSOLE NetWare utility that allows you to
monitor NetWare printers and queues. It shows a summary screen with details
about all the printers on the network. You can then select a printer and get
more detailed information about the current job, such as a progress bar showing
how much of the job has been completed.

### SNDMSG - Send Message

A GUI based utility for sending messages to other users on a NetWare network. It
shows you a list of all users and groups to whom you can send a message, then
prompts you for the message to send.

### TMGFXLIB - TextMode Graphics Library

This is a graphics library for manipulating the text modes of display adapters
under DOS. It was to be a more general purpose graphics library than the NetWare
version shown above, e.g. providing blitting and other features.

This library is unfinished.

### VIEWNET - NetWare Network Scanner

A GUI based NetWare network scanner that finds all the computers on a network
and allows you to query details about each one. It is based on Ralph Davis'
NetworkMap function in his NetWare Programmers Guide.

Chris Oldwood\
28th June 2014
