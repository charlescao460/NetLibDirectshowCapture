using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
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
                device.Capabilities = device.Capabilities.Where(c => c.Format == VideoFormat.ARGB || c.Format == VideoFormat.XRGB).ToList();
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
                    OnDialogMenuItemClick(toAdd, e, d);
                };
                DeviceMenu.Items.Insert(0, toAdd);
            });
            return;
        }


        private void OnDialogMenuItemClick(object sender, RoutedEventArgs e, VideoDevice device)
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
                Format = VideoFormat.XRGB
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
            _device.Start();
            CompositionTarget.Rendering += UpdateBitmapFromRawArray;
        }


        private void OnRefreshClick(object sender, RoutedEventArgs e)
        {
            EnumVideoDevices();
            ListVideoDevices();
        }

        private void OnFrame(object sender, VideoCapturedEventArgs e)
        {
            //Debug.Write("Frame");

            int rgbLength = e.Config.Cx * e.Config.CyAbs * 3;
            _rawBitmapArray ??= new byte[rgbLength];

            lock (_rawBitmapArray)
            {
                if (_rawBitmapArray.Length < rgbLength)
                {
                    _rawBitmapArray = new byte[rgbLength];
                }

                switch (e.Config.Format)
                {
                    case VideoFormat.XRGB:

                        int stride = 3 * e.Config.Cx;
                        int baseIndex = 0;
                        int copyIndex = stride - 1;
                        var src = e.Array;
                        // Flip y, Flip x
                        for (int i = e.Length - 1 - 4; i >= 0; i -= 4)
                        {
                            // R
                            _rawBitmapArray[baseIndex + copyIndex--] = src[i + 3];
                            // G
                            _rawBitmapArray[baseIndex + copyIndex--] = src[i + 2];
                            // B
                            _rawBitmapArray[baseIndex + copyIndex--] = src[i + 1];
                            if (copyIndex < 0)
                            {
                                baseIndex += stride;
                                copyIndex = stride - 1;
                            }
                        }
                        break;
                    default:
                        throw new NotImplementedException($"{Enum.GetName(e.Config.Format)} is not supported.");
                }
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
