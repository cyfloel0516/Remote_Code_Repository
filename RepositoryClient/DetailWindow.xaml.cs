using Newtonsoft.Json;
using RepositoryClient.Models;
using System;
using System.Collections.Generic;
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
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace RepositoryClient
{
    /// <summary>
    /// Interaction logic for DetailWindow.xaml
    /// </summary>
    public partial class DetailWindow : Window
    {
        [DllImport("../../../Debug/ApplicationInterfaces.dll", EntryPoint = "getFile", CallingConvention = CallingConvention.Cdecl)]
        //public static extern class CApplicationInterfaces;
        public static extern void getFile(string s, StringBuilder b, int length);

        [DllImport("../../../Debug/ApplicationInterfaces.dll", EntryPoint = "sendRequest", CallingConvention = CallingConvention.Cdecl)]
        //public static extern class CApplicationInterfaces;
        public static extern void sendRequest(string s, StringBuilder b, int length);

        private RepositoryMetadata metadata;
        public DetailWindow(RepositoryMetadata metadata)
        {
            this.metadata = metadata;
            InitializeComponent();
            WindowStartupLocation = WindowStartupLocation.CenterOwner;
            Owner = Application.Current.MainWindow;

            NameValue.Content = metadata.Name;
            VersionValue.Content = metadata.Version;
            ClosedValue.Content = metadata.Closed.ToString();
            DepValue.Text = String.Join(", ", metadata.Dependencies);
            FilesValue.Text = String.Join(", ", metadata.FileList);
        }

        private void download_btn_Click(object sender, RoutedEventArgs e){
            dynamic request = new {
                Resource = "/repository/download",
                FormData = new Object[] { new { key = "ModuleName", value = this.metadata.Fullname }, new { key = "Dependency", value = "True" } },
                Files = new String[] { }
            };

            var rS = JsonConvert.SerializeObject(request);
            //sendRequest(rS, result, 100000);
            var result = new StringBuilder(100000);
            getFile(rS, result, 100000);
            var filePath = result.ToString();
            var messageboxResult = MessageBox.Show(
                "Your file has been saved to \n" + filePath + "\nDo you want to open it now?", "Download Success", MessageBoxButton.YesNoCancel) ;
            switch (messageboxResult)
            {
                case MessageBoxResult.Yes:
                    System.Diagnostics.Process.Start(filePath);
                    break;
                case MessageBoxResult.No:
                    break;
                case MessageBoxResult.Cancel:
                    break;
            }
        }

        private void close_btn_Click(object sender, RoutedEventArgs e){
            if (metadata.Closed){
                MessageBox.Show(this, "This module has been closed before.", "Message");
                return;
            }
            dynamic request = new{
                Resource = "/repository/close_module",
                FormData = new Object[] { new { key = "ModuleName", value = this.metadata.Fullname } },
                Files = new String[] { }
            };

            var rS = JsonConvert.SerializeObject(request);
            var result = new StringBuilder(100000);
            sendRequest(rS, result, 100000);
            MessageBox.Show(this, "Module closed", "Module closed");
            ClosedValue.Content = metadata.Closed = true;
            this.Close();
        }
    }
}
