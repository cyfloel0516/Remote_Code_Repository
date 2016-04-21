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

        [DllImport("../../../Debug/ApplicationInterfaces.dll",EntryPoint = "fnApplicationInterfaces", CallingConvention = CallingConvention.Cdecl)]
        //public static extern class CApplicationInterfaces;
        public static extern int fnApplicationInterfaces();


        private void label_Loaded(object sender, RoutedEventArgs e)
        {
            label.Content = fnApplicationInterfaces();
            //label.Content = ApplicationInterfaces.Class1.Add(1, 2);
        }
    }
}
