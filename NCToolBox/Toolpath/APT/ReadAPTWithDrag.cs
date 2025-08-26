using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace NCToolBox.Toolpath.APT
{
    /// <summary>
    /// 由MaxPac导出的带侧倾角的刀轨
    /// </summary>
    public class ReadAPTWithDrag : ReadAPT
    {
        #region 宏
        public const int Drag = 8;
        #endregion

        public override string Head => "X,Y,Z,F,S,I,J,K,Drag";

        private const string DragPattern = @"DRAG=\s*(-?\d+(\.\d+)?)";
        private Match DragMatch;

        protected override bool ParasePosition(in string line, ref double[] point)
        {
            // GOTO/108.4843,-280.0770,-19.5434,0.380709,-0.921682,0.074586 $$PT2 DRAG=-90.0
            if (line.StartsWith("GOTO/"))
            {
                if (line.Contains("DRAG"))
                {     
                    string[] s = line.Replace("GOTO/", "").Split(',');
                    try
                    {
                        // 读取坐标
                        point[X] = float.Parse(s[0]);
                        point[Y] = float.Parse(s[1]);
                        point[Z] = float.Parse(s[2]);
                        point[I] = float.Parse(s[3]);
                        point[J] = float.Parse(s[4]);
                        point[K] = float.Parse(s[5].Split(' ')[0]);

                        // 读取DRAG
                        DragMatch = Regex.Match(line, DragPattern);
                        point[Drag] = DragMatch.Success ? float.Parse(DragMatch.Groups[1].Value) : -1;
                        return true;
                    }
                    catch { return true; }
                    
                }
                else // 如果是不含前倾角的GOTO/行，调用父类方法处理
                {
                   point[Drag] = -1;
                   base.ParasePosition(line, ref point);
                   return true;
                }
            }
            return false;
        }
    }
}
