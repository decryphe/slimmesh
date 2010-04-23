using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace SlimMesh
{
    public class TrackedCollection<T> : Collection<T>
    {
        public event EventHandler Changed;

        public TrackedCollection()
        {
        }

        public T[] ToArray()
        {
            T[] items = new T[Count];
            CopyTo(items, 0);

            return items;
        }

        protected virtual void OnChanged()
        {
            if (Changed != null)
                Changed(this, EventArgs.Empty);
        }

        protected override void ClearItems()
        {
            base.ClearItems();

            OnChanged();
        }

        protected override void InsertItem(int index, T item)
        {
            base.InsertItem(index, item);

            OnChanged();
        }

        protected override void RemoveItem(int index)
        {
            base.RemoveItem(index);

            OnChanged();
        }

        protected override void SetItem(int index, T item)
        {
            base.SetItem(index, item);

            OnChanged();
        }
    }
}
