1.1 (2021-09-03)

        * Added hlxproxyd, a caching proxy daemon for the HLX control
          protocl and hardware.

        * Correctly observe and honor in hlxc the value passed via -t
          or --timeout.

        * Generate unique connection identifiers for servers such as
          hlxsimd (for example, 'telnet_client_4: connected') rather
          than 'telnet_client_1: connected' for all connections.

1.0.1 (2021-07-20)

        * Added version information and interfaces plus support for
          returning version information via '-V' or '--version' for
          hlxc, hlxproxyd, hlxsimd.

        * Added Linux support via Tre, OpenCFLite, and OpenCFNetwork.

1.0 (2021-05-21)

	* First public release to Github.
