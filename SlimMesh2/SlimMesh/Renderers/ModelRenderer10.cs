using System;
using System.Collections.Generic;
using System.Text;
using SlimDX.Direct3D10;
using Buffer = SlimDX.Direct3D10.Buffer;
using SlimDX;

namespace SlimMesh.Renderers
{
    public class ModelRenderer10 : IDisposable
    {
        struct RenderData
        {
            public Buffer VertexBuffer;
            public Buffer IndexBuffer;
            public InputElement[] InputElements;
        }

        Device device;
        Dictionary<Model, RenderData> renderData = new Dictionary<Model, RenderData>();

        public ModelRenderer10(Device device)
        {
            this.device = device;
        }

        public void Prepare(Model model)
        {
            var renderData = new RenderData();

            using (var data = new DataStream(model.Indices.ToArray(), true, false))
                renderData.IndexBuffer = new Buffer(device, data, model.Indices.Count * sizeof(int), ResourceUsage.Default, BindFlags.IndexBuffer, CpuAccessFlags.None, ResourceOptionFlags.None);
            using (var data = new DataStream(model.VertexData.Positions.ToArray(), true, false))
                renderData.VertexBuffer = new Buffer(device, data, model.VertexData.Positions.Count * Vector3.SizeInBytes, ResourceUsage.Default, BindFlags.VertexBuffer, CpuAccessFlags.None, ResourceOptionFlags.None);


        }

        public void Render(Model model)
        {
            if (!renderData.ContainsKey(model))
                Prepare(model);
        }

        public void Dispose()
        {
            Dispose(true);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
            }
        }
    }
}
