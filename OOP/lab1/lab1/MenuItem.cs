using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    public abstract class MenuItem : IDisplayable
    {   

        private String label;
        private int pxWidth;
        private int pxHieght;
        private int x, y;

        public MenuItem()
        {
            pxWidth = pxHieght = x = y = 0;
        }

        public String Label
        {
            get { return label; }
            set
            {
                if(label.Length < 80)
                    label = value;
            }
        }

        public virtual void SetSize(int pxWidth, int pxHieght)
        {
            this.pxWidth = pxWidth;
            this.pxHieght = pxHieght;
        }

        public virtual void SetPos(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

        public void Display()
        {
            // display item;
        }

        public void Hide()
        {
            // hide item
        }
    }
}
