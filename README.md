# NetLibDirectshowCapture
[![Build](https://github.com/charlescao460/NetLibDirectshowCapture/actions/workflows/msbuild.yml/badge.svg)](https://github.com/charlescao460/NetLibDirectshowCapture/actions/workflows/msbuild.yml)
[![NuGet](https://img.shields.io/nuget/v/NetLibDirectshowCapture
)](https://www.nuget.org/packages/NetLibDirectshowCapture) 

A .NET Wrapper of [libdshowcapture](https://github.com/obsproject/libdshowcapture) using .NET 8.0 C++/CLI 

This is a direct wrapper, so almost all behaviors should be consistent with [libdshowcapture](https://github.com/obsproject/libdshowcapture). And it should be compatible with all the devices that [OBS Studio](https://obsproject.com/) supports.

# Performance
This library is written in C++/CLI (aka. C++ Interop), which has better performance according to [Microsoft](https://learn.microsoft.com/en-us/cpp/dotnet/performance-considerations-for-interop-cpp). All the marshaling details are manually controlled for optimized performance and latency. The callback exposed the native pointer directly, which eliminate all of the unnecessary copies. It should has lower latency compared to other camera library using explicit P/Invoke. 

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

# Git Submodules
Since this repo contains [libdshowcapture](https://github.com/obsproject/libdshowcapture) as its submodule, please use `--recurse-submodules` flag when cloning.

```bash
git clone --recurse-submodules git@github.com:charlescao460/NetLibDirectshowCapture.git
``` 