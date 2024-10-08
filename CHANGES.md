# Changes and What's New in Open HLX

1.2.2 (2024-08-23)

* Rebased the group and zone model classes on a common ancestor.

* Updated the CFUtilities package to 1.3.2.

* Updated the LogUtilities package to 1.0.4.

* Updated the nlbuild-autotools package to 1.7.8.

* Ensured that GitHub Action Workflow continuous integration (CI)
  works on Apple Silicon and the latest release of macOS.

* Enabled xz-compressed distribution archvies.

1.2.1 (2022-02-01)

* Addressed a resource leak in libopenhlx-client and, by extension, in
  hlxc and hlxproxyd that would leak the URL scheme string (for
  example, "telnet") on each new client-to-server connection.

* Added support in contrib/systemd for a systemd service configuration
  and default command line options for hlxproxyd.

1.2 (2022-01-05)

* Added full, first-class observe (that is, read-only) support for HLX
  network schema properties, including DHCPv4 enabled state, Ethernet
  network interface EUI-48 hardware address, HLX server host default
  router / gateway IP address, HLX server host IP address, HLX server
  host IP netmask, and Control4 SDDP enabled state.

  - The `hlxc` client program and client library both handle
    asynchronous notifications for changes in these properties.

  - The `hlxproxyd` program fully proxies these properties rather than
    returning previously-composed, canned data.

  - Finally, `hlxsimd` and the server library accurately generates the
    property data based on the connection in use.

  - Even though physical HLX hardware only ever supported IPv4, the
    implementation supports both IPv4 and IPv6 and, in fact, `hlxsimd`
    and the server library will emit IPv6 property data if the
    connection is using IPv6.

1.1.1 (2021-09-26)

* Addressed an issue in which hlxproxyd was not correctly deriving and
  synthesizing group/zone state when one or more zones are members of
  one or more groups.

1.1 (2021-09-13)

* Added hlxproxyd, a caching proxy daemon for the HLX control
  protocol and hardware.

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
