# Nokryptia

Nokryptia is a tool allowing you to transfer MP3s to the [Nokia 5510](https://en.wikipedia.org/wiki/Nokia_5510) in the LSE format that device uses, and vice-versa.

Unlike Nokia's audio manager, transferred files are not encrypted, they're actually readable by classic media players, like mpv for example.

Nokryptia can't decrypt LSEs created by the Nokia Audio Manager and recorded radio streams format (RLE).

This repository is an **UNOFFICIAL** fork of the original nokryptia by Roel Derickx. Due to the shutdown of tuxmobil.org, the software was not trivial to find and was broken with modern compilers since there has been no development since 2003.

## Dependencies

- [id3lib](http://id3lib.sourceforge.net)
- a modern compiler (clang/gcc>=8) -- older versions may work

## Installation

This will build and install nokryptia and its manpage to `/usr/bin`
and `/usr/share/man/man1` respectively:

    $ make
    $ sudo make install

Note to *BSD users: you may want to set the `PREFIX` environment variable to
point to `/usr/local` instead.

## Quickstart:

    $ cd /where/my/nokia5510/is/mounted
    $ nokryptia ~/music/my.mp3

More options are available, see the manpage.

## Todo

As far as i'm concerned, my goal is simply to let nokryptia build and
run on modern toolchains, with some minor changes, but PRs are welcome!
