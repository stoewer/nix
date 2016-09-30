[![Build Status](https://travis-ci.org/G-Node/nix.svg?branch=master)](https://travis-ci.org/G-Node/nix)
[![Build status](https://ci.appveyor.com/api/projects/status/cdupf2np8ffg5hjt/branch/master?svg=true)](https://ci.appveyor.com/project/stoewer/nix/branch/master)
[![Coverage Status](https://coveralls.io/repos/G-Node/nix/badge.svg?branch=master)](https://coveralls.io/r/G-Node/nix?branch=master)

--

About NIX
-------------

The *NIX* project started as an initiative of the Electrophysiology Task Force a part
of the [INCF](www.incf.org) Data sharing Program. 
The *NIX* data model allows to store fully annotated scientific data, i.e. the data together with its metadata within the same container. Our aim is to achieve standardization by providing a common structure for a multitude of types of datasets.
[See the wiki for more information](https://github.com/G-Node/nix/wiki)


The current implementations store the actual data using the [HDF5](www.hdfgroup.org) file format as a storage backend.


The NIX ecosystem
-----------------

In this repository you find the C++ library for reading and writing NIX files.

**Language bindings**
In addition to the C++ implementations we provide bindings in various languages:

- [Python bindings: *nixpy*](https://github.com/g-node/nixpy)
- [Matlab: *nix-mx*](https://github.com/g-node/nix-mx)
- [Java: *nix-java*](https://github.com/g-node/nix-java)

**Viewer for *NIX* files**
[nixView](https://github.com/bendalab/nixview)

**Tutorial and examples**
[nix demos](https://github.com/g-node/nix-demo)

We have assembled a set of tutorial
 [here](http://g-node.github.io/nixpy/tutorial.html) using the python
 language bindings.


Getting Started 
---------------

Installation instructions can be found:

-[Linux](https://github.com/G-Node/nix/blob/master/install_linux.md)
-[macOS](https://github.com/G-Node/nix/blob/master/install_mac.md)
-[Windows](https://github.com/G-Node/nix/blob/master/install_win.md)


NIX API Documentation
---------------------

The API documentation for the C++ library can be found [here](http://g-node.github.io/nix/)


