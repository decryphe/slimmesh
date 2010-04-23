using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.ObjectModel;
using SlimMath;

namespace SlimMesh
{
    public class VertexData
    {
        public event EventHandler Changed;

        public VertexChannel<Vector3> Positions
        {
            get;
            private set;
        }

        public VertexChannelCollection<Vector3> Normals
        {
            get;
            private set;
        }

        public VertexChannelCollection<Vector3> Tangents
        {
            get;
            private set;
        }

        public VertexChannelCollection<Vector3> Binormals
        {
            get;
            private set;
        }

        public VertexChannelCollection<Color4> Colors
        {
            get;
            private set;
        }

        public VertexChannelCollection<Vector2> TextureCoordinates
        {
            get;
            private set;
        }

        public VertexData()
        {
            Positions = new VertexChannel<Vector3>();
            Normals = new VertexChannelCollection<Vector3>();
            Tangents = new VertexChannelCollection<Vector3>();
            Binormals = new VertexChannelCollection<Vector3>();
            Colors = new VertexChannelCollection<Color4>();
            TextureCoordinates = new VertexChannelCollection<Vector2>();

            Positions.Changed += ChannelChanged;
            Normals.Changed += ChannelChanged;
            Tangents.Changed += ChannelChanged;
            Binormals.Changed += ChannelChanged;
            Colors.Changed += ChannelChanged;
            TextureCoordinates.Changed += ChannelChanged;
        }

        protected virtual void OnChanged()
        {
            if (Changed != null)
                Changed(this, EventArgs.Empty);
        }

        void ChannelChanged(object sender, EventArgs e)
        {
            OnChanged();
        }
    }
}
