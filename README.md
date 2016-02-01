<p align="center">
<img src="https://github.com/gusew/cate/blob/gh-pages/images/logo-cate.png" width="300" height="112"/>
</p>

# Classification Algorithms Testing Environment
CATE is a framework for consistently and reproducibly extracting the key performance characteristics of classification algorithms, such as memory footprint and matching speed. Classification algorithms are utilized in network packet classification which is an essential part of various services of today's packet-switched networks, like, for instance, firewalling, policy routing, intrusion detection, and traffic rate limiting.

## Quick Start Guide
1. Clone CATE (or extract the zip-file).

        $ git clone https://github.com/gusew/cate.git
  
2. Change into the CATE directory.

        $ cd cate/
  
3. Build CATE and dependencies.

        $ make
  
4. Create a directory for output results.

        $ mkdir results 
  
5. Run CATE with an example benchmark suite (1,000 rules and all algorithms).

        $ ./build/cate/cate examples/all-acl1-1K.lua results/


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


## Build CATE with disabled memory metering
Measuring the memory footprint of algorithms can be disabled by building the CATE framework with the preprocessor directive 'MEMTRACE_DISABLED'. In order to build CATE with deactivated memory metering, just use the makefile target 'build_all_nomem':

        $ make clean
        $ make build_all_nomem
        

## Run unit tests
You can execute all available unit tests with the following makefile-targets:

1. Test main components of the CATE framework.

        $ make utest
        
2. Test parts of each classification algorithm.

        $ make utest_linearsearch
        $ make utest_bitvector
        $ make utest_hicuts
        $ make utest_tuples

3. Or test all classification algorithms at once.

        $ make utest_all_algs


## How to add a new algorithm implementation?
You can extend the current set of available classification algorithms with your own implementation by the following steps:

1. Create a new sub-directory in 'cate/include/algorithms/' and 'cate/src/algorithms/'

2. Define your custom classification algorithm implementation as a class at 'cate/include/algorithms/myclassdir/MyClassAlg.hpp'. Here is a minimalistic example:

		#include <generics/Base.hpp>

		struct MyClassAlg : public Base {
			inline ~MyClassAlg() override {}
			
			/**
			 * Set algorithm parameters as specified by a benchmark configuration.
			 *
			 * @param params collection with algorithm-specific parameters
			 */
			void setParameters(const std::vector<double>& params) override;

			/**
			 * Classify incoming packet header data by finding the best matching rule
			 * in the given rule set with a specific algorithm.
			 *
			 * @param data container with packet header data
			 * @param indices container with indices of matched rules in rule set
			 */
			void classify(const Generic::PacketHeaderSet& data, Generic::RuleIndexSet& indices) override;

			/**
			 * Set rules for algorithm and let it convert them to a native implementation, 
			 * if needed. 
			 *
			 * @param ruleset reference to a generic ruleset structure
			 */
			void setRules(const Generic::RuleSet& ruleset) override;


			/** Reset algorithm instance to initial state. */
			void reset() override;
		};


3. Implement the functions 'setParameters', 'classify', 'setRules', and 'reset' in 'cate/src/algorithms/myclassdir/MyClassAlg.cpp'.

4. Create a Makefile with build targets for your algorithm in 'cate/make_alg_myclass.mk'. Here is a small example how such a Makefile could look like:

		ALG_MY_HPPS_DIR	= $(ALG_HPPS_DIR)myclassdir/
		ALG_MY_SRCS_DIR	= $(ALG_SRCS_DIR)myclassdir/

		# dependencies for the shared library
		ALG_MY_OBJ	= $(ALG_OBJ_BASIC) $(ALG_OBJ_TPL) $(ALG_OBJ_DIR)MyClassAlg.o

		# General targets for building an algorithm's object files
		$(ALG_OBJ_DIR)%.o: $(ALG_MY_SRCS_DIR)%.cpp $(ALG_MY_HPPS_DIR)%.hpp
			$(MKDIR)
			$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

		# Build a shared library of the classification algorithm
		$(ALG_LIB_DIR)MyClassAlg.so: $(ALG_MY_OBJ)
			$(MKDIR)
			$(CC) $(CFLAGS) $(CSOFLAGS) -I $(INCLUDE) $^ -o $@ $(GMP_BUILD_XXA) $(GMP_BUILD_A)

5. Register the new Makefile in 'cate/make_alg_myclass.mk':
		
		# PLEASE ADD YOUR ALGORITHM IMPLEMENTATION TO THIS LIST:
		ALG_LIST	= \
			# (...)
			$(ALG_LIB_DIR)MyClassAlg.so

		# Makefiles with targets for each algorithm implementation.
		include make_alg_common.mk
		# (...)
		# PLEASE ADD YOUR MAKEFILE FOR YOUR CUSTOM ALGORITHM HERE
		include make_alg_myclass.mk

6. Now, you could actually build your algorithm with

		$ make build_algorithms
		
7. Before or after your algorithm's coding, you could define unit tests for your implementation. To this end, add another file with unit test specifications, e. g., 'cate/test/AlgMyClass.cpp'. Then, you have to extend the Makefile structure by the new tests in order to build your unit tests. Create a corresponding Makefile, for example 'cate/make_test_alg_myclassalg.mk' with the target 'utest_myclassalg'. Register this Makefile in 'cate/make_test.mk'. Finally, you can build and run the unit tests for your implementation with
		
		$ make utest_myclassalg

For questions or more information on this topic, write me an [email](mailto:wladislaw@gusew.de).


## Publications

**Main Paper**: if your work uses or extends CATE, please refer to this paper

> [CATE: An Open and Highly Configurable Framework for Performance Evaluation of Packet Classification Algorithms](http://informs-sim.org)
> W. Gusew, S. Hager, and B. Scheuermann, in Proceedings of Winter Simulation Conference, Huntington Beach, USA, Dec 2015
>
> [BibTex](http://ti-publications.informatik.hu-berlin.de/publications/194/?bibtex) · [Presentation Slides](http://amor.cms.hu-berlin.de/~gusewwly/files/cate-wsc15-slides.pdf) · [Paper](http://informs-sim.org/wsc15papers/by_area.html#ptrack118)

