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
using System.Runtime.InteropServices;
using Newtonsoft.Json;

namespace RepositoryClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        [DllImport("../../../Debug/ApplicationInterfaces.dll",EntryPoint = "sendRequest", CallingConvention = CallingConvention.Cdecl)]
        //public static extern class CApplicationInterfaces;
        public static extern void sendRequest(string s, StringBuilder b, int length);

        private void label_Loaded(object sender, RoutedEventArgs e)
        {
            dynamic request = new {
                resource = "test",
                formData = new  Object[]{ new { key = "123", value = "value" } },
                files = new string[] { "12345"}

            };
            var rS = JsonConvert.SerializeObject(request);
            var result = new StringBuilder(100000);

            sendRequest(rS, result, 100000);
            label.Content =  result;
            var s = result.ToString();
            dynamic obj = JsonConvert.DeserializeObject(s);
            //label.Content = ApplicationInterfaces.Class1.Add(1, 2);
        }
    }
}
