using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    public class Field : GameObject
    {
        private List<ArgiCulture> growingCultures;

        public Field(Texture texture, int metersWidth, int metersHeight, int metersAltitude) : base(texture, metersWidth, metersHeight, metersAltitude)
        {
            growingCultures = new List<ArgiCulture>();
        }
    }
}
