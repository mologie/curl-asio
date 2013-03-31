curl-asio
=========

**Warning: Here be dragons. This library is still being developed and has not been tested yet. It compiles, that's all.**

This library makes use of libcurl's multi interface in order to enable easy integration into Boost.Asio applications.

* **simple interface** - Download and upload anything, synchronously or asynchronously, with just a few lines of code.
* **familiar** - If you have used libcurl in a C application before, you will feel right at home.
* **exceptions** - Libcurl errors throw exceptions. Integrates nicely with Boost.System's error_code class.
* **useful wrappers** - C++ interfaces for libcurl's easy, multi, form, share and string list containers. All setopt calls are wrapped into functions for type safety.
* **source/sink concept** - Works nicely with Boost.Iostreams
