# Classification Algorithms Testing Environment
CATE is a framework for consistently and reproducibly extracting the key performance characteristics of classification algorithms, such as memory footprint and matching speed. Classification algorithms are utilized in network packet classification which is an essential part of varios services of today's packet-switched networks, like, for instance, firewalling, policy routing, intrusion detection, and traffic rate limiting.

## Quick Start Guide
1. Clone CATE.

        $ git clone https://github.com/gusew/cate.git
  
2. Move into the CATE directory.

        $ cd cate/
  
3. Build CATE and dependencies.

        $ make
  
4. Create a directory for output results.

        $ mkdir results 
  
5. Run CATE with an example benchmark suite (1,000 rules and all algorithms).

        $ ./build/cate examples/all-acl1-1K.lua results/


## Implementations of Classification Algorithms
Currently, the following algorithms are implemented for the CATE framework:
* Linear Search
* [Bit Vector Search](http://conferences.sigcomm.org/sigcomm/1998/tp/paper17.pdf)
* [Hierarchical Intelligent Cuttings](http://tiny-tera.stanford.edu/~nickm/papers/HOTI_99.pdf) (HiCuts)
* [Tuple Space Search](http://cseweb.ucsd.edu/~varghese/PAPERS/Sigcomm99.pdf)

## Dependencies 
CATE has three dependencies which are included in this repository and which are automatically build by using the main makefile. These libraries are
* [Lua Interpreter](http://lua.org)
* [The GNU Multiple Precision Arithmetic Library](https://gmplib.org)
* A lightweight and portable [C++ unit testing library](http://libunittest.sourceforge.net)

## Contributing
We cordially welcome any implementations of further classification algorithms as well as patches to the CATE framework. For patches, please start by opening a new issue describing the change you are going to make. Then, [fork](https://help.github.com/articles/fork-a-repo) the project or create a new branch where you will be working at. Finally, push the commits to your fork and submit a [pull request](https://help.github.com/articles/creating-a-pull-request). 

