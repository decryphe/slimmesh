using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.ObjectModel;

namespace SlimMesh
{
    public class MeshSubset
    {
        public int StartIndex
        {
            get;
            set;
        }

        public int IndexCount
        {
            get;
            set;
        }

        public int Material
        {
            get;
            set;
        }

        public Collection<int> Textures
        {
            get;
            private set;
        }

        public MeshSubset(int startIndex, int indexCount)
            : this(startIndex, indexCount, -1, null)
        {
        }

        public MeshSubset(int startIndex, int indexCount, int material)
            : this(startIndex, indexCount, material, null)
        {
        }

        public MeshSubset(int startIndex, int indexCount, int material, IEnumerable<int> textures)
        {
            StartIndex = startIndex;
            IndexCount = indexCount;
            Material = material;
            Textures = new Collection<int>();

            if (textures != null)
            {
                foreach (var item in textures)
                    Textures.Add(item);
            }
        }
    }
}
