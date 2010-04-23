using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace SlimMesh
{
    public class Model : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public VertexData VertexData
        {
            get;
            private set;
        }

        public Collection<MeshSubset> Subsets
        {
            get;
            private set;
        }

        public TrackedCollection<int> Indices
        {
            get;
            private set;
        }

        public TrackedCollection<string> Textures
        {
            get;
            private set;
        }

        public TrackedCollection<Material> Materials
        {
            get;
            private set;
        }

        public Model()
        {
            VertexData = new VertexData();
            Subsets = new Collection<MeshSubset>();
            Indices = new TrackedCollection<int>();
            Textures = new TrackedCollection<string>();
            Materials = new TrackedCollection<Material>();

            VertexData.Changed += (o, e) => OnPropertyChanged("VertexData");
            Indices.Changed += (o, e) => OnPropertyChanged("Indices");
            Textures.Changed += (o, e) => OnPropertyChanged("Textures");
            Materials.Changed += (o, e) => OnPropertyChanged("Materials");
        }

        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
