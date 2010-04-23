using System;
using System.Collections.Generic;
using System.Text;

namespace SlimMesh
{
    public enum ParameterType
    {
        Float,
        Bool,
        Int,
        Matrix,
        MatrixArray,
        Vector2,
        Vector3,
        Vector4,
        VectorArray,
        FloatArray,
        BoolArray,
        IntArray
    }

    public class MaterialParameter
    {
        public string Name
        {
            get;
            set;
        }

        public ParameterType DataType
        {
            get;
            set;
        }

        public object Value
        {
            get;
            set;
        }

        public MaterialParameter(string name, ParameterType dataType)
            : this(name, dataType, null)
        {
        }

        public MaterialParameter(string name, ParameterType dataType, object value)
        {
            Name = name;
            DataType = dataType;
            Value = value;
        }
    }
}
