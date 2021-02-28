# NetLibDirectshowCapture
A .NET Wrapper of [libdshowcapture](https://github.com/obsproject/libdshowcapture) using .NET 5.0 C++/CLI 

This is a direct wrapper, so almost all behaviors should be consistent with [libdshowcapture](https://github.com/obsproject/libdshowcapture).
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
    Cx = 1920,
    CyAbs = 1080,
    CyFlip = false,
    FrameInterval = 166666,
    InternalFormat = VideoFormat.YUY2,
    Format = VideoFormat.XRGB,
};
// Create frame event handler
videoConfig.OnVideoCaptured += (o, e) =>
{
    // Process frames using e.Array
};
// Run!
var device = new Device();
device.ResetGraph();
device.VideoConfiguration = videoConfig;
device.ConnectFilters();
device.Start();
```
