SHELL=/bin/sh

prefix?=/usr/local
exec_prefix?=$(prefix)

# Executable for user
bindir?=$(exec_prefix)/bin

# Shell executable for administrator
sbindidr?=$(exec_prefix)/sbin

# Executable for program
libexecdir?=$(exec_prefix)/libexec

# Root directory for read-only architecture-independant data files
datarootdir?=$(prefix)/share

# Directory for installing idiosyncratic (particular) read-only
# architecture-independant data files
datadir?=$(datarootdir)

# Directory for installing read-only data files that pertain to a
# single machine
sysconfdir?=$(prefix)/etc

# Directory for installing archiecture-independant data files which
# the programs modify while they run
sharedstatedir?=$(prefix)/com

# Directory for installing architecture-independent data files which
# the programs modifiy while they run
localstatedir?=$(prefix)/var

# Directory for installing data files which the programs modify while
# they run, theat pertain to one specifig machine and persist longer
# thant the execution of the program
runstatedir?=$(localstatedir)/run

# Directory for installing header files to be included by the user
# programs
includedir?=$(prefix)/include

# Directory for installing header files for use with compilers other
# than GCC
oldincludedir?=/usr/include

# Directory for installing documentation files (other than Info)
docdir?=$(datarootdir)/doc/YAROS

# Directory for installing the Info files
infodir?=$(datarootdir)/info

# Directories for installing documentation files
htmldir?=$(docdir)
dvidir?=$(docdir)
pdfdir?=$(docdir)
psdir?=$(docdir)

# Directory for object files and libraries of object code
libdir?=$(exec_prefix)/lib

# Directory for installing any Emacs Lisp files
lispdir?=$(datarootdir)/emacs/site-lisp

# Directory for installing locale-specifig message
localedir?=$(datarootdir)/locale

# Top-level directory for installing man pages
mandir?=$(datarootdir)/man

# Directory for installing section 1
man1dir=$(mandir)/man1

# Section 2
man2dir=$(mandir)/man2

#manext?=.
man1ext=$(manext)1
man2ext=$(manext)2

# Directory for the sources being compiled
#srcdir?=
