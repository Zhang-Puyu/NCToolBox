using System;
using System.Collections.Generic;
using System.Text;

namespace NCToolBox.Toolpath.APT
{
    /// <summary>
    /// 由UG导出的带切触点的刀轨
    /// </summary>
    public class ReadAPTWithContact : ReadAPT
    {
        #region 宏
        public const int ContactX = 8;
        public const int ContactY = 9;
        public const int ContactZ = 10;
        #endregion

        public override string Head => "X,Y,Z,F,S,I,J,K,ContactX,ContactY,ContactZ";

        protected override bool ParasePosition(in string line, ref double[] point)
        {
            // GOTO/65.7226,49.2987,41.2312
            // GOTO/61.3959,49.2543,38.7258 $$ 60.0000,50.0000,37.3528
            // GOTO/59.0255,49.2300,37.3532,-0.7160734,0.2982207,0.6311128 $$ 57.7471,50.0000,36.1292
            if (line.StartsWith("GOTO/"))
            {
                if (line.Contains("$$"))
                {
                    point[S] = SpindleSpeed;
                    point[F] = RapidMark ? RapidFeedRate : FeedRate;

                    RapidMark = !RapidMark && RapidMark;

                    string[] cells = line.Split(new[] { "$$" }, StringSplitOptions.RemoveEmptyEntries);
                    string[] centerCells = cells[0].Replace("GOTO/", "").Split(',');
                    string[] contactCells = cells[1].Split(',');
                    try
                    {
                        // 读取坐标
                        point[X] = double.Parse(centerCells[0]);
                        point[Y] = double.Parse(centerCells[1]);
                        point[Z] = double.Parse(centerCells[2]);

                        // 读取接触点坐标
                        point[ContactX] = double.Parse(contactCells[0]);
                        point[ContactY] = double.Parse(contactCells[1]);
                        point[ContactZ] = double.Parse(contactCells[2]);

                        if (centerCells.Length > 3)
                        {
                            point[I] = double.Parse(centerCells[3]);
                            point[J] = double.Parse(centerCells[4]);
                            point[K] = double.Parse(centerCells[5].Split(' ')[0]);
                        }
                    }
                    catch { }
                    return true;
                }
                else
                {
                    // 如果是不带接触点的GOTO/行，调用父类方法处理
                    point[ContactX] = 0;
                    point[ContactY] = 0;
                    point[ContactZ] = 0;
                    base.ParasePosition(line, ref point);
                    return true;
                    
                }
            }
            return false;
        }
    }
}
