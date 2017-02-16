using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    public abstract class GameObject : IDisplayable
    {
        private Farm farm;
        private int metersWidth;
        private int metersHeight;
        private int metersAltitude;

        private int posX, posY;
        private Texture texture;

        public int PosX
        {
            get { return posX; }
        }

        public int PosY
        {
            get { return posY; }
        }

        public void SetPos(int posX, int posY)
        {
            if (farm.PosIsValid(posX, posY))
            {
                this.posX = posX;
                this.posY = posY;
            }
        }

        public void OnClick(){}

        public GameObject(Texture texture, int metersWidth, int metersHeight, int metersAltitude)
        {
            // if size parrams are valid
            this.metersWidth = metersWidth;
            this.metersHeight = metersHeight;
            this.metersAltitude = metersAltitude;

            this.texture = texture;
        }

        public void Display()
        {
            // do display
        }

        public void Hide()
        {
            // do hide
        }
    }
}
