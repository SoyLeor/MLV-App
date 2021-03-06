# :fire::fire::fire: MLV App :fire::fire::fire:
What is MLV App? A cross platform RAW conversion software that works **natively** with Magic Lantern MLV files.

![MLVApp Qt](https://image.ibb.co/hteCf6/Bildschirmfoto.png)

Find the latest releases [here](https://ilia3101.github.io/MLV-App/). Download, unpack and start.

## Features :collision:
- Import MLV files
- Support for spanned mlv (.m00, .m01, ...)
- Support for lossless mlv
- Support for any bit depth (…, 10, 12, 14bit)
- Demosaic bilinear or multithreaded AMaZE
- Processing with many parameters (exposure, white balance, saturation, dark & light adjustments, sharpen,…)
- Processing in sRGB or several LOG
- RAW corrections (fix focus & bad pixels, chroma smoothing, pattern noise, vertical stripes, deflicker)
- Support for dual ISO
- Show clip information
- Analysis: histogram, waveform monitor, RGB parade
- Session: open, import to, delete from, save, copy receipt, paste receipt (also batch paste), reset receipt, receipt file import & export, preview pictures for all clips
- Video playback in 2 modes: show each frame or drop frame mode (a kind of realtime playback which shows as many frames your computer can render)
- Audio playback in video drop frame mode
- Loop playback
- Show next frame, previous frame and scroll though timeline
- Auto load white balance, if MLV was filmed at sunny, shade, cloudy, thungsten, fluorescent, flash or kelvin
- Single frame (3x)8bit PNG export
- Clip export via ffmpeg 10bit ProRes 422 (Proxy, LT, Standard, HQ), ProRes 4444, RAW AVI, 8bit H.264 and H.265, 16bit TIFF sequence; all with or without audio
- macOS only: Clip export via Apple AVFoundation 12bit ProRes 422, ProRes 4444 and 8bit H.264, all without audio (for now)
- Clip export to (10/12/14/16bit for Dual ISO) Cinema DNG files (with default or Davinci Resolve naming). Exporting modes: uncompressed, lossless and fast pass (in the last one no RAW correction, processing or decompressing/compressing is done, raw data copied as is from MLV to DNG uncompressed or lossless).
- Frame rate override for export and playback
- Aspect Ratio: stretch width (1.33x, 1.5x, 1.75x, 1.8x, 2.0x) and height (1.67x) for playback and (ffmpeg) export per clip in session
- Resize frame resolution for (batch) export
- Clip batch export
- Zoom: fit to screen, 100% and free zoom, scroll through picture
- Zebras
- Create and load MAPP files (this files include all required information from the original MLV, plus video and audio frame index. If .MAPP file already created, importing is lot faster especially on slower HDD)

## Compiling :collision:
#### Cocoa App
```
git clone https://github.com/ilia3101/MLV-App.git
cd MLV-App/platform/cocoa
make app -j4
```

#### Qt App macOS
- install XCode depending on your OSX
- install Qt5 (minimum 5.6)
- unpack ffmpegOSX.zip in `platform/qt/FFmpeg`
- open `platform/qt/MLVApp.pro` in QtCreator
- go to tab project, add command line argument -j4 (for quad core) 
- Build and Start

#### Qt App Windows
- install Qt5 (minimum 5.6)
- unpack ffmpegWin.zip in `platform/qt/FFmpeg` (and copy it later into build directory)
- open `platform/qt/MLVApp.pro` in QtCreator
- go to tab project, add command line argument -j4 (for quad core) 
- Build and Start

#### Qt App Linux (generally)
- install Qt5 (minimum 5.6) and ffmpeg (we use v3.3.2) 
- `cd platform/qt/`
- `qmake MLVApp.pro` or equivalent (depending on distro and version and...)
- `make -j4` (for quad core)
- `./mlvapp` and have fun

## The Code
All the MLV stuff is in src/mlv

Image processing is in src/processing

Other stuff also in src/...

Platform specific/GUI things in platform/...

#### A note about code style
You may notice a strange mixture of these styles: 
1. `thisNameStyle`
2. `this_name_style`

The rule I have used in the libraries is: public functions use the thisNameStyle, and private functions use this_one.
Keep it in mind if you're going to be adding something major.

### Thanks for reading README.md

:frog:
