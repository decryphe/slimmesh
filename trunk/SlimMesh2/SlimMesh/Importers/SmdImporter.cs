using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using SlimMath;
using System.ComponentModel;

namespace SlimMesh.Importers
{
    public static class SmdImporter
    {
        static readonly TypeConverter FloatConverter = TypeDescriptor.GetConverter(typeof(float));

        public static Model Load(string fileName)
        {
            using (var stream = File.Open(fileName, FileMode.Open))
                return Load(stream);
        }

        public static Model Load(Stream stream)
        {
            Model result = new Model();
            using (var reader = new StreamReader(stream))
            {
                string line = reader.ReadLine();
                if (line != "version 1")
                    throw new InvalidDataException(string.Format("Incorrect or unexpected file version directive ('{0}').", line));

                // ignore these blocks for now
                ReadBlock(reader, "nodes");
                ReadBlock(reader, "skeleton");

                result.VertexData.Normals.StartNewChannel();
                result.VertexData.TextureCoordinates.StartNewChannel();

                var triangles = new Queue<string>(ReadBlock(reader, "triangles"));
                while (triangles.Count > 0)
                {
                    // ignore the texture
                    triangles.Dequeue();

                    for (int i = 0; i < 3; i++)
                    {
                        string triangle = triangles.Dequeue();
                        var fields = Array.ConvertAll(triangle.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries), s => (float)FloatConverter.ConvertFromInvariantString(s));

                        result.VertexData.Positions.Add(new Vector3(fields[1], fields[2], fields[3]));
                        result.VertexData.Normals.Last.Add(new Vector3(fields[4], fields[5], fields[6]));
                        result.VertexData.TextureCoordinates.Last.Add(new Vector2(fields[7], fields[8]));
                        result.Indices.Add(result.Indices.Count);
                    }
                }
            }

            return result;
        }

        static IEnumerable<string> ReadBlock(StreamReader reader, string name)
        {
            string line = reader.ReadLine();
            if (line != name)
                throw new InvalidDataException(string.Format("Expected a '{0}' block, but found a '{1}' block instead.", name, line));

            var lines = new List<string>();
            line = reader.ReadLine();
            while (line != "end")
            {
                lines.Add(line);
                line = reader.ReadLine();
            }

            return lines;
        }
    }
}
