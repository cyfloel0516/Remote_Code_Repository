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
using Newtonsoft.Json.Linq;
using RepositoryClient.Models;
using System.IO;

namespace RepositoryClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private List<RepositoryMetadata> metadatas;
        public MainWindow()
        {
            InitializeComponent();
            repo_list.ItemsSource = get_repo_list();
        }

        [DllImport("../../../Debug/ApplicationInterfaces.dll",EntryPoint = "sendRequest", CallingConvention = CallingConvention.Cdecl)]
        //public static extern class CApplicationInterfaces;
        public static extern void sendRequest(string s, StringBuilder b, int length);

        private void upload_btn_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new System.Windows.Forms.FolderBrowserDialog();
            dialog.SelectedPath = Directory.GetCurrentDirectory();
            var dialogResult = dialog.ShowDialog();

            if (dialogResult == System.Windows.Forms.DialogResult.Cancel || String.IsNullOrEmpty(dialog.SelectedPath)) return;

            //label.Content = dialog.SelectedPath;
            List<String> files = Directory.GetFiles(dialog.SelectedPath, "*.cpp").ToList();
            files.AddRange(Directory.GetFiles(dialog.SelectedPath, "*.h"));
            if(files.Count == 0){
                MessageBox.Show(this, "This folder does not contain any c++ files", "Message");
                return;
            }
            dynamic request = new
            {
                Resource = "/repository/checkin",
                FormData = new Object[] { new { key = "ModuleName", value = new DirectoryInfo(dialog.SelectedPath).Name } },
                Files = files.ToArray()
            };

            var rS = JsonConvert.SerializeObject(request);
            var result = new StringBuilder(100000);
            sendRequest(rS, result, 100000);
            repo_list.ItemsSource = get_repo_list();
        }

        private void list_btn_Click(object sender, RoutedEventArgs e)
        {
            repo_list.ItemsSource = get_repo_list();
        }

        private List<RepositoryMetadata> get_repo_list(){
            List<RepositoryMetadata> metadatas = new List<RepositoryMetadata>();
            dynamic request = new
            {
                Resource = "/repository/list",
                FormData = new Object[] { },
                Files = new string[] { }

            };
            var rS = JsonConvert.SerializeObject(request);
            //sendRequest(rS, result, 100000);
            var result = new StringBuilder(100000);
            sendRequest(rS, result, 100000);
            var resultString = result.ToString();
            //List<dynamic> jsonResult = JsonConvert.DeserializeObject<List<dynamic>>(resultString);
            List<RepositoryMetadata> jsonResult = JsonConvert.DeserializeObject<List<RepositoryMetadata>>(resultString);
            foreach (var o in jsonResult)
            {
                metadatas.Add(o);
            }
            this.metadatas = metadatas;
            return metadatas;
        }

        private void list_btn_Copy_Click(object sender, RoutedEventArgs e)
        {

        }

        private void view_detail_btn_Click(object sender, RoutedEventArgs e)
        {
            if(repo_list.SelectedItems.Count == 0){
                MessageBox.Show(this, "Please select one record in the list below.", "Cannot get detail");
                return;
            }
            DetailWindow detailWindow = new DetailWindow((RepositoryMetadata)repo_list.SelectedItems[0]);
            detailWindow.ShowDialog();
            repo_list.ItemsSource = get_repo_list();
        }
    }


}
