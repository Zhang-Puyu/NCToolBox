using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace NCToolBox.Toolpath
{
    public abstract class AbsractReader
    {
        #region 宏
        public const int X = 0;
        public const int Y = 1;
        public const int Z = 2;
        public const int F = 3;
        public const int S = 4;
        #endregion

        /// <summary>
        /// 写入文件的表头
        /// </summary>
        public abstract string Head { get; }

        /// <summary>
        /// 快速进给速度
        /// </summary>
        public double RapidFeedRate { get; set; } = -1.0;

        #region 事件
        public event Action<int> ReadLineEvent = null;
        public event Action<double[]> ParseLineEvent = null;
        public event Action<int> ReadStartEvent = null;
        public event Action<int> ReadFinishEvent = null;
        #endregion

        #region 读取

        /// <summary>
        /// 读取刀位点
        /// </summary>
        /// <param name="fileName">文件路径</param>
        /// <returns>刀位点列表</returns>
        public void Read(ref List<double[]> points, in string fileName, Encoding encoding = null)
        {
            int pointArrayLength = Head.Split(',').Length;

            points.Clear();
            points.Add(new double[pointArrayLength]);

            encoding = encoding ?? Encoding.GetEncoding("GBK");

            using (StreamReader reader = new StreamReader(fileName, encoding))
            {
                string[] lines = reader.ReadToEnd().Split(new[] { Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries);
                ReadStartEvent?.Invoke(lines.Length);

                int i = 0;
                foreach (string line in lines)
                {
                    double[] point = new double[pointArrayLength];
                    double[] lastPoint = points.Last();
                    // 复制上一个点的值
                    Array.Copy(lastPoint, point, pointArrayLength); 

                    // 解析当前行
                    ParseLine(in line, ref point);

                    ReadLineEvent?.Invoke(i++);

                    // 判断本行是否包含刀位点信息
                    if (!AreSame(point, lastPoint))
                    {
                        points.Add(point);
                        ParseLineEvent?.Invoke(point);
                    }          
                }
                reader.Close();

                ReadFinishEvent?.Invoke(points.Count);
            }

            // 移除第一个点
            points.RemoveAt(0);
        }

        /// <summary>
        /// 解析一行代码
        /// </summary>
        /// <param name="line">字符串</param>
        /// <param name="point">解析结果</param>
        protected abstract void ParseLine(in string line, ref double[] point);

        /// <summary>
        /// 判断两个刀位点是否相同, 作为一行代码解析后是否包含刀位点信息的依据
        /// </summary>
        /// <param name="p1">刀位点1</param>
        /// <param name="p2">刀位点2</param>
        /// <returns></returns>
        protected virtual bool AreSame(in double[] p1, in double[] p2)
        {
            return p1[X] == p2[X] 
                && p1[Y] == p2[Y] 
                && p1[Z] == p2[Z];
        }
        #endregion
    }
}
