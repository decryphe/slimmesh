using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.ObjectModel;

namespace SlimMesh
{
    public class VertexChannel<T> : TrackedCollection<T>
    {
        public VertexChannel()
        {
        }

        public void AddRange(IEnumerable<T> items)
        {
            foreach (var item in items)
                Add(item);
        }
    }
}
