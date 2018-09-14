## RetroPlayer

RetroPlayer is a player core like VideoPlayer, but plays games instead of videos.

RetroPlayer uses game add-ons to play selected ROMs. Game add-ons can also be loaded standalone.

### Overview of the source code

##### `streams/`

Streams are the core abstraction used by RetroPlayer. This currently includes audio and video, and in the future input and savestates will be implemented using streams as well.

Streams arrive from game add-ons in the `streams/` subfolder. There, metadata and binary data are demultiplexed from the stream and sent to the next subsystem.

##### `audio/`

This translates audio metadata for consumption by AudioEngine.

##### `rendering/`

This handles video streams. Video data can arrive in several forms:

* Raw RGB pixels
* Kernel/GPU buffers
* Hardware framebuffer objects

##### `playback/`

This controls the playback speed, and allows for fast-forward/rewind of games.

##### `savestates/`

This is used by playback to serialize/deserialize game memory (SRAM).

##### `process/`

This contains platform-specific code.

##### `guibridge/`, `guicontrols/` and `guiwindows/`

These folders contain the code needed to interact with the GUI. The bridge is required because the GUI has different lifetime requirements than player cores.
