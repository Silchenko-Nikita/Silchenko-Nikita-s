using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    public abstract class Building : GameObject
    {
        public Building(Texture texture, int metersWidth, int metersHeight, int metersAltitude) : base(texture, metersWidth, metersHeight, metersAltitude)
        {}
    }
}
