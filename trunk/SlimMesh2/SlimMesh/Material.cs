using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.ObjectModel;

namespace SlimMesh
{
    public class Material
    {
        public string ShaderName
        {
            get;
            set;
        }

        public Collection<MaterialParameter> Parameters
        {
            get;
            private set;
        }

        public Material()
        {
            Parameters = new Collection<MaterialParameter>();
        }
    }
}
