using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    public class Texture
    {
        // some bitmap or smth

        public Texture(string filename)
        {
            // load from file
        }

        public Texture(byte[] bitmap)
        {
            // load from bitmap
        }

        public Texture(Texture texture)
        {
            // copy from another texture
        }

        // some methods for texture wrapping
    }
}
