using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using NCToolBox.Toolpath;

namespace NCToolBox.Toolpath.APT
{
    public class ReadAPT : AbsractReader
    {
        #region 宏
        public const int I = 5;
        public const int J = 6;
        public const int K = 7;
        #endregion

        #region 表头
        public override string Head => "X,Y,Z,F,S,I,J,K";
        #endregion

        #region 切削参数
        /// <summary>
        /// 进给
        /// </summary>
        protected double FeedRate = 0.0;
        /// <summary>
        /// 主轴转速
        /// </summary>
        protected double SpindleSpeed = 0.0;

        /// <summary>
        /// 快速进给标记
        /// </summary>
        protected bool RapidMark = false;
        #endregion

        #region 代码解析
        protected override void ParseLine(in string line, ref double[] point)
        {
            if (ParasePosition(line, ref point)) return;
            if (ParaseFeedRate(line, point))     return;
            if (ParaseSpindlSpeed(line))         return;
        }

        /// <summary>
        /// 解析刀位点
        /// </summary>
        /// <param name="line">一行APT代码</param>
        /// <param name="point">刀位点</param>
        /// <returns>当前行是否包含刀位点信息</returns>
        protected virtual bool ParasePosition(in string line, ref double[] point)
        {
            // NREC: GOTO/108.4843,-280.0770,-19.5434,0.380709,-0.921682,0.074586 $$PT2
            // UG:   GOTO/97.8011,83.8859,-11.4651
            if (line.StartsWith("GOTO/"))
            {
                point[S] = SpindleSpeed;
                point[F] = RapidMark ? RapidFeedRate : FeedRate;

                RapidMark = !RapidMark && RapidMark;

                string[] cells = line.Replace("GOTO/", "")
                    .Split(new string[] { "$$" }, StringSplitOptions.None)
                    .First().Trim()
                    .Split(',');
                try
                {
                    // 读取坐标
                    point[X] = double.Parse(cells[0]);
                    point[Y] = double.Parse(cells[1]);
                    point[Z] = double.Parse(cells[2]);
                    if (cells.Length > 3)
                    {
                        point[I] = double.Parse(cells[3]);
                        point[J] = double.Parse(cells[4]);
                        point[K] = double.Parse(cells[5]);
                    }
                }
                catch { }
                return true;
            }
            return false;
        }

        /// <summary>
        /// 解析进给速度
        /// </summary>
        /// <param name="line">一行APT代码</param>
        /// <param name="point">刀位点</param>
        /// <returns>当前行是否包含进给速度信息</returns>
        protected virtual bool ParaseFeedRate(in string line, in double[] point)
        {
            // FEDRAT/MMPM,250.0000
            if (line.StartsWith("FEDRAT/MMPM"))
            {
                FeedRate = double.Parse(line.Split(',').Last());
                return true;
            }
            // FEDRAT/6000.0000
            if (line.StartsWith("FEDRAT/"))
            {
                FeedRate = double.Parse(line.Split('/').Last());
                return true;
            }
            if (line.StartsWith("RAPID"))
            {
                RapidMark = true;
                return true;
            }
            return false;
        }

        /// <summary>
        /// 解析主轴转速
        /// </summary>
        /// <param name="line">一行APT代码</param>
        /// <returns>当前行是否包含主轴转速信息</returns>
        protected virtual bool ParaseSpindlSpeed(in string line)
        {
            // SPINDL/ 2300, CLW
            if (line.StartsWith("SPINDL/"))
            {
                SpindleSpeed = double.Parse(Regex.Replace(line.Split('/').Last(), @",.*", "").Trim());
                return true;
            }
            return false;
        }

        protected override bool AreSame(in double[] p1, in double[] p2)
        {
            return base.AreSame(p1, p2) && 
                   p1[I] == p2[I] &&
                   p1[J] == p2[J] &&
                   p1[K] == p2[K];
        }
        #endregion
    }
}
