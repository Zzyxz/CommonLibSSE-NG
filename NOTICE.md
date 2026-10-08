# Notices

This template vendors two MIT-licensed libraries:

- `extern/CommonLibSSE`: CommonLibSSE NG 3.7.0 (commit `b93280e8`), with the changes listed in `README.md`.
- `extern/CommonLibVR`: CommonLibVR from the previous template. The build no longer uses it; the VR variant is built with CommonLibSSE NG.

Keep their `LICENSE` files and the licenses of the vcpkg dependencies when redistributing this repository:
spdlog (MIT) and rapidcsv (BSD-3-Clause).

`extern/CommonLibVR` also carries the sources of its own third-party dependencies (for example openvr,
BSD-3-Clause) with their license files.

No GPL-licensed code may be added.
