[![Build Status][openhlx-travis-svg]][openhlx-travis]
[![Coverage Status][openhlx-codecov-svg]][openhlx-codecov]

[OpenHLX-travis]: https://travis-ci.com/gerickson/openhlx
[OpenHLX-travis-svg]: https://travis-ci.com/gerickson/openhlx.svg?branch=main
[OpenHLX-codecov]: https://codecov.io/gh/gerickson/openhlx
[OpenHLX-codecov-svg]: https://codecov.io/gh/gerickson/openhlx/branch/main/graph/badge.svg

Open HLX
========

# Introduction

Open HLX provides a set of command line programs and associated
libraries for interacting with the Audio Authority HLX Series Modular
Matrix System High Definition Audio/Video Switching System, which has
gone end-of-life and, as a result, is no longer manufacturered or sold
by Audio Authority.

However, thousands of such systems have been sold worldwide and there
remains a need to interact with and control such systems.

With that in mind, Open HLX provides the following libraries:

  <dl>
      <dt><strong>libopenhlx-utilities</strong></dt>
      <dd>A set of adjunct utility interfaces and objects for working
          with the other Open HLX libraries.</dd>
      <dt><strong>libopenhlx-common</strong></dt>
      <dd>A set of interfaces and objects common to both client and
          server libraries and implementations.</dd>
      <dt><strong>libopenhlx-client</strong></dt>
      <dd>A set of interfaces and objects for building HLX clients
          such as command line utilities or mobile apps.</dd>
      <dt><strong>libopenhlx-model</strong></dt>
      <dd>A set of interfaces and objects for maintaining and
          manipulating HLX data and state, such as an equalizer preset
          or zone.</dd>
  </dl>

and the following command line programs, both built with the
aforementioned libraries:

  <dl>
      <dt><strong>hlxc</strong></dt>
      <dd>A command line client for observing and mutating HLX state
          such as setting the volume of a zone.</dd>
      <dt><strong>hlxsimd</strong></dt>
      <dd>A command line client that can simulate an HLX server and,
          as a result, actual HLX Series hardware.</dd>
  </dl>

The `hlxc` client functions both against real HLX Series hardware as
well as the `hlxsimd` simulator.

## Background

Despite its end-of-life status, the HLX Series remains a high-value
and high-function audio/video matrix unrivaled and unparalled even
among actively-sold and -produced competitive products.

In most situations, an HLX system will find itself with its
telnet-based Ethernet network interface disabled and tethered, via a
serial cable to an AMX, Crestron, Control4, or other controller where
those systems act as the primary user interface and experience surface
with the HLX acting silently in the background.

However, despite its strong performance and value, whether using its
serial interface or the aforementioned network interface, the
implementation of the HLX Series control protocol has some notable
limitations that severely limit its use as a general audio/video
matrix without an intervening controller.

First, it is limited to no more than two active telnet-based network
connections. This means in a consumer/residential or
commercial/enterprise environment, no more than two `hlxc` or mobile
app users may be active at a time. Given the one-shot nature of `hlxc`
and the short background time out of mobile app platforms, this ends
up being a secondary rather than primary limitation.

Second, and perhaps more limiting, is the fact that the HLX Series
control protocol does *character* rather than *line* or *buffer* at a
time input/output and multiplexes across multiple connections at that
granularity. There are two implications of this. First, particularly
for query commands such as 'Query Current Configuration [QX]', the
response is incredibly slow. Second, particularly when more than one
connection is active, the output among any client is interleaved,
resulting in unparseable and, by extension, corrupt output. This
renders concurrent client usage all but unusable when more than one
command request and/or response is in flight.

`hlxsimd` addresses both of these limitations (and points the way for
a future effort, `hlxproxyd`) by allowing a number of client
connections limited only by the system resources on which it runs and
by performing *buffer* at a time input/output, speeding responsiveness
of the control protocol by 1,000% or more.

# Getting Started with Open HLX

## Building Open HLX

If you are not using a prebuilt distribution of Open HLX,
building Open HLX should be a straightforward, two- or three-step
process. If you are building from the main branch, start with:

    % ./bootstrap

If you are building from the main branch, a release branch, or a
distribution package, continue (main branch) or start with:

    % ./configure
    % make

The first `bootstrap` step creates the `configure` script and
`Makefile.in` files from `configure.ac` and `Makefile.am` respectively
and only needs to be done once unless those input files have changed.

The second `configure` step generates `Makefile` files from
`Makefile.in` files and only needs to be done once unless those input
files have changed.

Although not strictly necessary, the additional step of sanity
checking the build results is recommended:

    % make check

### Dependencies

In addition to depending on the C and C++ Standard Libraries and the
C++ Standard Template Library (STL), Open HLX depends on:

  * [Boost](https://www.boost.org/)
  * [CFUtilities](https://github.com/Nuovations/CFUtilities)
  * [CoreFoundation](https://developer.apple.com/documentation/corefoundation)
  * [LogUtilities](https://github.com/Nuovations/LogUtilities)
  * [libtelnet](https://github.com/seanmiddleditch/libtelnet)
  * [nlassert](https://github.com/nestlabs/nlassert)
  * [nlunit-test](https://github.com/nestlabs/nlunit-test)
  * [Doxygen](http://doxygen.nl/) (developer documentation only)
  * [asciidoc](https://asciidoc.org/) (man pages only)
  * [xmlto](https://pagure.io/xmlto/) (man pages only)

However, nlunit-test is only required when building and running the Open
HLX unit test suite.

As a familiarity to Open HLX's author and an expediency to easy
integration into iOS and macOS apps, Open HLX depends on the Apple
CoreFoundation framework or library. The framework may be provided on
the iOS or macOS (or other Apple) platforms as a native system
framework.

In the future and alternatively when supported, CoreFoundation may be
provided as a static or shared library via either the [Apple
CFLite](https://opensource.apple.com/source/CF/) project or via the
[OpenCFLite](https://opencflite.sourceforge.io/) project fork thereof.

If you want to modify or otherwise maintain the Open HLX build
system, see "Maintaining Open HLX" below for more information.

## Installing Open HLX

To install Open HLX for your use simply invoke:

    % make install

to install Open HLX in the location indicated by the --prefix
`configure` option (default "/usr/local"). If you intended an
arbitrarily relocatable Open HLX installation and passed
`--prefix=/` to `configure`, then you might use DESTDIR to, for
example install Open HLX in your user directory:

    % make DESTIDIR="${HOME}" install

## Maintaining Open HLX

If you want to maintain, enhance, extend, or otherwise modify Open
HLX, it is likely you will need to change its build system,
based on GNU autotools, in some circumstances.

After any change to the Open HLX build system, including any
*Makefile.am* files or the *configure.ac* file, you must run the
`bootstrap` or `bootstrap-configure` (which runs both `bootstrap` and
`configure` in one shot) script to update the build system.

### Dependencies

Due to its leverage of GNU autotools, if you want to modify or
otherwise maintain the Open HLX build system, the following
additional packages are required and are invoked by `bootstrap`:

  * autoconf
  * automake
  * libtool

#### Linux

When supported on Linux, on Debian-based Linux distributions such as
Ubuntu, these Open HLX build system dependencies can be satisfied with
the following:

    % sudo apt-get install autoconf automake libtool

#### Mac OS X

On Mac OS X, these dependencies can be installed and satisfied using
[Brew](https://brew.sh/):

    % brew install autoconf automake libtool

# Interact

There are numerous avenues for Open HLX support:

  * Bugs and feature requests - [submit to the Issue Tracker](https://github.com/gerickson/openhlx/issues)

# Versioning

Open HLX follows the [Semantic Versioning guidelines](http://semver.org/)
for release cycle transparency and to maintain backwards compatibility.

# License

Open HLX is released under the [Apache License, Version 2.0 license](https://opensource.org/licenses/Apache-2.0).
See the `LICENSE` file for more information.
