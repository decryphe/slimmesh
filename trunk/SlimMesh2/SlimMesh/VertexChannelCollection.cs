using System;
using System.Collections.Generic;
using System.Text;

namespace SlimMesh
{
    public class VertexChannelCollection<T> : TrackedCollection<VertexChannel<T>>
    {
        public VertexChannel<T> Last
        {
            get { return Count == 0 ? null : this[Count - 1]; }
        }

        public VertexChannelCollection()
        {
        }

        public void StartNewChannel()
        {
            Add(new VertexChannel<T>());
        }

        protected override void InsertItem(int index, VertexChannel<T> item)
        {
            base.InsertItem(index, item);

            item.Changed += ItemChanged;
        }

        protected override void ClearItems()
        {
            foreach (var item in this)
                item.Changed -= ItemChanged;

            base.ClearItems();
        }

        protected override void RemoveItem(int index)
        {
            this[index].Changed -= ItemChanged;

            base.RemoveItem(index);
        }

        protected override void SetItem(int index, VertexChannel<T> item)
        {
            base.SetItem(index, item);

            item.Changed += ItemChanged;
        }

        void ItemChanged(object sender, EventArgs e)
        {
            OnChanged();
        }
    }
}
