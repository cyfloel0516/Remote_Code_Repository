using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RepositoryClient;

namespace RepositoryClient.Models
{
    public class RepositoryMetadata
    {
        public string Name { get; set; }

        public string Version { get; set; }

        public Boolean Closed { get; set; }

        public List<String> Dependencies { get; set; }

        public List<String> FileList { get; set; }

        public String Fullname { get { return this.Name + "__" + this.Version; } }

        public RepositoryMetadata(){
            this.Dependencies = new List<String>();
            this.FileList = new List<String>();
        }
    }
}
