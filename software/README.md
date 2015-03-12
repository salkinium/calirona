# Calirona Software

## Building from source

The sources are compiled using the [xpcc library][xpcc], which is used for
GPIO, communication, processing, build system and serial debugging.
Enter the `source` directory

	$ cd /path/to/calirona/software/source

To compile, execute:

	$ scons

To flash the binary onto the microcontroller, execute:

	$ scons program


Installing xpcc
---------------

xpcc is provided as a git submodule, to use it run this in the root `corona/` directory:

	$ git submodule init
	$ git submodule update


###### To install the xpcc build system on OS X (tested on Mavericks):

1.	Install [Homebrew][]:
		
		$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
2.	Install some dependencies:
	
		$ brew install scons python
		$ pip install lxml jinja2
3.	Install the latest version of [avr-libc][avrlibc] and avrdude using Homebrew:
		
		$ brew tap larsimmisch/avr
		$ brew install avr-libc avrdude


###### To install the xpcc build system on Linux (tested on Ubuntu 14.04):

	$ sudo apt-get update
	$ sudo apt-get install python scons python-jinja2 python-lxml \
	gcc-avr binutils-avr avr-libc avrdude


[xpcc]: http://xpcc.kreatives-chaos.com/
[homebrew]: http://brew.sh
[avrlibc]: https://github.com/larsimmisch/homebrew-avr
