using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    public class MenuButton : MenuItem, IClickable
    {
        public event OnClickHandle OnClickEvent;

        public void Click()
        {
            OnClickEvent(this, new OnClickEventArgs());
        }
    }
}
