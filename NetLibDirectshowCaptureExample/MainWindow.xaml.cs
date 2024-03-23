using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using NetLibDirectshowCapture;

namespace NetLibDirectshowCaptureExample
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private List<VideoDevice> _videoDevices;
        private Device _device;

        private byte[] _rawBitmapArray;
        private bool _dirty = false;
        private int _currentHeight;
        private int _currentWidth;
        private WriteableBitmap _displayingBitmap;

        public WriteableBitmap DisplayingBitmap
        {
            get => _displayingBitmap;
            set
            {
                _displayingBitmap = value;
                OnPropertyChanged(nameof(DisplayingBitmap));
            }
        }

        public MainWindow()
        {
            _displayingBitmap = new WriteableBitmap(1920, 1080, 96, 96, PixelFormats.Bgr24, null);
            InitializeComponent();
            DSLogger.CallBack = (t, s) =>
            {
                Trace.WriteLine($"{Enum.GetName(typeof(LogType), t)}: {s}");
            };
            EnumVideoDevices();
            ListVideoDevices();
        }

        private void EnumVideoDevices()
        {
            _videoDevices = Device.EnumVideoDevices();
            _videoDevices.ForEach(device =>
            {
                device.Capabilities = device.Capabilities.Where(c => c.Format == VideoFormat.YUY2 || c.Format == VideoFormat.XRGB).ToList();
            });
        }

        private void ListVideoDevices()
        {
            // Remove old menu items
            DeviceMenu.Items.Clear();
            _videoDevices.ForEach(d =>
            {
                MenuItem toAdd = new MenuItem()
                {
                    Header = d.Name,
                    IsCheckable = true
                };
                toAdd.Checked += (s, e) =>
                {
                    OnDeviceMenuItemClick(toAdd, e, d);
                };
                DeviceMenu.Items.Insert(0, toAdd);
            });
            return;
        }


        private void OnDeviceMenuItemClick(object sender, RoutedEventArgs e, VideoDevice device)
        {
            VideoConfig videoConfig = new VideoConfig()
            {
                Name = device.Name,
                Path = device.Path,
                UseDefaultConfig = false,
                Cx = 1920,
                CyAbs = 1080,
                CyFlip = false,
                FrameInterval = 166666,
                InternalFormat = VideoFormat.XRGB,
                Format = VideoFormat.RGB24,
            };
            videoConfig.OnVideoCaptured += OnFrame;
            _device = new Device();
            if (!_device.ResetGraph())
            {
                return;
            }

            _device.VideoConfiguration = videoConfig;

            if (!_device.ConnectFilters())
            {
                return;
            }

            _currentHeight = videoConfig.CyAbs;
            _currentWidth = videoConfig.Cx;
            Process.GetCurrentProcess().PriorityClass = ProcessPriorityClass.High;
            _device.Start();
            DeviceMenu.IsEnabled = false;
            CompositionTarget.Rendering += UpdateBitmapFromRawArray;
        }


        private void OnRefreshClick(object sender, RoutedEventArgs e)
        {
            EnumVideoDevices();
            ListVideoDevices();
        }

        private void OnFrame(object sender, VideoCapturedEventArgs e)
        {
            _rawBitmapArray ??= new byte[e.Length];
            lock (_rawBitmapArray)
            {
                if (_rawBitmapArray.Length < e.Length)
                {
                    _rawBitmapArray = new byte[e.Length];
                }
                Marshal.Copy(e.Ptr, _rawBitmapArray, 0, e.Length);
                _dirty = true;
            }
        }

        private void UpdateBitmapFromRawArray(object s, EventArgs e)
        {
            if (!_dirty)
            {
                return;
            }
            WriteableBitmap display;
            int width = _currentWidth;
            int height = _currentHeight;
            if (DisplayingBitmap == null || DisplayingBitmap.PixelWidth != width ||
                DisplayingBitmap.PixelHeight != height)
            {
                display = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgr24, null);
            }
            else
            {
                display = DisplayingBitmap;
            }
            int stride = (width * display.Format.BitsPerPixel / 8);

            lock (_rawBitmapArray)
            {
                display.WritePixels(new Int32Rect(0, 0, width, height), _rawBitmapArray, stride, 0);
                _dirty = false;
            }
            DisplayingBitmap = display;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }
}
