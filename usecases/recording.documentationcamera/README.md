# Recording Video

This folder provides the instructions for making video recordings from a V4L camera. The following micro-services are included: odsupercomponent, health, proxy-camera and odrecorderh264. odsupercomponent is used for software component lifecycle management in OpenDaVINCI. health checks the status of device nodes. proxy-camera provides the camera image and odrecorderh264 writes the camera images to disk. This usecase can be run on headless systems.

The recording files are stored at `~/recordings`. A recording consists of a `.rec`, a `.rec.mem` and a `.rec-DocumentationCamera0.h264` file. The filename format is `CID-xxx-odrecorderh264_yyy.zzz`, where `xxx` is the CID number from `.env`, `yyy` is the timestamp and `zzz` is the extension.

## Check for Camera Devices

Camera devices are listed in `/dev`. To check whether the camera is successfully attached, do

    $ ls /dev/video*
    
which should give you the list of attached cameras. To check whether the camera works, use the usecase `calibrate.documentationcamera`.

If the attached camera is not `/dev/video0`, modify the left side of the device mapping of `proxy-camera` in `docker-compose.yml`. For example if your camera device is `/dev/video2`, change the mapping from 

    devices:
        - /dev/video0:/dev/video0
        
to

    devices:
        - /dev/video2:/dev/video0
        
The right side of the mapping shall not be changed.
    
## Start 

To start recording, run
    
    $ docker-compose up --build

To stop recording, hit `Ctrl+C` in the terminal window. To leave the system in a clean state, stop all the containers of the usecase and remove them:

    $ docker-compose stop && docker-compose rm

## Troubleshooting

### Flipped Camera Image

It is assumed that the camera is mounted upside down, thus the video images are flipped before displaying. To disable flipping, set `proxy-camera.camera.flipped` in `configuration` to `0`.

### odsupercomponent

If the `odsupercomponent` service fails to start, try altering the `CID` in the file `.env`.

### Camera Image

To test whether the camera works, use the usecase `calibrate.documentationcamera`. Double-check whether the left-hand sides of the device mappings in `docker-compose.yml` coincide.
