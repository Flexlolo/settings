README
======

Description
-----------

Program that implements persistent value storage for my shell scripts. It uses env variable to determine path. Each variable is stored in file with its name.

Compilation and installation
----------------------------

	make
	make install

Usage
-----
	settings get test-setting
	settings set test-setting test-value
	settings toggle bool-setting
	settings toggle custom-setting 1 2 3 4