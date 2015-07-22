#### Audio Overload SDK - Development Release 1.4.8  February 15, 2009

Copyright (c) 2007-2009 R. Belmont and Richard Bannister.
All rights reserved.


Please refer to `license.txt` for the specific licensing details of this
software.

----

This SDK opens up some of the music file format engines developed for the
Audio Overload project.
You may use this code to play the formats on systems we don't support or
inside of applications other than AO.

##### Compilation
On Windows, either MinGW or Cygwin are required to build the code through the
Makefile.

On Linux, set the environment variable `OSTYPE` to `linux` before you run
`make`:

```
	OSTYPE=linux make
```

To build on a big-endian platform, change `LSB_FIRST=1` to `=0` in the
Makefile.

By default, the code is compiled with optimizations using the `release` make
target. Use the `debug` target to generate debugging information and disable
optimizations:

```
	make debug
```

New in Release 1.4.8
- Guard against invalid data sometimes created by makessf.py (fixes crashing
  Pebble Beach ST-V rips)

----

Entry points of an AO engine are as follows:

* `int32 XXX_start(uint8 *, uint32)`

	This function attempts to recognize and load a file of a specific
	type.  It is assumed external code has already checked the file's
	signature in cases where that's possible.  The first parameter is a
	pointer to the entire file in memory, and the second is the length of
	the file.  The return value is `AO_SUCCESS` if the engine properly
	loaded the file and `AO_FAIL` if it didn't.

* `int32 XXX_sample(stereo_sample_t *)`

	This function actually plays the song, generates the next sample in
	16-bit stereo format at 44100 Hz, and returns it through the given
	pointer.

* `int32 XXX_frame(void)`

	This function is called once per frame and can be used to update the
	internal state of the format engine if necessary.

* `int32 XXX_stop(void)`

	This function ceases playback and cleans up the engine.  You must call
	`_start()` again after this to play more music.

* `int32 XXX_command(int32, int32)`

	For some engines, this allows you to send commands while a song is
	playing.  The first parameter is the command (these are defined in
	`ao.h`), the second is the parameter.  These commands are as follows:

	* `COMMAND_PREV` (parameter ignored) - for file formats which have
	  more than one song in a file (NSF), this moves back one song.

	* `COMMAND_NEXT` (parameter ignored) - for file formats which have
	  more than one song in a file (NSF), this moves forward one song.

	* `COMMAND_RESTART` (parameter ignored) - Restarts the current song
	  from the beginning.  Not supported by all engines.

	* `COMMAND_HAS_PREV` (parameter ignored) - for file formats which have
	  more than one song in a file (NSF), this checks if moving backwards
	  from the current song is a valid operation.  (Returns `AO_FAIL` if
	  not)

	* `COMMAND_HAS_NEXT` (parameter ignored) - for file formats which have
	  more than one song in a file (NSF), this checks if moving forward
	  from the current song is a valid operation.  (Returns `AO_FAIL` if
	  not)

	* `COMMAND_GET_MIN` (parameter ignored) - for file formats which have
	  more than one song in a file (NSF), this returns the lowest valid
	  song number.
	* `COMMAND_GET_MAX` (parameter ignored) - for file formats which have
	  more than one song in a file (NSF), this returns the highest valid
	  song number.

	* `COMAND_JUMP` - for file formats which have more than one song in a
	  file (NSF), this command jumps directly to a specific song number,
	  which is passed in as the parameter.

* `int32 XXX_fillinfo(ao_display_info *)`

	This function fills out the `ao_display_info` struct (see `ao.h` for
	details) with information about the currently playing song.  The
	information provided varies by engine.
