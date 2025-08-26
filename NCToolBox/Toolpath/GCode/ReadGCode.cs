using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using NCToolBox.Toolpath;

namespace NCToolBox.Toolpath.GCode
{
    public class ReadGCode : AbsractReader
    {
        #region 宏
        public const int A = 5;
        public const int B = 6;
        public const int C = 7;
        #endregion

        public override string Head => "X,Y,Z,F,S,A,B,C";

        private readonly Regex GCodeRegex = new Regex(@"(([I-Z])(-?\d+(\.\d+)?|\.\d+))");

        protected override void ParseLine(in string line, ref double[] point)
        {
            Dictionary<char, double> map = new Dictionary<char, double>();
            MatchCollection matches = GCodeRegex.Matches(line);

            foreach (Match match in matches)
            {
                if (match.Groups.Count > 2)
                {
                    char letter = match.Groups[2].Value[0];
                    string number = match.Groups[3].Value;

                    if (letter == 'G' && double.Parse(number) == 0)
                        map['F'] = RapidFeedRate;
                    else
                        map[letter] = double.Parse(number);
                }
            }

            if (map.ContainsKey('X')) point[X] = map['X'];
            if (map.ContainsKey('Y')) point[Y] = map['Y'];
            if (map.ContainsKey('Z')) point[Z] = map['Z'];
            if (map.ContainsKey('A')) point[A] = map['A'];
            if (map.ContainsKey('B')) point[B] = map['B'];
            if (map.ContainsKey('C')) point[C] = map['C'];
            if (map.ContainsKey('F')) point[F] = map['F'];
            if (map.ContainsKey('S')) point[S] = map['S'];
        }

        protected override bool AreSame(in double[] p1, in double[] p2)
        {
            return base.AreSame(p1, p2) &&
                   p1[A] == p2[A] &&
                   p1[B] == p2[B] &&
                   p1[C] == p2[C];
        }
    }
}
