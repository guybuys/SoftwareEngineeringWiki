# B0381

## 2MP Global Shutter OV2311 Mono Camera Modules Pivariety(NoIR ), compatible with Raspberry Pi ISP and Gstreamer Plugin-Arducam

### Setting up (install) the camera module
#### Weblinks

[ArduCam Module website](https://www.arducam.com/arducam-2mp-ov2311-global-shutter-noir-mono-camera-modules-pivariety.html): Hierop kan je o.m. een **Wiki** vinden.

[ArduCam Quick Start Guide](https://docs.arducam.com/Raspberry-Pi-Camera/Pivariety-Camera/Quick-Start-Guide/): There are two main sections in this guide: **Hardware Connection** and **Software Configuration**.

#### Turn on I2C

Even though the camera’s internal I2C isn’t always exposed as /dev/i2c-*, on Raspberry Pi OS you should normally have at least /dev/i2c-1 once enabled.

Run:

```bash
sudo raspi-config
```
Then:

Interface Options
- Enable I2C
- Reboot:

```bash
sudo reboot
```

After reboot:

```bash
ls -l /dev/i2c*
```
If `/dev/i2c-1` appears, good.

---
### Using the camera

To check if camera is there and to make a test picture:

```bash
rpicam-hello --list-cameras
rpicam-still -o test.jpg
```