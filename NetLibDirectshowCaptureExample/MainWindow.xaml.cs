using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
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
    public partial class MainWindow : Window
    {
        private List<VideoDevice> _videoDevices;

        public MainWindow()
        {
            InitializeComponent();
            ListVideoDevices();
        }

        private void ListVideoDevices()
        {
            // Remove old menu items
            int previousNumberOfDevices = _videoDevices?.Count ?? 0;
            for (int i = 0; i < previousNumberOfDevices; ++i)
            {
                ConnectMenu.Items.RemoveAt(0);
            }
            // Enumerate devices and add new menu items
            _videoDevices = Device.EnumVideoDevices();
            _videoDevices.ForEach(d =>
            {
                MenuItem toAdd = new MenuItem()
                {
                    Header = d.Name,
                };
                ConnectMenu.Items.Insert(0, toAdd);
            });
            // Update items in dialog menu
            UpdateDirectshowDialogMenu();
            return;
        }

        private void UpdateDirectshowDialogMenu()
        {
            DialogMenu.Items.Clear();
            _videoDevices.ForEach(d =>
            {
                MenuItem toAdd = new MenuItem()
                {
                    Header = d.Name,
                };
                DialogMenu.Items.Insert(0, toAdd);
            });
        }

        private void OnDialogMenuItemClick(object sender, RoutedEventArgs e, VideoDevice device)
        {
            
        }

        private void OnRefreshClick(object sender, RoutedEventArgs e)
        {
            ListVideoDevices();
        }
    }
}
