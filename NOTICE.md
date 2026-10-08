# Notices

This template vendors two MIT-licensed libraries:

- `extern/CommonLibSSE`: CommonLibSSE NG 3.7.0 (commit `b93280e8`), with the changes listed in `README.md`.
- `extern/CommonLibVR`: CommonLibVR, used only for the VR variant.

Keep their `LICENSE` files and the licenses of the vcpkg dependencies when redistributing this repository:
spdlog (MIT), rapidcsv (BSD-3-Clause), and for the VR variant rsm-binary-io (MIT) and boost-stl-interfaces
(Boost Software License 1.0).

`extern/CommonLibVR` also carries the sources of its own third-party dependencies (for example openvr,
BSD-3-Clause) with their license files; they are only used by the VR variant.

No GPL-licensed code may be added.
