	BCE release 6.x <http://github.com/xiaojsoft/bce>

These are the release notes for BCE version 6. Read them carefully, as they tell you what this is all about, explain how to compile and use the kernel, and what to do if something goes wrong.

WHAT IS BCE?
    BCE is a mini but functional tool which can help you balance some complex chemical equations(or Ion chemical equations), written by Ji WenCong with assistance from hackers across the Internet. It aims to simplify the calculation of chemical equations.

    It has all the features you would expect in a modern fully-fledged chemical balancer including molecule analyzer, mathematic equation solver, chemical & ion equation syntax parser.

    It is distributed under the GNU General Public License - see the accompanying COPYING file for more details.

ON WHAT PLATFORM DOES IT RUN?
    Although originally developed first for Linux operating system, today BCE kernel also runs on (at least) the FreeBSD, NetBSD, OpenBSD, Linux, Microsoft(R) Windows(TM), ReactOS, Mac OS X(TM), DOS and Solaris.

    BCE is easyily portable to most general 32- or 64-bit operating system as long as they have a port of the GNU C compiler (gcc) (part of the GNU Compiler Collection, GCC).

DOCUMENTATION:
    There are various README files in libbce/docs/ subdirectory: these typically contain kernel-specific principle for some modules.

SOFTWARE REQUIREMENTS:
    Compiling the 6.x shell requires up-to-date versions of various compiler packages (eg. gcc or mingw32).

CONFIGURING the shell:
    Do not skip this step even if you are only upgrading one minor version. New configuration options are added in each releases, and odd problems will turn up if the configuration files are not set up as expected.
    - Alternate configuration command (or commands) is (or are):
      "make config"        Plain text interface.
    NOTES on "make config"
      - Section 'COMPILE_PLATFORM' defines which architecture the program will run (it may be 'linux' or 'win32').

COMPILING the shell:
    - Make sure you have at least gcc 3.2 available.


ABOUT FX-9860 PORT
    - BY ANDERAIN LOVELACE