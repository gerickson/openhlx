This serves as a place to log active and future work independent of
source code control and the project repository hosting site.

### To Do

- [ ] Address the `hlxc` issue where it will hang indefinitely, waiting for a completion event when the issued command is effectively a no-operation because the server state is already in the requested state.
- [ ] Update the `hlxsimd` network controller to return real data based on the server responder network interface.
- [ ] Add DNS-SD/mDNS support to `hlxsimd` and `hlxproxyd`.
- [ ] Port the HLX control protocol and data schema to Matter (formerly, Project CHIP).
  - [ ] Add Matter (formerly, Project CHIP) support to `hlxc`, `hlxproxyd`, and `hlxsimd`.
- [ ] Add support for Debian (Ubuntu) packaging and archive inclusion.

### In Progress

None

### Done

- [x] Port the package to Linux.
- [x] Create `hlxproxyd` proxy daemon to address the input/output and connection shortcomings of real HLX Series hardware.

