# NetLibDirectshowCapture
A .NET Wrapper of [libdshowcapture](https://github.com/obsproject/libdshowcapture) using .NET 8.0 C++/CLI 

This is a direct wrapper, so almost all behaviors should be consistent with [libdshowcapture](https://github.com/obsproject/libdshowcapture).

# Git Submodules
Since this repo contains [libdshowcapture](https://github.com/obsproject/libdshowcapture) as its submodule, please use `--recurse-submodules` flag when cloning.

```bash
git clone --recurse-submodules git@github.com:charlescao460/NetLibDirectshowCapture.git
``` 

# Usage Example
```C#
// List all capture devices
var videoDevices = NetLibDirectshowCapture.Device.EnumVideoDevices();
var videoDevice = videoDevices[0];
// Check device's capabilities
var cap = videoDevice.Capabilities
    .First(c => c.Format == VideoFormat.YUY2 && c.MaxCx == 1920 && c.MaxCy == 1080);
// Create config
VideoConfig videoConfig = new VideoConfig()
{
    Name = videoDevice.Name,
    Path = videoDevice.Path,
    UseDefaultConfig = false,
    Cx = cap.MaxCx,
    CyAbs = cap.MaxCy,
    CyFlip = false,
    FrameInterval = cap.MinInterval,
    InternalFormat = cap.Format,
    Format = VideoFormat.XRGB
};
byte[] managedArray = null;
// Create frame event handler
videoConfig.OnVideoCaptured += (o, e) =>
{
    managedArray ??= new byte[e.Length];
    System.Runtime.InteropServices.Marshal.Copy(e.Ptr, managedArray, 0, e.Length);
    // Process frames using managedArray...
};
// Run!
var device = new Device();
device.ResetGraph();
device.VideoConfiguration = videoConfig;
device.ConnectFilters();
device.Start();
```
