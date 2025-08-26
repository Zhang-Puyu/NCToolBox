using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace NCToolBox.Extension
{
    public static class ArrayListExtension
    {
        /// <summary>
        /// 写入csv文件
        /// </summary>
        /// <param name="points">刀位点列表</param>
        /// <param name="fileName">文件名</param>
        /// <param name="head">表头</param>
        /// <param name="separator">分隔符</param>
        /// <param name="encoding">编码格式</param>
        public static void ToCSV(this List<double[]> points, in string fileName,
            string head = "", 
            char separator = ',', 
            Encoding encoding = null)
        {
            encoding = encoding ?? Encoding.UTF8;
            using (var writer = new StreamWriter(fileName, false, encoding))
            {
                if (!string.IsNullOrEmpty(head))
                    writer.WriteLine(head);

                for (int i = 0; i < points.Count; ++i)
                    writer.WriteLine(string.Join(separator.ToString(), points[i]));

                writer.Close();
            }
        }

        /// <summary>
        /// 从csv文件中读取数据
        /// </summary>
        /// <param name="points">点集</param>
        /// <param name="fileName">文件名</param>
        /// <param name="head">表头</param>
        /// <param name="separator">分隔符</param>
        /// <param name="encoding">编码格式</param>
        public static void FromCSV(this List<double[]> points, in string fileName,
            out string[] head,
            char separator = ',',
            Encoding encoding = null)
        { 
            encoding = encoding ?? Encoding.UTF8;
            using (var reader = new StreamReader(fileName, encoding))
            {
                head = reader.ReadLine().Split(separator);
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();
                    var values = line.Split(separator);
                    var point = new double[values.Length];
                    for (int i = 0; i < values.Length; ++i)
                        point[i] = double.Parse(values[i]);
                    points.Add(point);
                }
            }
        }

        /// <summary>
        /// 从csv文件中读取数据
        /// </summary>
        /// <param name="points">点集</param>
        /// <param name="fileName">文件名</param>
        /// <param name="separator">分隔符</param>
        /// <param name="encoding">编码格式</param>
        public static void FromCSV(this List<double[]> points, in string fileName,
            char separator = ',',
            Encoding encoding = null)
        {
            encoding = encoding ?? Encoding.UTF8;
            using (var reader = new StreamReader(fileName, encoding))
            {
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();
                    var values = line.Split(separator);
                    var point = new double[values.Length];
                    for (int i = 0; i < values.Length; ++i)
                        point[i] = double.Parse(values[i]);
                    points.Add(point);
                }
            }
        }
    }
}
